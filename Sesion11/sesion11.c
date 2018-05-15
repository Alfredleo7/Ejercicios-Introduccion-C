#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <stdlib.h>

pid_t pidHijo;

void matarProceso(){
    // Manejador de SIGALRM
    kill(pidHijo, SIGKILL);
}

int main(int argc, char *argv[]){
    // Manejamos los argumentos
    if(argc < 3){
	return -1;
    }
    // Creamos el hijo
    pidHijo = fork();
    if(pidHijo > 0){
	// El padre pone la alarma para mandar SIGKILL al hijo
	signal(SIGALRM, matarProceso);
	struct itimerval tiempo;
	tiempo.it_value.tv_sec = atoi(argv[1]);
	tiempo.it_value.tv_usec = 0;
	tiempo.it_interval.tv_sec = 0;
	tiempo.it_interval.tv_usec = 0;

	setitimer(ITIMER_REAL, &tiempo, NULL);
	wait(0);
    } else if(pidHijo == 0){
	// Codigo del hijo
	int ret;
	char *arg = "/bin/bash";
	if((ret = execl(arg, "bash", "-c", argv[2], NULL)) < 0){
	    perror("execl");
	    exit(-1);
	}
    } else {
	perror("fork");
	return -1;
    }
    return 0;
}
