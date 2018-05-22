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

int main(){
  /* Declaramos variables */
  int descSocket;
  struct sockaddr_in dirCliente;
  struct sockaddr_in dirServer;
  socklen_t longitudDireccion;
  int numBytes; /* Numero de bytes leidos */
  char buffer[TBUFFER]; /* Buffer para recibir datos */

  /* Creamos el socket */
  if((descSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    perror("Error al abrir el socket");
    exit(-1);
  }
  /* Asignamos valores a la direccion */
  memset(&dirServer, 0, sizeof(dirServer)); /* Lo inicializamos */
  dirServer.sin_family = AF_INET;
  dirServer.sin_port = htons(PUERTO);
  dirServer.sin_addr.s_addr = INADDR_ANY; /* Lo ponemos a INADDR_ANY,
                                             ya que escuchamos a todas
                                            las IPs */
  /* Hacemos el bind par vincular el socket al puerto */
  if(bind(descSocket,(struct sockaddr*) &dirServer, sizeof(dirServer)) < 0){
    perror("Error en bind");
    exit(-1);
  }
  /* Calculamos la longitud de la direccion del cliente */
  longitudDireccion = sizeof(dirCliente);


  /* Comenzamos a leer */
  printf("Esperando mensajes...\n");
  /* Leemos por primera vez */
  if((numBytes = recvfrom(descSocket, buffer, TBUFFER, 0,
                          (struct sockaddr *) &dirCliente,
                          (socklen_t *) &longitudDireccion))< 0){
   perror("Error en recvfrom");
    exit(-1);
  }
  /* Ponemos el \0 */
  buffer[numBytes] = '\0';
  
  while((strcmp(buffer, "fin")) != 0){
    printf("He recibido algo: \n");
    printf("El paquete tiene un tamanyo: %zu\n", strlen(buffer));
    printf("Su contenido es: %s\n", buffer);

    if((numBytes = recvfrom(descSocket, buffer, TBUFFER, 0,
                            (struct sockaddr *) &dirCliente,
                            (socklen_t *) &longitudDireccion))< 0){
      perror("Error en recvfrom");
      exit(-1);
    }
    buffer[numBytes] = '\0';
  }
  close(descSocket);
  return 0;
}
