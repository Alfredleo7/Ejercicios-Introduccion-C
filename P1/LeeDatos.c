#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define T 32
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

struct Estructura {
    int a;
    float b;
    char c;
};

void cargarBytes(char *filename);

int main(int argc, char *argv[]){
    if(argc < 2){
	printf("Error!\n");
	return -1;
    }
    cargarBytes(argv[1]);
    return 0;
}

void cargarBytes(char *filename){
    int leidos, fd_origen;
    if((fd_origen =open(filename, O_RDONLY)) < 0){
	perror("open origen");
	exit(-1);
    }
    // Struct est
    struct Estructura est;
    if((leidos = read(fd_origen,&est.a, sizeof(int))) < 0){
	perror("est.a");
	exit(-1);
    }
    printf("%d\n", est.a);
    if((leidos = read(fd_origen, &est.b, sizeof(float))) < 0){
	perror("est.b");
	exit(-1);
    }
    printf("%F\n", est.b);
    if((leidos = read(fd_origen, &est.c, sizeof(char))) < 0){
	perror("est.c");
	exit(-1);
    }
    printf("%c\n", est.c);
    // int x
    int x;
    if((leidos = read(fd_origen, &x, sizeof(int))) < 0){
	perror("int x");
	exit(-1);
    }
    printf("%d\n", x);
    // arrayEnteros
    int arrayEnteros[4];
    for(int i = 0; i < 4; i += 1){
	if((leidos = read(fd_origen, &arrayEnteros[i], sizeof(int))) < 0){
	    perror("Array");
	    exit(-1);
	}
	printf("%d ", arrayEnteros[i]);
    }
    if(close(fd_origen) < 0){
	perror("close");
	exit(-1);
    }
}
