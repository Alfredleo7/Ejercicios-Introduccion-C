/* Simple programa que detecta la pulsacion de CTRL-c 
 * mediante el uso de señales del sistema Linux */
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#define T 256

void manejador(int signo){
	printf("CTRL-c pulsado \n");
	signal(SIGINT, manejador);
}

int main(){
	signal(SIGINT, manejador);
	printf("Manejando CTRL-c...\n");
	char b[T];
	int leidos;
	while(1){
		printf("Introduzca una cadena:\n");
		leidos = read(0, b, T);
		b[leidos-1] = '\0';
		printf("b = %s\n", b);
	}

}
