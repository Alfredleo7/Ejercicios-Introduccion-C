/*
 * The sum of the squares of the first ten natural numbers is,
 * 1^2 + 2^2 + ... + 10^2 = 385
 *
 * The square of the sum of the first ten natural numbers is,
 * (1 + 2 + ... + 10)^2 = 55^2 = 3025
 *
 * Hence the difference between the sum of the squares of the first ten natural numbers and the square of the sum is 3025  385 = 2640.
 *
 * Find the difference between the sum of the squares of the first one hundred natural numbers and the square of the sum.
 */
#include <stdio.h>

int main(){
    int	squaresSum=0;
	int sumSquared=0;
	for(int i =0; i <= 100; i++){
		squaresSum += i*i;
		sumSquared +=i;
	}
	sumSquared = sumSquared*sumSquared;
	printf("%d is the sum of the squares.\n%d is the squared sum.\n", squaresSum, sumSquared);
	int res = sumSquared - squaresSum;
	printf("%d is the difference between the sum of the squares and the square of the sum.\n", res);
}
