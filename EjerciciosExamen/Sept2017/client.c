#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define PUERTO 4950
#define MAXBUFF 512

int main(int argc, char *argv[]){
  /* Variables */
  struct sockaddr_in dirServer;
  int descSocket;
  char buffer[MAXBUFF];
  int longitud, bytes, leidos;
  uint16_t longitudBE;

  /* Inicializamos la direccion  y rellenamos sus campos*/
  memset(&dirServer, 0, sizeof(dirServer));

  dirServer.sin_family = AF_INET;
  dirServer.sin_port = htons(PUERTO);
  dirServer.sin_addr.s_addr = inet_addr(argv[1]);

  /* Creamos el socket */
  if((descSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    perror("Error al abrir el socket\n");
    exit(-1);
  }
  printf("Intentando conectar con el servidor...");

  /* Nos conectamos al server */
  if(connect(descSocket, (struct sockaddr *) &dirServer,
             sizeof(dirServer)) < 0){
    perror("Error al conectar con el servidor");
    exit(-1);
  }
  printf("Conexion con el servidor establecida!\n");
  printf("-------------------------------------\n");

  /* Leemos la cadena por primera vez */
  leidos = read(0, buffer, MAXBUFF);
  buffer[leidos - 1] = '\0';
  longitud = strlen(buffer);
  longitudBE = htons(longitud);

  while(strcmp(buffer, "fin") != 0){
    /* Enviamos primero la longitud */
    bytes = write(descSocket, &longitudBE, sizeof(longitudBE));
    if(bytes < 0){
      perror("Error al enviar longitud");
      close(descSocket);
      exit(-1);
    } else if(bytes == 0) {
      perror("Servidor desconectado.");
      close(descSocket);
      exit(-1);
    } else if (bytes != sizeof(longitudBE)){
      perror("Error al enviar longitud");
      close(descSocket);
      exit(-1);
    }
    printf("He enviado la longitud: %d\n", longitud);
    printf("He enviado %d bytes\n", bytes);
    /* Ahora enviamos la cadena */
    bytes = send(descSocket, buffer, longitud, 0);
    if(bytes < 0){
      perror("Error al enviar cadena");
      close(descSocket);
      exit(-1);
    } else if(bytes == 0) {
      perror("Servidor desconectado.");
      close(descSocket);
      exit(-1);
    } else if (bytes != longitud){
      perror("Error al enviar cadena");
      close(descSocket);
      exit(-1);
    }
    printf("He enviado la cadena: %s\n", buffer);
    printf("He enviado %d bytes\n", bytes);

    /* Ahora recibimos la cadena codificada de vuelta */
    /* Primero la nueva longitud */
    bytes = recv(descSocket, &longitudBE, sizeof(longitudBE), 0);
    if(bytes < 0){
      perror("Error al recibir longitud codificada");
      close(descSocket);
      exit(-1);
    } else if(bytes == 0) {
      perror("Servidor desconectado.");
      close(descSocket);
      exit(-1);
    } else if (bytes != sizeof(longitudBE)){
      perror("Error al recibir longitud codificada");
      close(descSocket);
      exit(-1);
    }

    longitud = ntohs(longitudBE);
    printf("Longitud recibida: %d\n", longitud);

    /* Ahora la cadena */
    bytes = recv(descSocket, buffer, longitud, 0);
    if(bytes < 0){
      perror("Error al recibir cadena codificada");
      close(descSocket);
      exit(-1);
    } else if(bytes == 0) {
      perror("Servidor desconectado.");
      close(descSocket);
      exit(-1);
    } else if (bytes != longitud){
      perror("Error al recibir cadena codificada");
      close(descSocket);
      exit(-1);
    }
    buffer[longitud] = '\0';
    printf("Cadena recibida: %s\n", buffer);
    printf("Introduzca nueva cadena:\n");
    /* Volvemos a leer */
    leidos = read(0, buffer, MAXBUFF);
    buffer[leidos - 1] = '\0';
    longitud = strlen(buffer);
    longitudBE = htons(longitud);
  }/* Fin de while(strcmp(buffer, "fin") */
  close(descSocket);
  return 0;
}/* Fin de main() */
