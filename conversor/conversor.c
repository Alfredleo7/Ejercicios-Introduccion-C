#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
void strrev(char *str);
void decToBin(int dec);
void decToOct(int dec);
void decToHex(int dec);
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
	printf("Binario: %s\n", binario);
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
	printf("Octal: %s \n",oct);
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
	printf("Hex: %s \n",hex);
}
