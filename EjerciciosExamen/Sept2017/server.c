#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define PUERTO 4950
#define MAXBUFF 512

void codificarCadena(char *cadena, int offset);

int main(int argc, char *argv[]){
  /* Declaramos las variables a utilizar */
  struct sockaddr_in dirServer, dirClient;
  int descSocket, nuevoDescSocket, bytes;
  socklen_t longDirCliente;

  /* Inicializamos la direccion del servidor */
  memset(&dirServer, 0, sizeof(dirServer));

  dirServer.sin_family = AF_INET;
  dirServer.sin_addr.s_addr = INADDR_ANY; /* Acepta conexiones de cualquier IP */
  dirServer.sin_port = htons(PUERTO);

  longDirCliente = sizeof(dirClient);

  /* Creamos el socket */
  descSocket = socket(AF_INET, SOCK_STREAM, 0);
  if(descSocket < 0){
    perror("No se ha podido abrir el socket");
    exit(-1);
  }

  /* Vinculamos el proceso con el puerto de escucha indicado en la variable
     dirServer */
  if(bind(descSocket, (struct sockaddr *) &dirServer, sizeof(dirServer)) < 0){
    perror("Error al vincular el proceso con el puerto");
    exit(-1);
  }

  if(listen(descSocket, 5) < 0){ /* Activamos el modo pasivo con una cola maxima
                                    de 5 */
    perror("Error al entrar en modo pasivo");
    exit(-1);
  }

  /* Bucle principal */
  while(1){
    printf("Servidor esperando conexion...\n");

    /* Esperamos una conexion de un cliente. Dicha conexion nos es devuelta
       en nuevoDescSocket */
    nuevoDescSocket = accept(descSocket, (struct sockaddr *) &dirClient,
                             &longDirCliente);
    printf("Nueva conexion aceptada!\n");
    while(1){
      if(nuevoDescSocket < 0){
        perror("Accept");
        close(descSocket);
        exit(-1);
      }
      /* Ya se ha aceptado la conexion del cliente */

      /* Declaramos las variables a utilizar */
      int longitud, leidos;
      uint16_t  longitudBE;
      char buffer[MAXBUFF];

      /* Recibimos la longitud de la cadena primero */
      bytes = recv(nuevoDescSocket, &longitudBE, sizeof(longitudBE), 0);
      if(bytes < 0){
        perror("Error al recibir la longitud");
        close(nuevoDescSocket);
        close(descSocket);
        exit(-1);
      }
      if(bytes == 0){
        printf("Cliente desconectado. Recibir longitud.\n");
        close(nuevoDescSocket);
        break;
      } else if (bytes != sizeof(longitudBE)){
        perror("Error al recibir la longitud");
        close(nuevoDescSocket);
        close(descSocket);
        exit(-1);
      }
      longitud = ntohs(longitudBE);

      printf("Longitud recibida: %d\n", longitud);
      /* Ahora recibimos la cadena */
      bytes = recv(nuevoDescSocket, buffer, longitud, 0);
      if((bytes != longitud)){
        perror("Error al recibir la cadena");
        close(nuevoDescSocket);
        close(descSocket);
        exit(-1);
      }
      if(bytes == 0){
        printf("Cliente desconectado. Recibir cadena.\n");
        close(nuevoDescSocket);
        break;
      } else if (bytes != longitud){
        perror("Error al recibir la cadena");
        close(nuevoDescSocket);
        close(descSocket);
        exit(-1);
      }
      buffer[longitud] = '\0';
      printf("Cadena recibida: %s\n", buffer);
      /* La codificamos */
      codificarCadena(buffer, atoi(argv[1]));
      /* Anyadimos el sufijo  y ajustamos la longitud */
      strcat(buffer, "123");
      longitud += 3;
      printf("Longitud enviada: %d\n", longitud);

      /* Enviamos la nueva longitud */
      longitudBE = htons(longitud);

      if((bytes = write(nuevoDescSocket, &longitudBE, sizeof(longitudBE))) < 0){
        perror("Error al enviar longitud");
        close(nuevoDescSocket);
        close(descSocket);
        exit(-1);
      } else if( bytes == 0){
        printf("Cliente desconectado\n");
        close(nuevoDescSocket);
        break;
      }else if (bytes != sizeof(longitudBE)){
        perror("Error al enviar longitud");
        close(nuevoDescSocket);
        close(descSocket);
        exit(-1);
      }
      /* Ahora enviamos la cadena codificada */
      if((bytes = write(nuevoDescSocket, buffer, longitud)) < 0){
        perror("Error al enviar cadena");
        close(nuevoDescSocket);
        close(descSocket);
        exit(-1);
      }else if( bytes == 0){
        printf("Cliente desconectado\n");
        close(nuevoDescSocket);
        break;
      }else if (bytes != longitud){
        perror("Error al enviar cadena");
        close(nuevoDescSocket);
        close(descSocket);
        exit(-1);
    }
      printf("Cadena enviada: %s\n", buffer);
    }/* Fin de while(1) */
  }/* Fin de while(1) */
}/* Fin de main() */

void codificarCadena(char *cadena, int offset){
  for(int i = 0; i < strlen(cadena); i++){
    cadena[i] += offset;
  }
}
