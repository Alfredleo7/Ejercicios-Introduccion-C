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
 * Servidor TCP que atiende peticiones ECHO de clientes, uno a uno, es decir,
 * recibe cadenas de texto que son enviadas de vuelta al cliente. El intercambio
 * de cadenas entre ambos se realiza enviando primero la longitud y después la
 * cadena en sí. El servidor atiende al cliente hasta que este se desconecta,
 * y después atiende a otro cliente.
 */
int main(int argc, char * argv[]) {
	//1.- Declaramos variables
	//2.- Creamos la dirección del servidor
	//3.- Creamos el socket
	//4.- Asociamos el socket al puerto:
	//5.- Ponemos el socket en escucha (modo pasivo)
	//6.- Iteramos
	//		6.1.- Aceptamos una conexión: accept()
	//      6.2.- Atender al cliente
	//7.- Close


	//1.- Declaramos variables
	int sd, n_sd; 			//descriptores de socket
	char buffer[MAXBUFFER];	//buffer para recepción/envío
	int longitud;			//variable para recibir la longitud de la cadena
	//direcciones de socket
	struct sockaddr_in serv_addr; 	//dirección del servidor
	struct sockaddr_in cli_addr;	//dirección del cliente conectado
	socklen_t addr_len;             //tamaño de la dirección del cliente conectado

	//2.- Creamos la dirección del servidor
	//2.1.- Inicializamos
	memset(&serv_addr, 0, sizeof(serv_addr));
	//2.2.- Rellenamos campos
	//2.2.1.- Familia de direcciones
	serv_addr.sin_family = AF_INET;
	//2.2.2.- Puerto
	serv_addr.sin_port =htons(PUERTO);
	//2.2.3.- Dirección IP
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	 
	//3.- Creamos el socket
	if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0 ) { // 0 al final porq es TCP
		perror("socket");
		exit(1);
	}
  
	//4.- Asociamos el socket al puerto: bind()
	if (bind(sd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 ) {
		perror("bind");
		exit(1);
	}
	
	//5.- Ponemos el socket en escucha (modo pasivo): listen()
	if (listen(sd, 10) < 0)  { // 10 tamanyo de cola
		perror("listen");
		exit(1);
	}
	
	//6.- Iteramos
	while(1) {
		printf("Servidor esperando conexión...\n");
		//6.1.- Aceptamos una conexión: accept()
		if ((n_sd = accept(sd, (struct sockaddr *) &cli_addr, &addr_len)) < 0) {
			perror("accept");
			close(n_sd);
			close(sd);
			exit(1);
		}

		//Aquí toca pensar...
    //Leemos longitud + cadena
    //Lectura adelantada de la longitud, y si leo algo, sigo leyendo la cadena
    //Una vez recibida la cadena completa, la enviamos de la misma
    //forma: longitud+cadena
    int leidos;
    uint32_t longBigEndian;
    leidos = read_n(n_sd, &longBigEndian, sizeof(longBigEndian));
    while(leidos > 0){
      // Aseguro que leo todo
      if(leidos != sizeof(longBigEndian)){
        perror("read_n longBigEndian");
        close(sd);
        close(n_sd);
        exit(-1);
      }
      // Ya se que el cliente esta vivo
      // y he recibido el numero de bytes esperado
      longitud = ntohl(longBigEndian);
      // Leemos la cadena
      if(read_n(n_sd, buffer, longitud) != longitud){
        perror("read_n");
        close(sd);
        close(n_sd);
        exit(-1);
      }
      // Imprimo lo que recibo
      printf("buffer = %s\n", buffer);
      // Respondo al cliente ECHO con la misma
      // cadena y siguiendo el mismo formato: longitud + cadena
      // Convertimos a formato de red
      longBigEndian = htonl(longitud);
      // Envio longitud
      if(write_n(n_sd, &longBigEndian, sizeof(longBigEndian)) != sizeof(longBigEndian)){
        perror("write_n longBigEndian");
        close(sd);
        close(n_sd);
        exit(-1);
      }
      // Envio la cadena
      if(write_n(n_sd, buffer, longitud) != longitud){
        perror("write_n longBigEndian");
        close(sd);
        close(n_sd);
        exit(-1);
      }
      // Vuelvo a leer
      leidos = read_n(n_sd, &longBigEndian, sizeof(longBigEndian));
    }
    if(leidos < 0){
      perror("read_n longBigEndian");
      close(sd);
      close(n_sd);
      exit(-1);
    } else {
      // leidos = 0
      if(close(n_sd) < 0){
        perror("close");
        exit(-1);
      }
    }
	}
	
	//7.- Close
	if (close(n_sd) < 0) {
		perror("close");
		exit(1);
	}
	
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
    /* Para permitir que devuelva un posible error de la llamada a write */
    return escrito;
  }
}
