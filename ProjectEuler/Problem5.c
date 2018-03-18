/*
 * 2520 is the smallest number that can be divided by each of the numbers from 1 to 10 without any remainder.
 *
 * What is the smallest positive number that is evenly divisible by all of the numbers from 1 to 20?
 */

#include <stdio.h>
#include <stdlib.h>

int main(){
	int min = 21;
	while(1){
		for(int i = 1; i<=20; i++){
			if(min%i!=0){break;}
			if(i == 20){
					printf("%d is the smallest positive number evenly divisible by 1 to 20\n", min);
					exit(1);
			}
		}
	min++;
	}		
return 0;
}
