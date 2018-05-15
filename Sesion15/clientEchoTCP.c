#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define PUERTO 4950
#define MAXBUFFER 512

ssize_t read_n(int fd, void * mensaje, size_t longitud_mensaje);
ssize_t write_n(int fd, void * mensaje, size_t longitud_mensaje);

/**
 * Cliente TCP que envía peticiones ECHO a un servidor cuya IP se introduce
 * como argumento del main y que escucha en el puerto 4950. 
 * Para realizar dichas peticiones, el cliente 
 * solicita al usuario cadenas por teclado y envía una a una hasta que éste
 * introduce la cadena "fin". La cadena "fin" no se envía al servidor.
 * Entonces se desconecta y finaliza la conexión.
 */
int main(int argc, char * argv[]) {
	//1.- Procesamos los argumentos
	//2.- Declaración de variables
	//3.- Montamos la dirección del servidor
	//4.- Creamos el socket
	//5.- Abrimos la conexión con el servidor
	//6.- Iteramos mientras que el cliente no introduzca "fin"
	//7.- Cerramos
	
	//1.- Procesamos los argumentos
	if (argc < 2) {
		printf("Uso: %s <IP>\n",argv[0]);
		exit(1);
	}
	
	//2.- Declaración de variables
	//descriptor de socket
	int sd;
	//buffer
	char buffer[MAXBUFFER];
	//dirección del servidor
	struct sockaddr_in serv_addr;

	
	//3.- Montamos la dirección del servidor
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port   = htons(PUERTO);
	//asignar la IP que viene como cadena de texto a una dirección IP de 
	//32 bits en big endian
	uint32_t ip_servidor = ?;
	
	
	//4.- Creamos el socket
	if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror("socket");
		exit(1);
	}	
	
	//5.- Abrimos la conexión con el servidor
	if (connect(?, ?, ?) < 0 ) {
		perror("connect");
		exit(1);
	}
	
	//6.- Aquí toca pensar...
	
	//7.- Cerramos
	if (close(sd) < 0) {
		perror("close");
		exit(1);
	}
	return 0;
}

/**
 * Funciones auxiliares
 */
ssize_t read_n(int fd, char * mensaje, size_t longitud_mensaje) {
  ssize_t a_leer = longitud_mensaje;
  ssize_t total_leido = 0;
  ssize_t leido;
  
  do {
    errno = 0;
    leido = read(fd, mensaje + total_leido, a_leer);
    if (leido >= 0) {
      total_leido += leido;
      a_leer -= leido;
    }
  } while((
      (leido > 0) && (total_leido < longitud_mensaje)) ||
      (errno == EINTR));
  
  if (total_leido > 0) {
    return total_leido;
  } else {
    /* Para permitir que devuelva un posible error en la llamada a read() */
    return leido;
  }
}

ssize_t write_n(int fd, char * mensaje, size_t longitud_mensaje) {
  ssize_t a_escribir = longitud_mensaje;
  ssize_t total_escrito = 0;
  ssize_t escrito;
  
  do {
    errno = 0;
    escrito = write(fd, mensaje + total_escrito, a_escribir);
    if (escrito >= 0) {
      total_escrito += escrito ;
      a_escribir -= escrito ;
    }
  } while(
        ((escrito > 0) && (total_escrito < longitud_mensaje)) ||
        (errno == EINTR));
  
  if (total_escrito > 0) {
    return total_escrito;
  } else {
    /* Para permitir que devuelva un posible error de la llamada a write() */
    return escrito;
  }
}
