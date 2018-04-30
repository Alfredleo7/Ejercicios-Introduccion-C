/* Este programa copia el archivo de origen en el archivo de destino,
 * copiando de T bytes en T bytes. Para iniciar la copia debe recibir
 * una señal SIGUSR1. Esto inicia un alarm de S segundos antes de copiar
 * los T bytes. No puede recibir otras señales mientras esta esperando a la
 * alarma. */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#define T 8
#define S 1 // Tiempo entre que recibimos la señal y copiamos

// Variables globales
int fd_destino;
char buffer[T];
int leidos;

void manejador_alarm(int signo);
void manejador_sigusr1(int signo);

int main(int argc, char *argv[]){
    // 1. Procesar argumentos
    printf("Mi PID es: %d\n", getpid());
    if (argc < 3){
        printf("Error! Uso: %s <nombredeorigen> <nombrededestino>\n", argv[0]);
        exit(-1);
    }
    // 2. Registrar manejadores
    signal(SIGALRM, manejador_alarm);
    signal(SIGUSR1, manejador_sigusr1);
    
    // 3. Abrir ficheros
    int fd_origen;
    if((fd_origen = open(argv[1], O_RDONLY)) < 0){
        perror("Open origen");
        return -1;
    }
    if((fd_destino = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC))<0){
        perror("Open destino");
        return -1;
    }
    // 4. Iniciamos la copia
    leidos = read(fd_origen, buffer, T);
    while(leidos>0){
        printf("Esperando para write...\n");
        pause(); // Espera a SIGUSR1
        pause(); // Espera a SIGALARM

        leidos = read(fd_origen, buffer, T);
    }
    if(leidos<0){
        perror("Read");
        close(fd_destino);
        close(fd_origen);
        exit(-1);
    }
    return 0; 
}

void manejador_alarm(int signo){
    if(write(fd_destino, buffer, leidos) < 0){
        perror("Error Write");
        exit(-1);
    }
    printf("Bloque copiado\n");
    signal(SIGALRM, manejador_alarm);
    signal(SIGUSR1, manejador_sigusr1);
}

void manejador_sigusr1(int signo){
    alarm(S);
    printf("Temporizador conectado... \n");
    signal(SIGUSR1, SIG_IGN); // No podemos recibir la señal de copiar mientras esperamos
}
