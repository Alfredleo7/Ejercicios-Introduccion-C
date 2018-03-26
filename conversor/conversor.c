#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KRED "\x1B[31m"
#define KRESET "\x1B[0m"
void strrev(char *str);
void decToBin(int dec);
void decToOct(int dec);
void decToHex(int dec);
int binToDec(char *bin);
// TODO - Añadir entrada para octal y hex, con sus respectivas funciones
int main(int argc, char *argv[]){
	if(argc < 3 || *argv[1] != '-'){
		printf("Error! Formato de entrada: %s -<notacion> <numero>\n", argv[0]);
		exit(-1);
	}
	switch(*(argv[1]+1)) {
		case 'd':
			decToBin(atoi(argv[2]));
			decToOct(atoi(argv[2]));
			decToHex(atoi(argv[2]));
			break;
		case 'b':
			printf(KRED "Decimal: %d\n" KRESET,binToDec(argv[2]));
			decToOct(binToDec(argv[2]));
			decToHex(binToDec(argv[2]));
			break;
	}	
	return 0;
}
void strrev(char *str){
	char *start = str; // Apuntamos al inicio
	char *end = str + strlen(str)-1; // Apuntamos al final
	char temp;	
	while(end > start){
		temp = *start;
		*start = *end;
		*end = temp;
		start++;
		end--;
	}
	}

void decToBin(int dec){
	char binario[100];
	int pos = 0;
	while(dec>0){
		if(dec % 2 == 0){
			binario[pos]='0';
		} else {
			binario[pos]='1';
		}
		pos++;
		dec/=2;
	}	
	binario[pos] = '\0';
	strrev(binario);
	printf(KRED "Binario: %s\n"KRESET, binario);
}
void decToOct(int dec){
	char oct[100];
	int pos = 0;
	while(dec>0){
		oct[pos]='0'+dec%8;
		pos++;
		dec/=8;
	}
	oct[pos]='\0';
	strrev(oct);
	printf(KYEL "Octal: %s \n" KRESET,oct);
}

void decToHex(int dec){
	char hex[100];
	int pos = 0;
	while(dec>0){
		int value = dec%16;
		switch(value){
			case 15:
				hex[pos]='F';
				break;
			case 14:
				hex[pos]='E';
				break;
			case 13:
				hex[pos]='D';
				break;
			case 12:
				hex[pos]='C';
				break;
			case 11:
				hex[pos]='B';
				break;
			case 10:
				hex[pos]='A';
				break;
			default:
				hex[pos]='0'+dec%16;
				break;
		}
		pos++;
		dec/=16;
	}
	hex[pos]='\0';
	strrev(hex);
	printf(KGRN"Hex: %s \n"KRESET,hex);
}
int binToDec(char *bin){
	int dec = 0;
	for(int i = 1; i<=strlen(bin);i++){
		if(bin[strlen(bin)-i]=='1'){
		dec += pow(2, i-1);
		}
	}
	return dec;
}
