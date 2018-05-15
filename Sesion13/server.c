/*
Implemente en C un servidor de registro de sucesos (logs) remotos que va almacenando en un fichero la información que llega de dos clientes. La especificación es la siguiente:

  Para comunicarse con ellos, el servidor recibe como argumento de entrada los nombres de dos FIFOs que se usan para enviar los eventos que hay que almacenar.
  El servidor procede atendiendo a los dos clientes, demultiplexando la entrada/salida con la función select().
  Los eventos son enviados por los clientes como cadenas de texto y, una vez las recibe, el servidor las escribe en un fichero llamado log.txt.
  Para distinguir la procedencia de los eventos, el servidor prefija cada cadena con el cliente que la envía: Cliente 1: y Cliente 2: , respectivamente.
  Cada cadena se inserta en una línea nueva, sin líneas vacías entre ellas
  Si el fichero de logs no existe, se crea y, si existe, se añaden entradas por el final.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#define T 512

int max_fd(int actual, int nuevo); // Devuelve el mayor de dos fd (ints).

void copia_fdset(fd_set *destino, fd_set *origen, int max_fd_mas_uno);

void procesarFifo(int fifo,int fd, int *conectados, char *prefijo);

int main(int argc, char *argv[]){

  if (argc < 3){
    exit(-1);
  }

  // Abrimos las fifos 
  int f1, f2;

  if((f1 = open(argv[1], O_RDONLY)) < 0){
    perror("Open f1");
    exit(-1);
  }

  if((f2 = open(argv[2], O_RDONLY)) < 0){
    perror("Open f2");
    exit(-1);
  }
  // Abrimos el fichero
  int fd;
  if((fd = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644)) < 0){
    perror("Open log.txt");
    close(f1); close(f2);
    exit(-1);
  }
  // Configuramos los conjuntos de descriptores

  fd_set rfd, active_rfd;
  FD_ZERO(&rfd);
  FD_SET(f1, &rfd);
  FD_SET(f2, &rfd);

  int conectados = 2;

  while(conectados > 0){
    // Cada vez que vayamos a llamar a select() creamos una copia de rfd para
    // manipular esta en vez del conjunto original

    copia_fdset(&active_rfd, &rfd, max_fd(f1, f2) + 1);

    // select() comprueba si algun fifo esta abierto para lectura. Al no haber
    // timeout espera indefinidamente. A select() le pasamos el maximo descriptor + 1
    // y la copia de rfd, ya que dejara en esta solo los descriptores de los archivos
    // que estan listos

    if((select(max_fd(f1, f2) + 1, &active_rfd, NULL, NULL, NULL)) < 0){
      perror("select");
      close(f1); close(f2);
      exit(-1);
    }

    // Procedemos a comprobar el contenido de active_rfd
    if(FD_ISSET(f1, &active_rfd)){
      // Procesamos f1
      procesarFifo(f1, fd, &conectados, "Cliente 1: ");
    }
    if(FD_ISSET(f2, &active_rfd)){
      procesarFifo(f2, fd, &conectados, "Cliente 2: ");

    }
  }
  // Cerramos las fifos
  if(close(f1) < 0){
    perror("Close");
    exit(-1);
  }
  if(close(f2) < 0){
    perror("Close");
    exit(-1);
  }
  return 0;
}

int max_fd(int actual, int nuevo){
  if(actual > nuevo){
    return actual;
  }
  return nuevo;
}

void copia_fdset(fd_set *destino, fd_set *origen, int max_fd_mas_uno){
  FD_ZERO(destino); // Vaciamos el destino
  // Iteramos desde el descriptor 0 hasta el mas grande + 1. Si
  // el descriptor esta en el conjunto original se copia en el destino
  for(int i = 0; i < max_fd_mas_uno; i++){
    if(FD_ISSET(i, origen)){
      FD_SET(i, destino);
    }
  }
}

void procesarFifo(int fifo, int fd, int *conectados, char *prefijo){
  // Leemos de la fifo
  int leidos, longitud;
  char buffer[T];
  // Leemos la longitud del mensaje
  leidos = read(fifo, &longitud, sizeof(int));
  // Comprobamos los valores que devuelve la lectura

  if(leidos < 0){ // Error de lectura

    perror("Read longitud");
    exit(-1);

  } else if (leidos == 0) { // Se ha cerrado la fifo

    (*conectados)--;

  } else if (leidos != sizeof(int)){ // Hemos leido erroneamente la longitud

    perror("Read longitud");
    exit(-1);

  } else { // Leido == sizeof(int), por lo que hemos leido bien la longitud

    // Leemos el mensaje
    leidos = read(fifo, buffer, longitud);
    if(leidos < 0){

      perror("Read mensaje");
      exit(-1);

    } else if (leidos != longitud){ // La longitud del mensaje recibido no coincide con la esperada

      perror("Read mensaje");
      exit(-1);

    } else if (leidos == 0){ // Fifo desconectada

      (*conectados)--;

    } else {

      // Hemos leido correctamente el mensaje, por lo que escribimos en log.txt
      buffer[longitud] = '\0';
      int l = strlen(prefijo);
      if(write(fd, prefijo, l) != l){
        perror("Write log.txt");
        exit(-1);
      }
      if(write(fd, buffer, longitud) != longitud){
        perror("Write log.txt");
        exit(-1);
      }
      if(write(fd, "\n", 1) != 1){
        perror("Write log.txt");
        exit(-1);
      }
    }
  }
}
