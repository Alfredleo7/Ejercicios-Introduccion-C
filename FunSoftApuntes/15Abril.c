#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
void espera_hijo(){
    wait(0);
    printf("Hijo esperado...\n");
    signal(SIGCHLD, espera_hijo);
}
int main(int argc, char *argv[]){
    pid_t pid = fork();
    signal(SIGCHLD, espera_hijo);
    int v = 10;
    if(pid == 0){
	// Codigo del hijo
	v = 5;
	//while(1);
	exit(0);
    } else if (pid > 0){
	// Codigo del padre
	//while(1);
	wait(NULL);
    } else {
	perror("fork");
	return -1;
    }
    printf("Terminado\n");
    pause();
    return 0;
}
