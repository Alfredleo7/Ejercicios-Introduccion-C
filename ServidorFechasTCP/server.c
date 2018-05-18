/****************************************************************/
/* FICHERO:     servidor.c                                      */
/* DESCRIPCION: codigo de un servidor de fechas                  */
/****************************************************************/
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "defs.h"

int main(){
  /* Declaramos las direcciones de los servidores y los sockets */
  struct sockaddr_in direccionServidor, direccionCliente;
  int descSocket, nuevoDescSocket;
  socklen_t longDirCliente; /* longitud de la direccionCliente*/
  int codigoOperacion;

  /* Creacion del socket para TCP*/
  descSocket = socket(PF_INET, SOCK_STREAM, 0);
  if(descSocket < 0){
    perror("No se ha podido abrir el socket");
    exit(-1);
  }
  /* Se pone a cero la direccion del server */
  memset(&direccionServidor, 0, sizeof(direccionServidor));
  direccionServidor.sin_family = AF_INET;
  direccionServidor.sin_addr.s_addr = INADDR_ANY;
  direccionServidor.sin_port = htons(atoi(puerto_tcp));
  /* Vincula el proceso con el puerto de escucha indicado en la variable
     direccionServidor*/
  if(bind(descSocket, (*struct sockaddr *) &direccionServidor,
          sizeof(direccionServidor)) < 0){
    perror("Error al vincular la direccion local");
    exit(1);
  }
  /* Pone el socket en modo pasivo para que pueda aceptar conexiones */
  listen(descSocket, 5); /* Cinco clientes pueden esperar su turno en cola */
  while(1){
    printf("Servidor esperando conexion");
    /* Espera una conexion del cliente, devuelve dicha conexion en nuevoDescSocket */
    longDirCliente = sizeof(direccionCliente); /* Longitud de la direccionCliente */
    nuevoDescSocket = accept(descSocket, (struct sockaddr *) &direccionCliente,
                             &longDirCliente);
    /* Ya se ha aceptado la conexion del cliente. Dicha conexion esta en nuevoDescSocket */
  }
}
