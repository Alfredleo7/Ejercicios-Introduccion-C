/*
 * A palindromic number reads the same both ways. The largest palindrome made from the product of two 2-digit numbers is 9009 = 91  99.
 *
 * Find the largest palindrome made from the product of two 3-digit numbers.
 */



#include <stdio.h>
#include <stdlib.h>

int isPalindrome(int originalNumber);

int main(){
int max = 0;
	for(int i=999; i > 100; i--){
		for(int j=999; j > 100; j--){
				int mul = i*j;
				if(isPalindrome(mul) && mul > max){ max = mul;}
			}
		} 
printf("%d is the max palindrome.", max);
return 0;
}
int isPalindrome(int originalNumber){
	int n = originalNumber;
	int remainder;
	int new = 0;

	while(n!=0){
		remainder = n%10;
		new = new*10+remainder;
		n /= 10;
	}
	if(new == originalNumber){return 1;}
	return 0;
}
