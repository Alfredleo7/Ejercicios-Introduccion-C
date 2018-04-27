#include <string.h>
#include <sys/wait.h>

#define SIZE 512
int main(){
    // 1.Definimos las pipes
    int a[2]; // Pipe padre -> hijo
    int b[2]; // Pipe hijo -> padre
    // 2.Creamos las pipes
    if(pipe(a) < 0){
	    perror("Close");
	    exit(-1);
    }
    if(pipe(b) < 0){
	    perror("Close");
	    exit(-1);
    }
    // 3.Clonamos
    
    pid_t pid = fork();
    if(pid == 0){
	// Codigo del hijo
	// Cerramos las pipes que no usamos
	if(close(a[1]) < 0){
	    perror("Close");
	    exit(-1);
	}
	if(close(b[0]) < 0){
	    perror("Close");
	    exit(-1);
	}
	// Leer dos cadenas del padre

	// Concatenamos las cadenas

	// Se la enviamos al padre

	// Cerramos las pipes

	// Salimos con exit para notificar al padre

    } else if(pid > 0){
	// Codigo del padre
	// Cerramos las pipes que no usamos
	if(close(b[1]) < 0){
	    perror("Close");
	    return -1;
	}
	if(close(a[0]) < 0){
	    perror("Close");
	    return -1;
	}
	// Leemos de teclado
	char w1[SIZE];
	char w2[SIZE];

	printf("Cadena 1:\n");
	int leidos = read(0, w1, SIZE);
	w1[leidos-1] = '\0';
	printf("Cadena 2:\n");
	int leidos = read(0, w1, SIZE);
	w2[leidos-1] = '\0';

	// Envio protegido de w1 y w2
	int longitud;
	longitud = strlen(w1);

	if((write_n(a[1], &l, sizeof(longitud))) != sizeof(longitud)){
	    perror("write");
	    exit(-1);
	}
	if((write_n(a[1], w1, longitud)) != longitud){
	    perror("write");
	    exit(-1);
	}

	// Cerrar la pipe de escritura a[1]
	if(close(a[1])){
	    perror("close");
	    exit(-1);
	}

	// Leer la cadena de nuestro hijo por b[0] y la imprimimos

	// Cerramos la pipe de lectura b[0]

	// Hacemos wait()
    } else {
	perror("fork");
	return -1;
    }
}
