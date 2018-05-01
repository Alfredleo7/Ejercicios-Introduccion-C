#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#define T 512

void procesarFifo(int fifo, int fd, int *conectados, char *prefijo);

int main(int argc, char *argv[]){
    // Procesar argumentos(nombres de las FIFOs) con las que nos conectamos a
    // los clientes.
    if(argc < 3){
      return -1;
    }
    // Apertura de FIFOs y ficheros
    int fifo1, fifo2, fd;

    if((fifo1 = open(argv[1], O_RDONLY)) < 0){
      perror("open");
      return -1;
    }
    if((fifo2 = open(argv[2], O_RDONLY)) < 0){
      perror("open");
      return -1;
    }
    if((fd = open("logs.txt", O_WRONLY | O_CREAT | O_APPEND, 0644)) < 0){
      perror("open");
      return -1;
    }
    
    // Iteramos mientras haya clientes conectados
    int conectados = 2;
    while(conectados > 0){
      // Leer fifo1 y escribir en fichero con el prefijo
      procesarFifo(fifo1, fd, &conectados, "Cliente 1: ");
      // Leer fifo2 y escribir en fichero con el prefijo
      procesarFifo(fifo2, fd, &conectados, "Cliente 2: ");
    }
    
    // Cerramos FIFOs y fichero
    if((close(fifo1)) < 0){
      perror("close");
      return -1;
    }
    if((close(fifo2)) < 0){
      perror("close");
      return -1;
    }
    if((close(fd)) < 0){
      perror("close");
      return -1;
    }

    return 0;
}
void procesarFifo(int fifo, int fd, int *conectados, char *prefijo){
  // Leemos de la fifo
  int leidos, longitud;
  char buffer[T];
  // Leemos la longitud del mensaje
  leidos = read(fifo, &longitud, sizeof(int));
  // Que valores puede devolver leer de una FIFO?
  if(leidos < 0){
    perror("read n longitud");
    exit(-1);
  } else if(leidos == 0) { // El otro extremo ha desconectado, asi q leemos 0
    (*conectados)--;
  } else if(leidos != sizeof(int)){ // No hemos leido la longitud correctamente
    perror("read n longitud");
    exit(-1);
  } else {
    // Estoy seguro de haber leido la longitud de la cadena
    leidos = read(fifo, buffer, longitud);
    if(leidos < 0){
      perror("read");
      exit(-1);
    } else if (leidos != longitud){
      perror("read");
      exit(-1);
    } else if (leidos == 0){
      (*conectados)--;
    } else {
	    // He recibido bien la cadena, por lo que escribo
      buffer[longitud] = '\0';
	    int l = strlen(prefijo);
      if(write(fd, prefijo, l) != l){
        perror("write");
        exit(-1);
	    }
	    if(write(fd, buffer, longitud) != longitud){
        perror("write");
        exit(-1);
	    }
	    if(write(fd, "\n", 1) != 1){
        perror("write");
        exit(-1);
	    }
    }
  }
}
