#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#define T 256
char msg[T];
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
/*_____ _____  _      ______ 
 |_   _|  __ \| |    |  ____|
   | | | |  | | |    | |__   
   | | | |  | | |    |  __|  
  _| |_| |__| | |____| |____ 
 |_____|_____/|______|______|
*/
      switch(estado){
      case IDLE:
	  strcpy(msg, "Sistema de refrigeracion apagado (IDLE)\n");
	  write(1, msg, strlen(msg));
	  switch(evento){
	  case TEM_ALTA:
	      estado = NORMAL;
	      // Set time
	      strcpy(msg, "Temporizador restablecido\n");
	      write(1,msg, strlen(msg));
	      eventoRecibido = TEM_ALTA;
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
	  strcpy(msg, "Sistema de refrigeracion en modo normal\n");
	  write(1, msg, strlen(msg));
	  switch(evento){
	  case TEM_ALTA:
	      // Set timer
	      strcpy(msg, "Temporizador restablecido\n");
	      write(1,msg, strlen(msg));
	      if((setitimer(ITIMER_REAL, &t_normal, NULL))< 0){
		  perror("setitimer");
		  exit(1);
	      }
	      break;
	  case TEMP_EXTREMA:
	      estado = FULL;
	      // Set timer con el timepo t_full
	      strcpy(msg, "Temporizador restablecido\n");
	      write(1,msg, strlen(msg));
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
	  strcpy(msg, "Sistema de refrigeracion encendido a potencia maxima\n");
	  write(1, msg, strlen(msg));
	  switch(evento){
	  case TEM_ALTA:
	      estado = FULL;
	      strcpy(msg, "Temporizador restablecido\n");
	      write(1,msg, strlen(msg));

	      if((setitimer(ITIMER_REAL, &t_full, NULL)) < 0){
		  perror("setitimer");
		  exit(1);
	      }
	      break;
	  case TEMP_EXTREMA:
	      estado = FULL;
	      strcpy(msg, "Temporizador restablecido\n");
	      write(1,msg, strlen(msg));
	      if((setitimer(ITIMER_REAL, &t_full, NULL)) < 0){
		      eventoRecibido = TEM_ALTA;
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
    strcpy(msg, "#Llega SIGUSR1\nNivel de alerta 1\n");
    eventoRecibido = TEM_ALTA;
    signal(SIGUSR1, temperaturaElevada);
    write(1,msg, strlen(msg));
}
void temperaturaExtrema(int signo){
    strcpy(msg, "#Llega SIGUSR2\nNivel de alerta 2\n");
    eventoRecibido = TEMP_EXTREMA;
    signal(SIGUSR2, temperaturaExtrema);
    write(1,msg, strlen(msg));
}
void timeout(int signo){
    strcpy(msg, "Temporizador expirado\n");
    write(1,msg, strlen(msg));
    signal(SIGALRM, timeout);
    eventoRecibido = TIMEOUT;
}
int espera_evento(){
    pause();
    return eventoRecibido;
}

