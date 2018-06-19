#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <stdio.h>

#define PUERTO 4950
#define MAXBUFF 512


int main(int argc, char *argv[]){
  /* Declaramos variables */
  int descSocket;
  char buffer[MAXBUFF];
  int longitud, bytes;
  uint32_t longitudBE;
  struct sockaddr_in dirServer;

  /* Inicializamos la direccion*/
  memset(&dirServer, 0, sizeof(dirServer));

  dirServer.sin_family = AF_INET;
  dirServer.sin_port = htons(PUERTO);
  dirServer.sin_addr.s_addr = inet_addr(argv[1]);

  /* Creamos el socket */
  if((descSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    perror("Error al abrir el socket");
    exit(1);
  }
  printf("Intentando establecer conexion con el servidor...\n");

  /* Nos conectamos al server */
  if(connect(descSocket, (struct sockaddr *) &dirServer, sizeof(dirServer)) < 0){
    perror("Error al conectar al servidor");
    close(descSocket);
    exit(1);
  }

  printf("Conexion con el server %s establecida!\n", argv[1]);

  /* Ahora leemos por primera vez */
  bytes = read(0, buffer, MAXBUFF);
  if(bytes < 0){
    perror("Error al leer por teclado");
    close(descSocket);
    exit(1);
  }
  buffer[bytes - 1] = '\0';
  longitud = strlen(buffer);
  longitudBE = htonl(longitud);

  while(strcmp(buffer, "fin") != 0){
    /* Enviamos primero la longitud */
    bytes = write(descSocket, &longitudBE,  sizeof(longitudBE));
    if(bytes != sizeof(longitudBE)){
      perror("Error al enviar longitud");
      close(descSocket);
      exit(1);
    }

    /* Ahora enviamos la cadena */
    bytes = write(descSocket, buffer, longitud);
    if(bytes != longitud){
      perror("Error al enviar cadena");
      close(descSocket);
      exit(1);
    }

    /* Ahora leemos la longitud de ECO */
    bytes = read(descSocket, &longitudBE, sizeof(longitudBE));
    if(bytes != sizeof(longitudBE)){
      perror("Error al leer longitud");
      close(descSocket);
      exit(1);
    }
    longitud = ntohl(longitudBE);

    /* Ahora leemos la cadena */
    bytes = read(descSocket, buffer, longitud);
    if(bytes != longitud){
      perror("Error al leer cadena");
      close(descSocket);
      exit(1);
    }

    /* Ahora volvemos a leer de teclado */
    bytes = read(0, buffer, MAXBUFF);
    if(bytes < 0){
      perror("Error al leer por teclado");
      close(descSocket);
      exit(1);
    }
    buffer[bytes - 1] = '\0';
    longitud = strlen(buffer);
    longitudBE = htonl(longitud);
  }
  close(descSocket);
  return 0;
}
