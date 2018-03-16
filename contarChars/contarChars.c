#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#define T 32

void contar(char *filename, int *lower,
                 int *upper, int *digit);
int main(int argc, char *argv[]){
    if (argc < 2){
        printf("Uso: %s <nombredesuarchivo>\n", argv[0]);
        return -1;
    }
    int lower = 0, upper = 0, digit = 0;
    contar(argv[1], &lower, &upper, &digit);
    printf("Lower: %d, Upper: %d, Digit: %d", lower, upper, digit);
}

void contar(char *filename, int *lower, int *upper, int *digit){
    // 1. Abrir el archivo 
    int fd;
    if((fd = open(filename, O_RDONLY)) < 0){
        perror("open");
        exit(-1);
    }
    // 2. Procesar la informacion
    *lower = 0;
    *upper = 0;
    *digit = 0;
    char buffer[T];
    int leidos = read(fd, buffer, T);
    while(leidos > 0){
        for(int i = 0; i < leidos; i++){
            if(islower(buffer[i])){
            *lower+=1;
            }
          if(isupper(buffer[i])){
              *upper+=1;
          }
          if(isdigit(buffer[i])){
              *digit+=1;
          }
        }
    leidos = read(fd, buffer, T);
    }
}
