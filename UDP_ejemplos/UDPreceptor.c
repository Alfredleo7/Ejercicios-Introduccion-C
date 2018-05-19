#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PUERTO 4950 /* Puerto donde escucha el receptor */

int main(int argc, char *argv[]){

  /* Declaramos las variables que vamos a necesitar */
  int descriptorSocket;
  struct sockaddr_in direccionReceptor;
  int numBytes; /* Numero de bytes leidos */

  if(argc != 3){
    printf("Numero de argumentos invalido \n");
    exit(-1);
  }

  /* Obtenemos la IP del receptor de argv y la pasamos al
     formato necesario */
  uint32_t ip_receptor;
  if((ip_receptor = inet_addr(argv[1])) < 0){
    perror("inet_addr()");
    exit(-1);
  }

  /* Creamos el Socket UDP */
  if((descriptorSocket = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
    perror("Socket");
    exit(-1);
  }

  /* Inicializamos la direccion del receptor y le asignamos valores */
  memset(&direccionReceptor, 0, sizeof(direccionReceptor));
  direccionReceptor.sin_family = AF_INET;
  direccionReceptor.sin_port = htons(PUERTO);
  memcpy(&direccionReceptor.sin_addr, &ip_receptor, 4);

  /* Enviamos con sendto() */
  if((numBytes = sendto(descriptorSocket, argv[2], strlen(argv[2]), 0,
                        (struct sockaddr *) &direccionReceptor,
                        sizeof(direccionReceptor))) < 0){
    /* argv[2] es el mensaje a enviar */
    perror("Error al enviar con sendto()");
    exit(-1);
  }

  /* Ya tenemos los datos, por lo que vamos a imprimirlos */
  printf("Enviados %d bytes a la direccion %s\n", numBytes,
         inet_ntoa(direccionReceptor.sin_addr));
  /* Cerramos el Socket */
  close(descriptorSocket);
  return 0;
}
