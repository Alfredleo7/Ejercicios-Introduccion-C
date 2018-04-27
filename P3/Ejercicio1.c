#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#define T 512

int main(){
  // Apertura de pipe
  int p[2];
  if(pipe(p)){
    perror("pipe");
    return -1;;
    }
  // Generamos el hijo
  pid_t pid = fork();
  if (pid > 0){ // Codigo del padre
    // Cerramos el extremo de la pipe que no vamos a utilizar
    if(close(p[0]) < 0){
      perror("close pipe");
      return -1;
    }
    // Leemos de teclado y escribimos por la pipe
    // Primera lectura
    int leidos, escritos;
    char bufferEscritura[T];
    if((leidos = read(0, bufferEscritura, T)) < 0){
      perror("read");
      return -1;
    }
    int longitud = strlen(bufferEscritura);
    bufferEscritura[leidos - 1] = '\0';
    if((write(p[1], &longitud, sizeof(longitud)) != sizeof(longitud))){
      perror("write longitud");
      return -1;
    }
    if((write(p[1], bufferEscritura, longitud) != longitud)){
      perror("write frase");
      return -1;
    }
    // Lectura iterada hasta encontrar "fin\n" o error
    while((strcmp(bufferEscritura, "fin")) != 0 &&  longitud > 0){
      if((leidos = read(0, bufferEscritura, T)) < 0){
        perror("read");
        return -1;
      }
      int longitud = strlen(bufferEscritura);
      bufferEscritura[leidos-1] = '\0';
      if((write(p[1], &longitud, sizeof(longitud)) != sizeof(longitud))){
        perror("write longitud");
        return -1;
      }
      if((write(p[1], bufferEscritura, longitud) != longitud)){
        perror("write frase");
        return -1;
      }
    }
    if(close(p[1]) < 0 ){
      perror("close pipe");
      return -1;
    }
    wait(0);
  } else if (pid == 0) {
    // Codigo del hijo
    // Cerramos el extremo que no necesitamos de la pipe
    if(close(p[1]) < 0){
      perror("close pipe");
      exit(-1);
    }
    char bufferLectura[T];
    int longitudRecibida;
    while(1){
      // Leemos
      if((read(p[0], &longitudRecibida, sizeof(longitudRecibida))) != sizeof(longitudRecibida)){
        perror("read");
        exit(-1);
      }
      if((read(p[0], bufferLectura, longitudRecibida)) != longitudRecibida){
        perror("read");
        exit(-1);
      }
      printf("Hijo: %s\n", bufferLectura);
      if(strcmp(bufferLectura, "fin") == 0){
        break;
      }
    }
    if(close(p[0]) < 0){
      perror("close pipe");
      exit(-1);
    }
    exit(0);
  } else {
    // Error
    perror("Fork");
    return -1;
  }
}
