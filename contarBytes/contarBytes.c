#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define T 32

int contarBytes(char *filename);
int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Uso: %s <nombredelfichero>\n", argv[0]);
       return -1;
    }
int bytes = contarBytes(argv[1]);
printf("Num de bytes: %d\n", bytes);
return 0;

}
int contarBytes(char *filename){
    int cont = 0; // Contador
    // 1. Abrir el fichero
    int fd;
    fd = open(filename, O_RDONLY); // Descriptor del fichero
    if(fd < 0){
        //Es error, ya que open devuelve entero > 0 y solo < 0 para error
       perror("open"); // Mostramos el error
       exit(-1); // Cerramos el programa. Return devuelve el control a la funcion desde la que invocamos, exit cierra el proceso.
    }
    /*Las lineas mostradas mas arriba podrian hacerse de forma mas corta:
     * if((fd = open(filename, U_RDONLY)) < 0){} */
    // 2. Procesar la informacion
    char buffer[T];
    int leidos;
    // lectura adelantada + iterador
    leidos = read(fd, buffer, T); // Lee de fd T bytes y los guarda en buffer
    while (leidos > 0){
        // Acumulamos el num de bytes leidos
       cont += leidos;
       leidos = read(fd, buffer, T);
    }
    if(leidos < 0){
        perror("read");
    }
    if(close(fd)<0){
        perror("close");
        exit(-1);
    }
    return cont;
}
