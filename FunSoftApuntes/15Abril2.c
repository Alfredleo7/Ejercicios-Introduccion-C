#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// Imprime por pantalla un fichero de texto llamando a cat

int main(int argc, char *argv[]){
    if(argc<2){
	printf("Uso: %s <fichero>\n", argv[0]);
	exit(-1);
    }

    int ret;
    ret = execl("/bin/cat", "cat", argv[1], NULL);
    if(ret < 0) {
	perror("execl");
	return -1;
    }
    printf("Esto no se va a imprimir por execl\n");
    return 0;
    
}
