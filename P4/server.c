#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>
#define MAXBUFFSIZE 8
void manejador(int signo);
int max_fd(int actual, int nuevo); // Devuelve el mayor de dos int
void copia_fdset(fd_set *destino, fd_set *origen, int max_fd_mas_uno); // Copia fdset
void procesarFifo(int fifo, int *conectados, char *prefijo);
int main(int argc, char *argv[]){
  if(argc < 3){
    printf("ERROR!");
    return -1;
  }
  char buffer[MAXBUFFSIZE];
  // Apertura de fifos
  int f1, f2;
  signal(SIGUSR1, manejador);
  printf("Mi pid es: %d\n", getpid());
  pause();
  pause();
  if((f1=open(argv[1], O_RDONLY))< 0){
    perror("Open f1");
    return -1;
  }
  if((f2=open(argv[2], O_RDONLY)) < 0){
    perror("Open f2");
    return -1;
  }
  // Configuramos los conjuntos de descriptores
  fd_set rfd, active_rfd;
  FD_ZERO(&rfd);
  FD_SET(f1, &rfd);
  FD_SET(f2, &rfd);
  struct itimerval tiempo;
 	tiempo.it_value.tv_sec = 3;
	tiempo.it_value.tv_usec = 0;
	tiempo.it_interval.tv_sec = 0;
	tiempo.it_interval.tv_usec = 0;

  int conectados = 2;

  while(conectados > 0){
    // Cada vez que llamamos a select creamos una copia del conjunto original
    copia_fdset(&active_rfd, &rfd, max_fd(f1,f2)+1);
    // Select comprueba si algun fifo esta abierto para lectura. Como no hay
    // timeout espera indefinidamente
    int rc;
    if((rc= select(max_fd(f1,f2)+1, &active_rfd, NULL, NULL, &tiempo.it_value))< 0){
      perror("Select");
      close(f1); close(f2);
      return -1;
    }
    // Si el f1 tiene algo que leer de el...
    int leidos;
    if(FD_ISSET(f1, &active_rfd)){
      // Procesamos f1
      procesarFifo(f1, &conectados, "Cliente 1:");
    }
    if(FD_ISSET(f2, &active_rfd)){
      // Procesamos f2
      procesarFifo(f2, &conectados, "Cliente 2:");
    }
  }
  // Cerramos fifos
  if((close(f1)) < 0){
    perror("Close");
    exit(-1);
  }
  if((close(f2)) < 0){
    perror("Close");
    exit(-1);
  }
  return 0;
}

int max_fd(int actual, int nuevo){ // Devuelve el mayor de dos int
  if(actual > nuevo){
    return actual;
  }
  return nuevo;
}

void copia_fdset(fd_set *destino, fd_set *origen, int max_fd_mas_uno){ // Copia fdset
  FD_ZERO(destino);
  for(int i = 0; i < max_fd_mas_uno; i++){
    if(FD_ISSET(i, origen)){
      FD_SET(i,destino);
    }
  }
}

void procesarFifo(int fifo, int *conectados, char *prefijo){
  // Leemos de la fifo
  int leidos, longitud;
  char buffer[MAXBUFFSIZE];
  // Leemos la longitud del mensaje
  leidos = read(fifo, &longitud, sizeof(int));
  // Comprobamos los valores que devuelve la lectura
  if(leidos < 0){
    printf("Primer read de %s\n",prefijo);
    // Error de lectura
    perror("Read longitud");
    exit(-1);
  } else if(leidos == 0){
    // Hemos leido 0, por lo que la fifo se ha desconectado
    (*conectados) --;
  } else if(leidos != sizeof(int)){
    // Hemos leido erroneamente la longitud
    printf("Segundo read de %s\n",prefijo);
    perror("Read longitud");
    exit(-1);
  } else {
    // Hemos recibido la longitud
    leidos = read(fifo, buffer, longitud);
    if(leidos < 0){
      // Error de lectura
      perror("Read");
      exit(-1);
    } else if(leidos != longitud){
      // No hemos recibido la cadena completa
      perror("Read");
      exit(-1);
    } else if(leidos == 0){
      // Se ha desconectado
      (*conectados)--;
    } else {
      buffer[longitud] = '\0';
      printf("%s ", prefijo);
      printf("%s\n", buffer);
    }
  }
}

void manejador(int signo){
  printf("Ha llegado SIGUSR1\n");
  signal(SIGUSR1, manejador);
}
