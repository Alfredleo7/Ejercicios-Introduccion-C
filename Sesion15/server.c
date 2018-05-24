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

int main(){
  /* Declaramos las variables */
  struct sockaddr_in dirServer, dirCliente;
  int descSocket, nuevoDescSocket;
  socklen_t longitudDirCliente;
  char buffer[MAXBUFF];
  /* Asignamos valores */
  memset(&dirServer, 0, sizeof(dirServer));
  dirServer.sin_port = htons(PORT);
  dirServer.sin_addr.s_addr = INADDR_ANY;
  dirServer.sin_family = AF_INET;

  /* Creamos el socket */
  if((descSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    perror("No se ha podido abrir el socket");
    exit(-1);
  }

  /* Hacemos bind para vincular el puerto */
  if(bind(descSocket, (struct sockaddr *) &dirServer,
          sizeof(dirServer)) < 0 ){
    perror("Error al hacer bind!");
    close(descSocket);
    exit(-1);
  }

  /* Ponemos el socket en modo pasivo */
  if(listen(descSocket, 10) < 0){
    perror("listen");
    close(descSocket);
    exit(-1);
  }
  /* ITERAMOS */
  while(1){
    printf("Esperando conexion...\n");
    /* Hacemos accept para obtener el nuevo descriptor que nos
       comunique con el cliente */
    if((nuevoDescSocket = accept(descSocket, (struct sockaddr *) &dirCliente,
                                 &longitudDirCliente)) < 0){
      perror("listen");
      close(descSocket);
      exit(-1);
    }
    /* Hemos aceptado ya la nueva conexion. Nos comunicaremos a traves de
       nuevoDescSocket */

    /* Leemos por primera vez la longitud */
    uint32_t longitudBE;
    int longitud, leidos;

    if((leidos = recv(nuevoDescSocket, &longitudBE, sizeof(longitudBE), 0)) < 0){
      perror("Recv");
      exit(-1);
    }
    while(leidos > 0 && strcmp(buffer,"fin") != 0){
      /* Pasamos la longitud a formato local */
      printf("Longitud recibida: %d\n", leidos);
      longitud = ntohl(longitudBE);
      if((leidos = recv(nuevoDescSocket, buffer, longitud, 0)) != longitud){
        perror("Error al recibir mensaje");
        close(nuevoDescSocket);
        close(descSocket);
        exit(-1);
      }
      buffer[leidos] = '\0';
      printf("Mensaje recibido:\n %s\n", buffer);
      /* Procedemos a enviar de vuelta la longitud */
      if((send(nuevoDescSocket, &longitudBE, sizeof(longitudBE), 0)) < 0){
        perror("Error al enviar la longitud");
        close(nuevoDescSocket);
        close(descSocket);
        exit(-1);
      }
      /* Y enviamos de vuelta el mensaje */
      if((send(nuevoDescSocket, buffer, longitud, 0)) != longitud){
        perror("Error al enviar mensaje");
        close(nuevoDescSocket);
        close(descSocket);
        exit(-1);
      }
      /* Por ultimo volvermos a leer */
      if((leidos = recv(nuevoDescSocket, &longitudBE, sizeof(longitudBE), 0)) < 0){
        perror("Error al recibir mensaje");
        close(nuevoDescSocket);
        close(descSocket);
        exit(-1);
      }
    }
    close(nuevoDescSocket);
  }
  close(descSocket);
  return 0;
}
