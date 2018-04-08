#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define T 32
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

struct Estructura{
    int a;
    float b;
    char c;
};



void guardarBytes(char *filename, struct Estructura est, int x, int *arrayEnteros);

int main(int argc, char *argv[]){
    if(argc < 2){
	printf("ERROR!\n");
	return -1;
    }
    int x = 7;
    int array_enteros[4] = {0x00, 0x01, 0x02, 0x02};
    for(int i = 0 ; i < 4; i++){
	printf("%d\n", array_enteros[i]);
    }
    struct Estructura est;
    est.a = 1;
    est.b = 2.0;
    est.c = '3';
    guardarBytes(argv[1], est, x, array_enteros);
    return 0;
}

void guardarBytes(char *filename, struct Estructura est, int x, int *arrayEnteros){
    int escritos, fd_destino;
    if((fd_destino = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0){
	perror("open");
	exit(-1);
    }
    
    // Struct est
    if((escritos = write(fd_destino, &est.a,sizeof(int))) < 0){
	perror("est.a");
	exit(-1);
    }
    if((escritos = write(fd_destino, &est.b, sizeof(float))) < 0){
	perror("est.b");
	exit(-1);
    }   
    if((escritos = write(fd_destino, &est.c, sizeof(char))) < 0){
	perror("est.c");
	exit(-1);
    }
    // int x
    if((escritos = write(fd_destino, &x, sizeof(int))) < 0){
	perror("int x");
	exit(-1);
    }
    // arrayEnteros
    for(int i = 0; i < 4; i+=1){
	if((escritos = write(fd_destino,&arrayEnteros[i], sizeof(int))) < 0){
	    perror("Array");
	    exit(-1);
	}
    }
    if(close(fd_destino) < 0){
	perror("close");
	exit(-1);
    }
}
