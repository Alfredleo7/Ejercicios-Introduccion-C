/*
By listing the first six prime numbers: 2, 3, 5, 7, 11, and 13, we can see that the 6th prime is 13.

What is the 10 001st prime number?
*/
#include <stdlib.h>
#include <stdio.h>
#define MAX 10001
int main(){
	int current = 2;
	int primePos = -1;
	while(primePos <= MAX){
		int isPrime = 1;
		for(int i = 2; i < current/2; i++){
			if(current%i==0){
				isPrime = 0;
			}
		}
		if(isPrime == 1){
			primePos+=1;
			if(primePos==MAX){
				printf("%d is the 10001st prime\n", current);
				exit(0);
			}
		}
		current +=1;
	}
return 0;
}
