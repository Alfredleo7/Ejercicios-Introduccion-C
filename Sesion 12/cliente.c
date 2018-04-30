#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#define T 512

int main(int argc, char *argv[]){
  // Procesamos los argumentos
  if(argc < 2){
    return -1;
  }
  // Abrimos la FIFO
  int fifo;
  if((fifo = open(argv[1], O_WRONLY)) < 0){
    perror("open");
    return -1;
  }
  // Leemos por primera vez

  int leidos;
  char buffer[T];
  
  if((leidos = read(0, buffer, T)) < 0){ // Lectura del mensaje
    perror("read");
    return -1;
  }
  int longitud = strlen(buffer); // Medimos la longitud del mensaje
  buffer[leidos - 1] = '\0'; // Sustituimos el '\n'

  // Escribimos por primera vez

  if((write(fifo, &longitud, sizeof(longitud)) != sizeof(longitud))){
    perror("write longiutd");
    return -1;
  }
  if((write(fifo, buffer, longitud)) != longitud){
    perror("write mensaje");
    return -1;
  }
  // Lectura y escritura iterada
  while(strcmp(buffer, "fin\n") != 0){
    if((leidos = read(0, buffer, T)) < 0){ // Lectura del mensaje
      perror("read");
      return -1;
    }
    longitud = strlen(buffer); // Medimos la longitud del mensaje
    buffer[leidos - 1] = '\0'; // Sustituimos el '\n'
    
    if((write(fifo, &longitud, sizeof(longitud)) != sizeof(longitud))){
      perror("write longiutd");
      return -1;
    }
    if((write(fifo, buffer, longitud)) != longitud){
      perror("write mensaje");
      return -1;
    }
  }
}
