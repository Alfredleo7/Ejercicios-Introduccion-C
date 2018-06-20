#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <signal.h>

#define PUERTOENVIO 4950
#define PUERTORECEP 2119
#define MAXBUFF 512

void manejador(int signo){
  wait(0);
  signal(SIGCHLD, manejador);
}
void procesarCadena(char *cadena){
  while(strstr(cadena, "taco") != NULL){
    char *pos = strstr(cadena, "taco");
    for(int i = 0; i < 4; i++){
      pos[i] = '*';
    }
    printf("La cadena contenia un taco\n");
  }
}

int main(int argc, char *argv[]){
  /* Declaramos variables a utilizar */
  int leidos, escritos, descServer, descSocket, longitud, nuevoDesc;
  struct sockaddr_in dirClient, dirProxy, dirServer;
  char buffer[MAXBUFF];
  uint32_t longitudBE;

  /* Nos aseguramos de manejar SIGCHLD */
  signal(SIGCHLD, manejador);

  /* Inicializamos las direcciones y rellenamos los campos */
  /* Primero la direccion propia del proxy */
  memset(&dirProxy, 0, sizeof(dirProxy));

  dirProxy.sin_port = htons(PUERTORECEP);
  dirProxy.sin_family = AF_INET;
  dirProxy.sin_addr.s_addr = INADDR_ANY;

  /* Ahora la direccion para conectarse al servidor */
  memset(&dirServer, 0, sizeof(dirServer));

  dirServer.sin_port = htons(PUERTOENVIO);
  dirServer.sin_family = AF_INET;
  dirServer.sin_addr.s_addr = inet_addr(argv[1]);

  /* Creamos los sockets necesarios */

  descSocket = socket(AF_INET, SOCK_STREAM, 0);
  descServer = socket(AF_INET, SOCK_STREAM, 0);

  if(descSocket < 0 || descServer < 0){
    perror("Socket");
    exit(1);
  }
  /* Vinculamos el proceso al puerto */
  if(bind(descSocket, (struct sockaddr *) &dirProxy, sizeof(dirProxy)) < 0){
    perror("Bind");
    close(descSocket);
    close(descServer);
    exit(1);
  }

  /* Ponemos el proxy en modo escucha para que se conecten clientes */
  if(listen(descSocket, 5) < 0){
    perror("Listen");
    close(descSocket);
    close(descServer);
    exit(1);
  }

  /* Y nos conectamos al servidor */
  if(connect(descServer, (struct sockaddr *) &dirServer, sizeof(dirServer)) < 0){
    perror("Connect");
    close(descSocket);
    close(descServer);
    exit(1);
  }

  printf("Conexion con el servidor establecida!\n");

  /* Bucle principal */
  while(1){
    socklen_t dirLon = sizeof(dirClient);
    /* Aceptamos una nueva conexion */
    if((nuevoDesc = accept(descSocket, (struct sockaddr *) &dirClient,
                           &dirLon)) < 0){
      perror("Accept");
      close(descSocket);
      close(descServer);
      exit(1);
    }
    /* Creamos un hijo */
    int pid = fork();

    if(pid == 0){ /* CODIGO DEL PADRE */
      /* Cerramos el descriptor, ya que no nos va a volver a hacer falta */
      if(close(nuevoDesc) < 0){
        perror("close");
        close(descSocket);
        close(descServer);
        exit(1);
      }
    } else if(pid > 0){/* CODIGO DEL HIJO */
      printf("Conectado con el cliente %s\n", inet_ntoa(dirClient.sin_addr));
      while(1){
        /* Leemos por primera vez del cliente */
        if((leidos = read(nuevoDesc, &longitudBE, sizeof(longitudBE))) < 0){
          perror("Read longitudBE");
          close(nuevoDesc);
          exit(1);
        } else if (leidos == 0){
          printf("Cliente desconectado...");
          break;
        }
        longitud = ntohl(longitudBE);
        printf("Recibida del cliente la longitud: %d ", longitud);
        /* Ahora leemos la cadena... */
        if((leidos = read(nuevoDesc, buffer, longitud)) < 0){
          perror("Read longitudBE");
          close(nuevoDesc);
          exit(1);
        } else if (leidos == 0){
          printf("Cliente desconectado...");
          break;
        }
        buffer[longitud] = '\0';
        printf("Cadena: %s\n", buffer);
        /* Comprobamos si contiene la palabra taco */
        procesarCadena(buffer);

        /* Enviamos al servidor la longitud */
        if((escritos = write(descServer, &longitudBE, sizeof(longitudBE))) < 0){
          perror("Write longitudBE");
          close(nuevoDesc);
          exit(1);
        }
        /* Enviamos al servidor la  cadena */
        if((escritos = write(descServer, buffer, longitud)) < 0){
          perror("Write cadena");
          close(nuevoDesc);
          exit(1);
        }

        /* Recibimos de vuelta la longitud del servidor */
        if((leidos = read(descServer, &longitudBE, sizeof(longitudBE))) < 0){
          perror("Read longitudBE");
          close(nuevoDesc);
          exit(1);
        }
        printf("Recibida del server la longitud: %d ", longitud);
        /* Recibimos de vuelta la cadena del servidor */
        if((leidos = read(descServer, buffer, longitud)) < 0){
          perror("Read cadena");
          close(nuevoDesc);
          exit(1);
        }
        buffer[longitud] = '\0';
        printf("Cadena: %s\n", buffer);

        /* Volvemos a enviar todo al cliente */
        /* Enviamos al cliente la longitud */
        if((escritos = write(nuevoDesc, &longitudBE, sizeof(longitudBE))) < 0){
          perror("Write longitudBE");
          close(nuevoDesc);
          exit(1);
        }
        /* Enviamos al servidor la  cadena */
        if((escritos = write(nuevoDesc, buffer, longitud)) < 0){
          perror("Write cadena");
          close(nuevoDesc);
          exit(1);
        }
        buffer[longitud] = '\0';

        printf("-------------------------------\n");
      }/* FIN DEL HIJO */
    } else{ /* ERROR */
      perror("Error fork");
      close(descSocket);
      close(nuevoDesc);
      exit(1);
    }
  }/* while(1) */
  close(descSocket);
  return 0;
}/* main() */
