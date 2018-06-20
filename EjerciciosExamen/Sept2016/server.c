/* Servidor ECHO TCP */
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

void copia_fdset(fd_set *origen, fd_set *destino, int maxfdmasuno);
int maximoxy(int x, int y);

int main(){
  /* Declaramos las variables */
  int descSocket;
  char buffer[MAXBUFF];
  int longitud, leidos;
  uint32_t longitudBE;
  struct sockaddr_in dirServer, dirClient;
  socklen_t addrLen = sizeof(dirClient);
  /* Creamos la direccion del servidor */
  memset(&dirServer, 0, sizeof(dirServer));

  dirServer.sin_family = AF_INET;
  dirServer.sin_port = htons(PUERTO);
  dirServer.sin_addr.s_addr = INADDR_ANY;

  /* Creamos el socket */
  if((descSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    perror("Error al abrir el socket!");
    exit(1);
  }

  /* Asociamos el socket al puerto */
  if(bind(descSocket, (struct sockaddr *) &dirServer, sizeof(dirServer)) < 0){
    perror("Error en bind");
    close(descSocket);
    exit(1);
  }

  /* Ponemos el socket en modo pasivo con una cola maxima de 5 */
  if(listen(descSocket, 5) < 0){
    perror("Error al entrar en modo pasivo");
    close(descSocket);
    exit(1);
  }

  /* Declaramos e inicializamos las variables para select */
  fd_set cjtoDesc, cjtoMod;
  int maxDesc;
  FD_ZERO(&cjtoDesc);
  FD_SET(descSocket, &cjtoDesc);
  maxDesc = descSocket;

  /* Empezamos el bucle principal */
  while(1){
    printf("Servidor esperando conexion...\n");

    /* Hacemos una copia del cjto de descriptores */
    copia_fdset(&cjtoDesc, &cjtoMod, maxDesc + 1);

    /* Ahora comprobamos si alguno de los descriptores esta listo */
    if(select(maxDesc+1, &cjtoMod, NULL, NULL, NULL) < -1){
      perror("Select");
      exit(1);
    }

    /* Comprobamos que descriptor esta listo para leer */
    for(int desc = 0; desc < maxDesc + 1; desc++){

      if(FD_ISSET(desc, &cjtoMod)){ /* El descriptor esta listo para leer */

        /* Ahora debemos comprobar si se trata del descriptor del socket (pasivo)
           para realizar el accept */
        if( descSocket == desc){

          /* Se trata del descriptor del socket */
          int nuevoDesc = accept(descSocket, (struct sockaddr *)&dirClient,
                                 &addrLen);

          /* Ahora la nueva conexion esta en nuevoDesc, por lo que hay que
             anyadirlo al conjunto de descriptores */
          if(nuevoDesc < 0){
            perror("Error al aceptar la nueva conexion!");
              exit(1);
          }

          FD_SET(nuevoDesc, &cjtoDesc);
          maxDesc = maximoxy(nuevoDesc, maxDesc);

          printf("Nuevo cliente conectado desde %s en el socket %d\n",
                 inet_ntoa(dirClient.sin_addr), nuevoDesc);

        } else { /* No se trata de descSocket */
          /* Ahora debemos trabajar con desc, que es el socket activo */
          /* Leemos primero la longitud */
          leidos = read(desc, &longitudBE, sizeof(longitudBE));

          while(leidos > 0){
            if(leidos != sizeof(longitudBE)){
              perror("Read longitudBE size");
              close(descSocket);
              close(desc);
              exit(1);
            }

            /* Pasamos a formato local la longitud */
            longitud = ntohl(longitudBE);
            printf("Longitud recibida %d\n", longitud);

            /* Ahora leemos la cadena */
            if(read(desc, buffer, longitud) != longitud){
              perror("Read cadena");
              close(descSocket);
              close(desc);
              exit(1);
            }
            buffer[longitud] = '\0';
            printf("Buffer = %s\n", buffer);

            /* Enviamos la longitud y la cadena de vuelta */

            longitudBE = htonl(longitud);
            if(write(desc, &longitudBE, sizeof(longitudBE)) != sizeof(longitudBE)){
                perror("Write longitudBE");
                close(descSocket);
                close(desc);
                exit(1);
            }

            if(write(desc, buffer, longitud) != longitud){
              perror("Write cadena");
              close(descSocket);
              close(desc);
              exit(1);
            }
            /* Volvemos a leer la longitud */
            leidos = read(desc, &longitudBE, sizeof(longitudBE));
          }

          if(leidos < 0){
            /* Error en la lectura */
            perror("Read longitudBE 0");
            close(descSocket);
            close(desc);
            exit(1);
          } else { /* leidos == 0, por lo que se ha desconectado */
            close(desc);
          }
          FD_CLR(desc, &cjtoDesc);
        }
      }
    }
  } /* while() */
  /* FIN */
  close(descSocket);
  return 0;
}/* main() */

void copia_fdset(fd_set *origen, fd_set *destino, int maxfdmasuno){
  FD_ZERO(destino);
  for(int i = 0; i < maxfdmasuno; i++){
    if(FD_ISSET(i, origen)){
      FD_SET(i, destino);
    }
  }
}

int maximoxy(int x, int y){
  if(x >= y){
    return x;
  }
  return y;
}
