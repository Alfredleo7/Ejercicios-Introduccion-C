#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

// Definicion de estados y eventos

#define IDLE 0
#define NORMAL 1
#define FULL 2

#define TEM_ALTA 10
#define TEMP_EXTREMA 11
#define TIMEOUT 12

int eventoRecibido;


int espera_evento();
void temperaturaElevada(int signo);
void temperaturaExtrema(int signo);
void timeout(int signo);
void imprimeEstado(int estado, int evento);

int main(int argc, char *argv[]){
    printf("%d es el pid\n", getpid());
    // Variables
    int estado, evento;
    // Temporizadores
    struct itimerval t_normal, t_full;
    struct timeval zero;
    zero.tv_sec = 0;
    zero.tv_usec = 0;

    struct timeval gap1; // 2.2s
    gap1.tv_sec = 2;
    gap1.tv_usec = 200000;
    struct timeval gap2; // 3.5s
    gap2.tv_sec = 3;
    gap2.tv_usec = 500000;

    t_normal.it_value = gap1;
    t_normal.it_interval = zero;

    t_full.it_value = gap2;
    t_full.it_interval = zero;

    // Registramos los manejadores
    signal(SIGUSR1, temperaturaElevada);
    signal(SIGUSR2, temperaturaExtrema);
    signal(SIGALRM, timeout);

    // Implementacion de la maquina de estados
    estado = IDLE;
    while(1){
	evento = espera_evento();
	imprimeEstado(estado, evento);
/*_____ _____  _      ______ 
 |_   _|  __ \| |    |  ____|
   | | | |  | | |    | |__   
   | | | |  | | |    |  __|  
  _| |_| |__| | |____| |____ 
 |_____|_____/|______|______|
*/
      switch(estado){
      case IDLE:
	  switch(evento){
	  case TEM_ALTA:
	      estado = NORMAL;
	      // Set timer
	      if((setitimer(ITIMER_REAL, &t_normal, NULL))< 0){
		  perror("setitimer");
		  exit(1);
	      }
	      break;
	  case TEMP_EXTREMA:
	      break;
	  case TIMEOUT:
	      break;
	  default:
	      printf("Evento no esperado %d\n", evento);
	      return -1;
	  }
	  break;
/*_   _  ____  _____  __  __          _      
 | \ | |/ __ \|  __ \|  \/  |   /\   | |     
 |  \| | |  | | |__) | \  / |  /  \  | |     
 | . ` | |  | |  _  /| |\/| | / /\ \ | |     
 | |\  | |__| | | \ \| |  | |/ ____ \| |____ 
 |_| \_|\____/|_|  \_\_|  |_/_/    \_\______|
*/
      case NORMAL:
	  switch(evento){
	  case TEM_ALTA:
	      // Set timer
	      if((setitimer(ITIMER_REAL, &t_normal, NULL))< 0){
		  perror("setitimer");
		  exit(1);
	      }
	      break;
	  case TEMP_EXTREMA:
	      estado = FULL;
	      // Set timer con el timepo t_full
	      if((setitimer(ITIMER_REAL, &t_full, NULL))< 0){
		  perror("setitimer");
		  exit(1);
	      }  
	      break;
	  case TIMEOUT:
	      estado = IDLE;
	      if((setitimer(ITIMER_REAL, &t_normal, NULL))< 0){
		  perror("setitimer");
		  exit(1);
	      }
	      break;
	  default:
	      printf("Evento no esperado %d\n", evento);
	      return -1;
	  }
	  break;
/*______ _    _ _      _      
 |  ____| |  | | |    | |     
 | |__  | |  | | |    | |     
 |  __| | |  | | |    | |     
 | |    | |__| | |____| |____ 
 |_|     \____/|______|______|
*/
      case FULL:
	  switch(evento){
	  case TEM_ALTA:
	      estado = FULL;
	      if((setitimer(ITIMER_REAL, &t_full, NULL)) < 0){
		  perror("setitimer");
		  exit(1);
	      }
	      break;
	  case TEMP_EXTREMA:
	      estado = FULL;
	      if((setitimer(ITIMER_REAL, &t_full, NULL)) < 0){
		  perror("setitimer");
		  exit(1);
	      }
	      break;
	  case TIMEOUT:
	      estado = NORMAL;
	      if((setitimer(ITIMER_REAL, &t_full, NULL)) < 0){
		  perror("setitimer");
		  exit(1);
	      }
	      break;
	  default:
	      printf("Evento no esperado %d\n", evento);
	      return -1;
	  }
	  break;
      default:
	  printf("Estado no esperado: %d\n", estado);
	  return -1;
      }
  }
}

void temperaturaElevada(int signo){
    eventoRecibido = TEM_ALTA;
    signal(SIGUSR1, temperaturaElevada);
}
void temperaturaExtrema(int signo){
    eventoRecibido = TEMP_EXTREMA;
    signal(SIGUSR2, temperaturaExtrema);
}
void timeout(int signo){
    signal(SIGALRM, timeout);
    eventoRecibido = TIMEOUT;
}
int espera_evento(){
    pause();
    return eventoRecibido;
}

void imprimeEstado(int estado, int evento){
    printf("Estado %d Evento %d \n", estado, evento);
}
