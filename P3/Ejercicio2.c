/* Realice  una  variación  del  ejercicio  anterior  donde  esta  vez  el  hijo  será  el  que ponga fin a los dos procesos cuando alcance la cuenta de X caracteres leídos por 
la  pipe  (este  valor  se  puede  obtener  como  argumento  al  programa).  En  esta 
ocasión, se creará una pipe adicional para que el hijo le pueda transmitir al padre 
si éste debe continuar leyendo mensajes del usuario por teclado o de
be finalizar. 
El algoritmo es el que sigue: 
a.
Padre  lee  mensaje  de  teclado  del  usuario  y  lo  manda  al  hijo  por  la  pipe 
original.
b.
Hijo lee mensaje por la pipe original, lo muestra por pantalla y actualiza 
su  cuenta  de  bytes  leídos.  Si  se  alcanza  o  supera el 
máximo  X  entonces utiliza  la  pipe  nueva  para  enviar  al  padre  el  carácter  'n'  y termina.  Si  no se  alcanza  X,  entonces  utiliza  la  pipe  nueva  para  enviar  al  padre  el carácter 'y' y vuelve a (b).
c.
El padre lee por la pipe nueva un carácter. Si es 'y' vuelve al 
paso (a). Si es 'n', hace wait() del hijo y termina la ejecución.
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#define T 512
int main(int argc, char *argv[]){
  if(argc < 2){
    printf("Uso: %s <numeroCaracteres>\n", argv[0]);
    return -1;
  }
  // Apertura de las pipes
  int padreHijo[2], hijoPadre[2];
  if(pipe(hijoPadre) || pipe(padreHijo)){}
  // Generamos el hijo
  pid_t pid = fork();
  if(pid > 0){ // Codigo del padre
    // Cerramos los extremos de las pipes que no vamos a utilizar
    if(close(hijoPadre[1]) < 0){
      perror("close pipe");
      return -1;
    }
    if(close(padreHijo[0])){
      perror("close pipe");
      return -1;
    }
    // Leemos por teclado y mandamos al hijo
    // Primera lectura
    int leidos, escritos;
    char bufferEscritura[T], retorno[T];
    if((leidos = read(0, bufferEscritura, T)) < 0){
      perror("read");
      return -1;
    }
    int longitud = strlen(bufferEscritura);
    bufferEscritura[leidos - 1] = '\0';
    if((write(padreHijo[1], &longitud, sizeof(longitud)) != sizeof(longitud))){
      perror("write");
      return -1;
    }
    if((write(padreHijo[1], bufferEscritura, longitud) != longitud)){
      perror("write");
      return -1;
    }
    if((read(hijoPadre[0], retorno, 1)) < 0){
      perror("read");
      return -1;
    }
    // Lectura iterada hasta recibir un "n" del hijo
    while((strcmp(bufferEscritura, "fin")) != 0 && (strcmp(retorno, "n")) != 0){
      if((leidos = read(0, bufferEscritura, T)) < 0){
        perror("read");
        return -1;
      }
      int longitud = strlen(bufferEscritura);
      bufferEscritura[leidos - 1] = '\0';
      if((write(padreHijo[1], &longitud, sizeof(longitud)) != sizeof(longitud))){
        perror("write");
        return -1;
      }
      if((write(padreHijo[1], bufferEscritura, longitud) != longitud)){
        perror("write");
        return -1;
      }
      if((read(hijoPadre[0], retorno, 1)) < 0){
        perror("read");
        return -1;
    }
    }
  } else if(pid == 0){ // Codigo del hijo
    char bufferLectura[T], respuesta[T];
    // Cerramos los extremos de las pipes que no vamos a utilizar
    if(close(hijoPadre[0]) < 0){
      perror("close pipe");
      return -1;
    }
    if(close(padreHijo[1])){
      perror("close pipe");
      return -1;
    }
    int longitudRecibida, totalLeidos = 0;
    while(totalLeidos < atoi(argv[1])){
      if((read(padreHijo[0], &longitudRecibida,sizeof(longitudRecibida))) != sizeof(longitudRecibida)){
        perror("read");
        exit(-1);
      }
      if((read(padreHijo[0], bufferLectura, longitudRecibida)) != longitudRecibida){
        perror("read");
        exit(-1);
      }
      totalLeidos += longitudRecibida;
      printf("Hijo: %s\n", bufferLectura);
      if(strcmp(bufferLectura, "fin") == 0){
        break;
      }
      if(totalLeidos >= atoi(argv[1])){
        if((write(hijoPadre[1], "n", 1)) < 0){
          perror("write");
          exit(-1);
        }
      } else {
        if((write(hijoPadre[1], "y", 1)) < 0){
          perror("write");
          exit(-1);
        }
      }
    }
    // Hemos terminado de leer
    if(close(padreHijo[0]) < 0){
      perror("close pipe");
      exit(-1);
    }
    if(close(hijoPadre[1]) < 0){
      perror("close pipe");
      exit(-1);
    }
    exit(0);
  } else {
    perror("Fork");
    return -1;
  }
}
