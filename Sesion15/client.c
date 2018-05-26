#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define PORT 4950
#define MAXBUFF 512

int main(int argc, char *argv[]){
  /* Comprobamos argumentos */
  if(argc != 3){
    printf("Error! Uso: %s <IP> <MENSAJE>\n", argv[0]);
    exit(-1);
  }

  /* Declaramos las variables */
  struct sockaddr_in dirServer;
  int descSocket;

  /* Inicializamos la direccion y rellenamos sus campos */
  memset(&dirServer, 0, sizeof(dirServer));

  dirServer.sin_addr.s_addr = inet_addr(argv[1]);
  dirServer.sin_port = htons(PORT);
  dirServer.sin_family = AF_INET;

  /* Abrimos el socket */
  if((descSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    perror("Error al abrir el socket!");
    exit(-1);
  }
  printf("Estableciendo conexion con el servidor...\n");

  /* Nos conectamos al servidor */
  if(connect(descSocket, (struct sockaddr *) &dirServer,
             sizeof(dirServer) < 0)){
    perror("Error al conectar al servidor");
    close(descSocket);
    exit(-1);
  }
  printf("Conexion con el servidor establecida!\n");

  /* Declaramos variables para poder enviar los datos */;
  int longitud;
  uint32_t longitudBE;
  longitud = strlen(argv[2]);
  longitudBE = htonl(longitud);
  char buffer[MAXBUFF];
  /* Mandamos primero la longitud */
  printf("DATOS A ENVIAR:\nCadena: %s \n Longitud: %d\n", argv[2], longitud);
  if((send(descSocket, &longitudBE, sizeof(longitudBE), 0)) != sizeof(longitudBE)){
    perror("Error al enviar longitud");
    close(descSocket);
    exit(-1);
  }
  /* Ahora mandamos la cadena */
  if((send(descSocket, argv[2], longitud, 0)) != longitud){
    perror("Error al enviar cadena\n");
    close(descSocket);
    exit(-1);
  }
  printf("Datos correctamente enviados\n");

  /* Ahora procedemos a leer el ECHO */
  if((recv(descSocket, &longitudBE, sizeof(longitudBE), 0)) != sizeof(longitudBE)){
    perror("Error al recibir longitud\n");
    close(descSocket);
    exit(-1);
  }
  longitud = ntohl(longitudBE);

  /* Ahora leemos la cadena */ 
  if((recv(descSocket, buffer, longitud, 0)) != longitud){
    perror("Error al recibir cadena\n");
    close(descSocket);
    exit(-1);
  }
  printf("ECHO:\nCadena: %s\nLongitud: %d", buffer, longitud);

  /* Cerramos el socket y terminamos */
  if(close(descSocket) < 0){
    perror("Error al cerra el socket!");
    exit(-1);
  }
  return 0;
}

