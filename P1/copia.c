#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define T 32

void copiarFichero(char *origen, char *destino);

int main(int argc, char *argv[]){
    if(argc < 3){
	printf("Too few arguments!\n");
	return -1;
    }
    copiarFichero(argv[1], argv[2]);
    return 0;
}

void copiarFichero(char *origen, char *destino){
    // 1. Abrir ficheros
    int fd_Origen, fd_Destino, leidos, escritos;
    if((fd_Origen = open(origen, O_RDONLY)) < 0){
	perror("open origen");
	exit(-1);
    }
    if((fd_Destino = open(destino, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0){
	perror("open destino");
	exit(-1);
    }

    // 2. Copiar la info

    char buffer[T];
    leidos = read(fd_Origen, buffer, T);
    while(leidos > 0){
	escritos = write(fd_Destino, buffer, T);
	if(escritos < 0){
	    perror("write");
	    close(fd_Origen);
	    close(fd_Destino);
	    exit(-1);
	}
	leidos = read(fd_Origen, buffer, T);
    }
    close(fd_Origen);
    close(fd_Destino);
}
igen);
 