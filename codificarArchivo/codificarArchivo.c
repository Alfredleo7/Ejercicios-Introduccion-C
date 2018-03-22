/* Este programa codifica un archivo y lo guarda en otro nuevo con el nombre facilitado.
 * La codificacion consiste en sumar a cada caracter del archivo el offset indicado.
 * Si tuvieramos un archivo con una cadena tal que: "AAAA", y lo codificamos con un offset
 * de 1, obtendriamos un nuevo archivo con la cadena "BBBB". */
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
	// 1. Comprobamos que la invocacion al programa es correcta
	if (argc < 4){
	printf("Error! Uso: %s <nombreorigen> <nombredestino> <offset>\n", argv[0]);
	exit(-1);
	}
	// 2. Procedemos a codificar el archivo
	codificar(argv[1], argv[2], atoi(argv[3]));
	return 0;
}

void codificar(char *origen, char *destino, int offset){
	// 1. Abrimos los ficheros, comprobando en ambos que no haya error al hacer esto.
	int fd_origen, fd_destino; // Enteros que representan al archivo (file descriptor)
	if((fd_origen = open(origen, O_RDONLY)) < 0){
		perror("open origen");
		exit(-1);
	}

	if((fd_destino = open(destino, O_WRONLY | O_CREAT | O_TRUNC, 0644))<0){
		perror("open destino");
		exit(-1);
	}
	// 2. Procesamos los ficheros. Leo de origen, codifico, escribo en destino
	char buffer[T]; // Buffer donde almacenaremos T bytes para codificarlos
	int leidos, escritos; // Numero de bytes leidos y escritos
	//Leemos del archivo al que corresponde fd_origen T bytes y los almacenamos en buffer
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
