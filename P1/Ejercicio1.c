#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define T 32

void imprimirBytes(char *filename);

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Uso: %s <nombredelfichero>\n", argv[0]);
       return -1;
    }
    imprimirBytes(argv[1]);
    return 0;
}

void imprimirBytes(char *filename){
    // 1. Abrir el fichero
    int fd;
    if((fd = open(filename, O_RDONLY))<0){
	perror("open");
	exit(-1);
    }
    // 2. Imprimir la info

    char buffer[T];
    int leidos;
    leidos = read(fd, buffer, T);
    while(leidos > 0){
	printf("%s", buffer);
	leidos = read(fd,buffer, T);
    }
    if(leidos < 0){
	perror("read");
    }
    if(close(fd) < 0){
	perror("close");
    }
}
