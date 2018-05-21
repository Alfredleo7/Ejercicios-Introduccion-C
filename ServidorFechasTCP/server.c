/****************************************************************/
/* FICHERO:     servidor.c                                      */
/* DESCRIPCION: codigo de un servidor de fechas                 */
/****************************************************************/
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
  /* Declaramos las direcciones de los servidores y los sockets */
  struct sockaddr_in direccionServidor, direccionCliente;
  int descSocket, nuevoDescSocket;
  socklen_t longDirCliente; /* longitud de la direccionCliente*/
  int codigoOperacion;
  /* Se pone a cero la direccion del server */
  memset(&direccionServidor, 0, sizeof(direccionServidor));
  direccionServidor.sin_family = AF_INET;
  direccionServidor.sin_addr.s_addr = INADDR_ANY; /* Lo ponemos a cualquiera,
                                                     ya que aceptaremos
                                                     conexiones de todas las
                                                     IPs */
  direccionServidor.sin_port = htons(atoi(puerto_tcp));
  /* Vincula el proceso con el puerto de escucha indicado en la variable
     direccionServidor*/
  /* Creacion del socket para TCP*/
  descSocket = socket(PF_INET, SOCK_STREAM, 0);
  if(descSocket < 0){
    perror("No se ha podido abrir el socket");
    exit(-1);
  }
  if(bind(descSocket, (struct sockaddr *) &direccionServidor,
          sizeof(direccionServidor)) < 0){
    perror("Error al vincular la direccion local");
    exit(-1);
  }
  /* Pone el socket en modo pasivo para que pueda aceptar conexiones */
  if(listen(descSocket, 5) < 0){/* Cinco clientes pueden esperar su turno en cola */
    perror("listen");
    exit(-1);
  }
  while(1){
    printf("Servidor esperando conexion...\n");
    /* Espera una conexion del cliente, devuelve dicha conexion en nuevoDescSocket */
    nuevoDescSocket = accept(descSocket, (struct sockaddr *) &direccionCliente,
                             &longDirCliente);
    if(nuevoDescSocket < 0){
      perror("Accept");
      close(nuevoDescSocket);
      close(descSocket);
      exit(-1);
    }
    /* Ya se ha aceptado la conexion del cliente. Dicha conexion esta en nuevoDescSocket */
    printf("Nueva conexion aceptada\n");
    if(nuevoDescSocket < 0){
      /* ERROR */
      printf("Error");
      exit(-1);
    }
    /* Recibimos los datos de codigoOperacion */
    uint32_t codOpBE;
    if(recv(nuevoDescSocket, &codOpBE, sizeof(codOpBE), 0)  != sizeof(codOpBE)){
      perror("Recv");
      exit(-1);
    }
    codigoOperacion = ntohl(codOpBE); /* Pasamos de big endian a int */
    switch(codigoOperacion){
    case SOLICITUD_FECHA: {
      /* Nos solicitan la fecha, por lo que la obtenemos con time() */
      time_t fechaActual = time(0);
      /* Pasamos la fecha a un formato tm, mas comodo */
      struct tm *fechaPtr = gmtime(&fechaActual);
      struct FechaYHora fecha;
      /* Almacenamos en nuestro struct fecha y hora */
      fecha.dia = fechaPtr->tm_mday;
      fecha.mes = fechaPtr->tm_mon + 1;/* Devuelve los meses del 0 al 11 */
      fecha.anno = fechaPtr->tm_year + 1900;/* Devuelve los meses del 0 al 11 */
      fecha.hora = fechaPtr->tm_hour;
      fecha.minuto = fechaPtr->tm_min;
      fecha.segundo = fechaPtr->tm_sec;
      /* Pasamos los datos a formato de red */
      codigoOperacion = RESPUESTA_FECHA;
      printf("%d:%d:%d %d-%d-%d\n", fecha.hora, fecha.minuto, fecha.segundo,
             fecha.dia, fecha.mes, fecha.anno);
      codOpBE = htonl(codigoOperacion);
      if(write(nuevoDescSocket, &codOpBE, sizeof(codOpBE)) != sizeof(codOpBE)){
        perror("write");
        exit(-1);
      }
      /* Pasamos los datos del struct UNO A UNO */
      if((write(nuevoDescSocket, &fecha.dia, sizeof(fecha.dia)))!= sizeof(fecha.dia)){
        perror("write");
        exit(-1);
      }
      printf("He mandado dia\n");
      if((write(nuevoDescSocket, &fecha.mes, sizeof(fecha.mes))!= sizeof(fecha.mes))){
        perror("write");
        exit(-1);
      }
      printf("He mandado mes\n");
      if((write(nuevoDescSocket, &fecha.anno, sizeof(fecha.anno))!= sizeof(fecha.anno))){
        perror("write");
        exit(-1);
      }
      printf("He mandado anno\n");
      if((write(nuevoDescSocket, &fecha.hora, sizeof(fecha.hora))!= sizeof(fecha.hora))){
        perror("write");
        exit(-1);
      }
      printf("He mandado hora\n");
      if((write(nuevoDescSocket, &fecha.minuto, sizeof(fecha.minuto))!= sizeof(fecha.minuto))){
        perror("write");
        exit(-1);
      }
      printf("He mandado minuto\n");
      if((write(nuevoDescSocket, &fecha.segundo, sizeof(fecha.segundo))!= sizeof(fecha.segundo))){
        perror("write");
        exit(-1);
      }
      printf("He mandado minuto\n");
      break;
    }
    default:
      printf("SERVIDOR: Mensaje no valido: %d\n", codigoOperacion);
      exit(-1);
    }
    /* Fin de switch */
  }
  /*fin de while */
}
