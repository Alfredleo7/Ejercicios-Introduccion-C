#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]){

    // Creamos el hijo
    pid_t pidA = fork();

    if(pidA == 0){
	// Codigo del hijoA - Envia SIGUSR1 cada 2s e imprime
	printf("Soy el hijoA con pid %d. Mi padre es pid %d\n",getpid(), getppid());
	pause();
	exit(0);
    } else if(pidA > 0) {
	// Codigo del padre
	// Creamos el segundo hijo y cogemos su pid
	pid_t pidB = fork();
	
	if(pidB == 0){
	    // Codigo del hijoB - Espera SIGUSR1 de la funcion espera_hijo e imprime
	    printf("Soy el hijoB con pid %d. Mi padre es pid %d\n", getpid(), getppid());
	    pause();
	    exit(0);
	} else if(pidB > 0){
	    // Codigo del padre
	    printf("Soy el padre con pid %d y acabo de crear a pid %d\n", getpid(), pidB);
	    wait(0);
	    wait(0);
	} else {
	    perror("Segundo fork");
	    return -1;
	}
    } else {
	perror("fork");
	return -1;
    }
}
