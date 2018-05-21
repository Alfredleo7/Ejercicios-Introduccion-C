#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "defs.h"
#include <unistd.h>

int main(){
  /*1. Declaramos variables */
  struct sockaddr_in direccionServidor;
  int descSocket;
  int codigoOperacion;
  struct FechaYHora fecha;

  /*2. Creamos la direccion del cliente */
  /*--2.1 Inicializamos */
  memset(&direccionServidor, 0, sizeof(direccionServidor));
  /*--2.2 Rellenamos los campos */
  direccionServidor.sin_family = AF_INET;
  direccionServidor.sin_port = htons(atoi(puerto_tcp));
  direccionServidor.sin_addr.s_addr = inet_addr(direccion_ip);

  /*3. Creamos el socket */
  if((descSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    perror("Error en socket()");
    exit(-1);
  }
  /*4. Nos conectamos al servidor */
  if(connect(descSocket, (struct sockaddr *) &direccionServidor,
             sizeof(direccionServidor)) < 0){
    perror("Error al conectar");
    exit(-1);
  }
  printf("Conexion con el servidor establecida...\n");
  /*5. Transmitimos los datos */
  codigoOperacion = SOLICITUD_FECHA;
  /* Pasamos el codigo a formato de red */
  uint32_t codigoOpBE = htonl(codigoOperacion);
  /* Enviamos el codigo y esperamos respuesta */
  if((send(descSocket, &codigoOpBE, sizeof(codigoOpBE), 0)) != sizeof(codigoOpBE)){
    perror("Error en el envio");
    exit(-1);
  }
  if((recv(descSocket, &codigoOpBE, sizeof(codigoOpBE), 0)) != sizeof(codigoOpBE)){
    perror("Error al recibir codigo");
    exit(-1);
  }
  codigoOperacion = ntohl(codigoOpBE);
  /* Recibimos la fecha campo por campo en el mismo orden que la hemos
   enviado */
  if(codigoOperacion == RESPUESTA_FECHA){
    if((recv(descSocket, &fecha.dia, sizeof(fecha.dia), 0)) != sizeof(fecha.dia)){
      perror("Error al recibir");
      exit(-1);
    }
    if((recv(descSocket, &fecha.mes, sizeof(fecha.mes), 0))  != sizeof(fecha.mes)){
      perror("Error al recibir");
      exit(-1);
    }
    if((recv(descSocket, &fecha.anno, sizeof(fecha.anno), 0))  != sizeof(fecha.anno)){
      perror("Error al recibir");
      exit(-1);
    }
    if((recv(descSocket, &fecha.hora, sizeof(fecha.hora), 0))  != sizeof(fecha.hora)){
      perror("Error al recibir");
      exit(-1);
    }
    if((recv(descSocket, &fecha.minuto, sizeof(fecha.minuto), 0))  != sizeof(fecha.minuto)){
      perror("Error al recibir");
      exit(-1);
    }
      if((recv(descSocket, &fecha.segundo, sizeof(fecha.segundo), 0)) !=  sizeof(fecha.segundo)){
      perror("Error al recibir");
      exit(-1);
    }
    printf("%d:%d:%d %d-%d-%d\n", fecha.hora, fecha.minuto, fecha.segundo,
           fecha.dia, fecha.mes, fecha.anno);
  } else {
    printf("Codigo erroneo!");
    exit(-1);
  }
  return 0;
}
