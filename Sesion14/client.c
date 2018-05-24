/*
  Implemente un servidor UDP que escuche en el puerto 4950 y por cualquier interfaz de red con IP asignada en la máquina (INADDR_ANY). El servidor atenderá peticiones de clientes (una a una), e imprimirá por pantalla tanto la cadena recibida, como la IP del cliente que la ha enviado. El servicio procesará cadenas de clientes hasta que uno de ellos envíe la cadena "stop". En ese caso, cierra el socket y termina.

  Implemente también el cliente UDP correspondiente, cuyo funcionamiento es más simple. Éste toma como argumento la dirección IP del servidor UDP, una cadena de texto, y se la envía, mostrando la cantidad de bytes enviados.

  Dada la complejidad que supone desarrollar pruebas automáticas para este tipo de programas, hay disponible en el campus virtual un ejecutable del cliente y otro del servidor con el que podréis hacer pruebas en local correspondiente. Siette sólo compilara vuestros códigos y los dejará almacenados para su inspección posterior.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PUERTO 4950
#define TBUFFER 512

int main(int argc, char *argv[]){
  /* Comprobamos argumentos */
  if(argc < 3){
    printf("Error! Uso: %s <IP> <Mensaje>\n", argv[0]);
    exit(-1);
  }
  /* Declaramos las variables */
  int descSocket;
  struct sockaddr_in dirServer;
  int numBytes;

  /* Pasamos la IP a entero de 32 bytes */
  uint32_t ipServer;
  if((ipServer = inet_addr(argv[1])) < 0){
    perror("Error al pasar la IP a int 32 bytes");
    exit(-1);
  }

  /* Creamos el socket para UDP */
  if((descSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    perror("Error al abrir el socket");
    exit(-1);
  }

  /* Inicializamos la direccion del servidor y le asignamos
     los valores */
  memset(&dirServer, 0, sizeof(dirServer));
  dirServer.sin_family = AF_INET;
  dirServer.sin_port = htons(PUERTO);
  dirServer.sin_addr.s_addr = ipServer;

  /* Enviamos el mensaje con sendto() */
  if((numBytes = sendto(descSocket, argv[2], strlen(argv[2]), 0,
                        (struct sockaddr*) &dirServer,
                        sizeof(dirServer))) < 0){
    perror("Error al enviar datos");
    exit(-1);
  }
  printf("Enviados %d bytes a la direccion %s\nMensaje: %s\n", numBytes,
         argv[1], argv[2]);
  close(descSocket);
  return 0;
}
