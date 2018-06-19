#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>

#define PUERTO 4950
#define MAXBUFF 512

void manejador(int signo){
  wait(0);
  signal(SIGCHLD, manejador);
}

int main(){
  /* Declaramos las variables a utilizar */
  int descSocket, nuevoDesc;
  char buffer[MAXBUFF];
  int longitud, leidos, escritos, pid;
  uint32_t longitudBE;
  struct sockaddr_in dirServer, dirClient;
  socklen_t addrLen = sizeof(dirClient);
  /* Manejamos hijos */
  signal(SIGCHLD, manejador);
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

  /* Asociamos el proceso con el puerto */
  if(bind(descSocket, (struct sockaddr *) &dirServer, sizeof(dirServer)) < 0){
    perror("Error en bind");
    close(descSocket);
    exit(1);
  }
  /* Ponemos el servidor en modo escucha */
  if(listen(descSocket, 5) < 0){
    perror("Error en listen");
    close(descSocket);
    exit(1);
  }
  printf("Servidor listo para aceptar conexiones...\n");
  /* Bucle principal */
  while(1){
    /* Aceptamos una nueva conexion */
    nuevoDesc = accept(descSocket, (struct sockaddr *) &dirClient, &addrLen);
    if(nuevoDesc < 0){
      perror("Error al aceptar conexion del cliente");
      close(descSocket);
      exit(1);
    }
    /* Ahora tenemos la nueva conexion en nuevoDes. Debemos crear un hijo para    *
     * manejarla y tambien debemos cerrarla en el padre, ya que no la vamos a usar*
     * y queremos aceptar otras conexiones                                        */

    /* Creamos el hijo */
    pid = fork();

    /* Manejamos padre e hijo */

    if(pid == 0){ /* CODIGO DEL PADRE */
      close(nuevoDesc);
    } else if(pid > 0){ /* CODIGO DEL HIJO */
      printf("Conectado con el cliente %s\n", inet_ntoa(dirClient.sin_addr));
      while(1){
        /* Primero leemos longitud */
        leidos = recv(nuevoDesc, &longitudBE, sizeof(longitudBE),0);
        if(leidos < 0){
          perror("Error al recibir longitud");
          close(nuevoDesc);
          exit(1);
        } else if (leidos == 0){
          /* Cliente desconectado */
          printf("Cliente %s desconectado\n", inet_ntoa(dirClient.sin_addr));
          close(nuevoDesc);
          exit(0);
        }
        longitud = ntohl(longitudBE);
        printf("Recibida la longitud: %d ", longitud);

        /* Ahora recibimos la cadena */
        leidos = recv(nuevoDesc, buffer, longitud,0);

        if(leidos < 0){
          perror("Error al recibir cadena");
          close(nuevoDesc);
          exit(1);
        } else if (leidos == 0){
          /* Cliente desconectado */
          printf("Cliente %d desconectado\n", nuevoDesc);
          close(nuevoDesc);
          exit(0);
        }

        buffer[longitud] = '\0';
        printf("Cadena: %s\n", buffer);

        /* Mandamos la longitud de vuelta */
        escritos = write(nuevoDesc, &longitudBE, sizeof(longitudBE));

        if(escritos < 0){
          perror("Error al escribir longitud");
          close(nuevoDesc);
          exit(1);
        }
        printf("Enviada la longitud: %d ", longitud);

        /* Mandamos la cadena de vuelta */
        escritos = write(nuevoDesc, buffer, longitud);

        if(escritos < 0){
          perror("Error al escribir longitud");
          close(nuevoDesc);
          exit(1);
        }
        printf("Cadena: %s\n", buffer);
      }/* while(1) */
    } /* FIN DEL HIJO */
  }
  close(descSocket);
  return 0;
}
