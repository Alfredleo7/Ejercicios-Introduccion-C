// NO FUNCIONA BIEN PORQUE ENVIAMOS T = 128. Para que funcione bien debemos mandar la info
// tal que: 4 Paco 5 Lopez, de manera que sepamos cuantos chars tenemos que leer a la salida
// Hay que terminar de implementar esto en el hijo
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#define T 128
/* El padre va a leer unos argumentos, los mandara al hijo y este
 * los imprimira por pantalla. */

int main(int argc, char *argv[]){
    // Creamos la PIPE
    int p[2];
    if (pipe(p) < 0){
	perror("pipe");
	return -1;
    }
    // Variables
    char nombre[T];
    char apellido[T];
    int leidos;
    // Nos clonamos: fork()
    pid_t pid;
    if((pid = fork()) < 0){
	perror("fork");
	return -1;
    }
    if( pid > 0){
	// Codigo del padre
	printf("Introduce nombre: \n");
	leidos = read(0, nombre, T);
	nombre[leidos - 1] = '\0';

	printf("Introduce apellido: \n");
	leidos = read(0, nombre, T);
	apellido[leidos - 1] = '\0';

	// Cerramos el extemo de la PIPE que no necesitamos
	if(close(p[0]) < 0){
		perror("close");
		return -1;
	}
	// Enviamos los datos a nuestro hijo
	int longitud = strlen(nombre);
	write(p[1], &longitud, sizeof(longitud));
	write(p[1], nombre, longitud);
	longitud = strlen(apellido);
	write(p[1], &longitud, sizeof(longitud));
	write(p[1], apellido, longitud);
	// Cerramos la PIPE
	if(close(p[1])){
	    perror("Close");
	    return -1;
	}
	// Esperamos al hijo
	wait(0);
    } else {
	// Codigo del hijo
	// Cerramos lo que no necesitamos
	if(close(p[1]) < 0) {
		perror("close");
		return -1;
	}
	leidos = read(p[0], nombre, T);
	nombre[leidos] = '\0'; // Volvemos a poner el \0 ya que al pasarlo por el pipe solo pasa el string
	printf("Nombre = %s\n", nombre);
	leidos = read(p[0], apellido, T);
	apellido[leidos] = '\0';
	printf("Apellido = %s\n", apellido);
	// Terminamos
	exit(0);
    }

    return 0;
}
