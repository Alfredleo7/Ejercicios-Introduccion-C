#include <stdio.h>
#include <ctype.h>
#include <string.h>
int esVocal(char c);
void eliminarVocales(char *c);

int main(int argc, char *argv[]){
  if(argc < 4){
    printf("Uso: %s c = \"<sucadena>\"\n", argv[0]);
    return -1;}
  char *c = argv[3];
  eliminarVocales(c);
  printf("c = %s\n", c);
  return 0;
}

int esVocal(char c){
  int esVocal = (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
  return esVocal;
}
void eliminarVocales(char *c){
  for(int i = 0;c[i]!='\0'; i++){
    if(esVocal(c[i])){
      for(int j = i;c[j]!='\0'; j++){
        c[j] = c[j+1];
      }
    i -=1;
    }
  }
}
