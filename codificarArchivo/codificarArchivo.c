#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#define T 32
void codificar(char *origen, char *destino, int offset);
void codificarBuffer(char *buffer, int offset, int leidos);
int main (int argc, char *argv[]){
	if (argc < 4){
	printf("Error! Uso: %s <nombreorigen> <nombredestino> <offset>\n", argv[0]);
	exit(-1);
	}
	codificar(argv[1], argv[2], atoi(argv[3]));
	return 0;
}

void codificar(char *origen, char *destino, int offset){
	// 1. Abrimos los ficheros
	int fd_origen, fd_destino;
	if((fd_origen = open(origen, O_RDONLY)) < 0){
		perror("open origen");
		exit(-1);
	}

	if((fd_destino = open(destino, O_WRONLY | O_CREAT | O_TRUNC, 0644))<0){
		perror("open destino");
		exit(-1);
	}
	// 2. Procesamos los ficheros. Leo de origen, codifico, escribo en destino
	char buffer[T];
	int leidos, escritos;
	leidos = read(fd_origen, buffer, T);
	while(leidos > 0){
		// Codificamos el contenido del buffer
		codificarBuffer(buffer, offset, leidos);
		printf("%s\n", buffer);
		// Escribimos en destino
		escritos = write(fd_destino, buffer, leidos);
		if (escritos<0){
			perror("write");
			close(fd_origen);
			close(fd_destino);
			exit(-1);
		}
		printf("%d\n", escritos);
		leidos = read(fd_origen, buffer, T);
	}
	if(leidos<0){
		perror("read");
		exit(-1);
	}
}
void codificarBuffer(char *buffer, int offset, int leidos){
	int pos = 0;
	for(int i = 0; i < leidos; i++){
		buffer[pos] += offset;
		pos+=1;
	}
}
