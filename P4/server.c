#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAXBUFFSIZE 8
int max_fd(int actual, int nuevo); // Devuelve el mayor de dos int
void copia_fdset(fd_set *destino, fd_set *origen, int max_fd_mas_uno); // Copia fdset

int main(int argc, char *argv[]){
  if(argc < 3){
    printf("ERROR!");
    return -1;
  }
  char buffer[MAXBUFFSIZE];
  // Apertura de fifos
  int f1, f2;
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
 	tiempo.it_value.tv_sec = 1;
	tiempo.it_value.tv_usec = 0;
	tiempo.it_interval.tv_sec = 0;
	tiempo.it_interval.tv_usec = 0;
  while(1){
    // Cada vez que llamamos a select creamos una copia del conjunto original
    copia_fdset(&active_rfd, &rfd, max_fd(f1,f2)+1);
    // Select comprueba si algun fifo esta abierto para lectura. Como no hay
    // timeout espera indefinidamente
    int rc;
    if((rc= select(max_fd(f1,f2)+1, &active_rfd, NULL, NULL, tiempo)< 0){
      perror("Select");
      close(f1); close(f2);
      return -1;
    }
    // Si el f1 tiene algo que leer de el...
    int leidos;
    if(FD_ISSET(f1, &active_rfd)){
      if((leidos = read(f1, buffer, MAXBUFFSIZE) < 0)){
          perror("Read");
          close(f1); close(f2);
          return -1;
        }
      printf("%s\n", buffer);
    }
    if(FD_ISSET(f2, &active_rfd)){
      if((leidos = read(f2, buffer, MAXBUFFSIZE) < 0)){
          perror("Read");
          close(f1); close(f2);
          return -1;
      }
      printf("%s\n", buffer);
    }
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
