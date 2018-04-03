#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>

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
  strcut timeval gap2; // 3.5s
  gap2.tv_sec = 3;
  gap2.tv_usec = 500000;

  t normal.it value = gap1;
  t normal.it interval = zero;

  t full.it value = gap2;
  t full.it interval = zero;

  // Registramos los manejadores
  signal(SIGUSR1, temperaturaElevada);
  signal(SIGUSR2, temperaturaExtrema);
  signal(SIGALRM, timeout);

  // Implementacion de la maquina de estados
  estado = IDLE;
  while(1){
    evento = espera_evento();
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

       case FULL:
	 switch(evento){
	 case TEM_ALTA:
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
    default:
      printf("Estado no esperado: %d\n", estado);
      return -1;
    }
  }
}

void temperaturaElevada(int signo){
  eventoRecibido = TEMP_ALTA;
  signal(SIGUSR1, temperaturaElevada);
}
void temperaturaExtrema(int signo);
void timeout(int signo);
int espera_evento(){
  pause();
  return eventoRecibido;
}
