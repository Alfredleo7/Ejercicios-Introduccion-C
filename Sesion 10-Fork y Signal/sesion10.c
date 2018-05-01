#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#define T 256
char msg[T];

void espera_hijo(){
    // Imprimimos el mensaje
    sprintf(msg,"Soy el proceso padre con pid = %d: ya voy!\n",getpid());
    write(1,msg,strlen(msg));
    signal(SIGUSR1, espera_hijo);
}


void espera_padre(){
    // Imprimimos el mensaje
    sprintf(msg, "Soy el proceso hijo2 con pid = %d: ya estoy despierto, pero me duermo enseguida\n", getpid());
    write(1, msg, strlen(msg));
    signal(SIGUSR1, espera_padre);
}
void temporizador(){
    sprintf(msg,"Soy proceso hijo1 con PID=%d: papá, despierta a mi hermano\n",getpid());
    write(1,msg,strlen(msg));
    signal(SIGALRM, temporizador);
    kill(getppid(), SIGUSR1);
}

int main(int argc, char *argv[]){
    // Preparamos el signal del padre
    signal(SIGUSR1, espera_hijo);
    // Creamos el hijo
    pid_t pidA = fork();

    if(pidA == 0){
	// Codigo del hijoA - Envia SIGUSR1 cada 2s e imprime
	struct itimerval t_normal;

	struct timeval timer, interval;
	
	timer.tv_sec = 2;
	timer.tv_usec = 0;

	interval.tv_sec = 2;
	interval.tv_usec = 0;

	t_normal.it_value = timer;
	t_normal.it_interval = interval;
	
	signal(SIGALRM, temporizador);
	setitimer(ITIMER_REAL,&t_normal, NULL);
	pause();
	while(1){
	    setitimer(ITIMER_REAL, &t_normal, NULL);
	    signal(SIGUSR1,SIG_IGN);
	    pause();
	}
    } else if(pidA > 0) {
	// Codigo del padre
	// Creamos el segundo hijo y cogemos su pid
	pid_t pidB = fork();
	
	if(pidB == 0){
	    // Codigo del hijoB - Espera SIGUSR1 de la funcion espera_hijo e imprime
	    signal(SIGUSR1, espera_padre);
	    while(1){
		pause();
	    }
	} else if(pidB > 0){
	    // Codigo del padre
	    // Esperamos en bucle SIGUSR1 del hijoA para mandarlo al hijoB
	    while(1){
		pause();
		kill(pidB, SIGUSR1);
	    }
	    wait(0);
	} else {
	    perror("Segundo fork");
	    return -1;
	}
	wait(0);
    } else {
	perror("fork");
	return -1;
    }
}
