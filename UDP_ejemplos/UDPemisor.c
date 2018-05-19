#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PUERTO 4950 /* Puerto para el bind */
#define TBUFFER 100 /* Max tamanyo del buffer */

int main(){

  /* Declaramos las variables que vamos a necesitar */
  int descriptorSocket;
  struct sockaddr_in direccionReceptor;
  struct sockaddr_in direccionEmisor;
  socklen_t longitudDireccion;
  int numBytes; /* Numero de bytes leidos */
  char buffer[TBUFFER]; /* Buffer en el que recibir datos */

  /* Creacion del socket UDP */
  if((descriptorSocket = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
    perror("Error en la creacion del socket");
    exit(-1);
  }
  /* Asignamos valores a la direccion del receptor */
  memset(&direccionReceptor, 0, sizeof(direccionReceptor));
  direccionReceptor.sin_family = AF_INET;
  direccionReceptor.sin_port = htons(PUERTO);
  direccionReceptor.sin_addr.s_addr = INADDR_ANY; /* Escucha por cualquier
                                                    tarjeta con una IP asignada */
  /* Hacemos el bind */
  if(bind(descriptorSocket, (struct sockaddr *)direccionReceptor, sizeof(direccionReceptor)) < 0){
    perror("Error al hacer bind()");
    exit(-1);
  }
  /* Sacamos la longitud de la direccion del emisor y recibimos datos de este */
  longitudDireccion = sizeof(direccionEmisor);
  if((numBytes = recvfrom(descriptorSocket, buffer, TBUFFER, 0,
                               (struct sockaddr *)&direccionEmisor,
                               (socklen_t *) &longitudDireccion)) < 0){
    perror("Error al recibir datos.");
    exit(-1);
  }

  /* Ya hemos recibido los datos, por lo que vamos a imprimirlos por pantalla */
  printf("Paquete recibido de %s\n", inet_ntoa(direccionEmisor.sin_addr)); /* Usamos inet_ntoa() para
                                                                              pasar de uint32_t a cadena
                                                                              de chars */
  printf("El paquete tiene %d bytes de longitud\n", numBytes);
  /* Le ponemos el \0 al buffer */
  buffer[numBytes] = '\0';
  printf("El paquete contiene esta cadena: %s\n", buffer);

  /* Cerramos el Socket */
  close(descriptorSocket);
  return 0;
}
