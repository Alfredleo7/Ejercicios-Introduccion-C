/*
The prime factors of 13195 are 5, 7, 13 and 29.

What is the largest prime factor of the number 600851475143 ?
*/
#include <stdio.h>
#include <stdlib.h>

int main(){
  long number = 600851475143;
  long aux = 2; 
  while(aux != number){
    if(number % aux == 0){
      printf("%ld ", aux);
      number = number / aux;
      aux = 2;
    } else { aux += 1; }
  }
  printf("%ld\n", number);
  return 0;
}
