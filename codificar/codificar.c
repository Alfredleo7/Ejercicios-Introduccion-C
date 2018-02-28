#include <stdio.h>
#include <unistd.h>
#define T 4
#define MAX 34
//Probando git
struct RegCod {
	int offset;
	char cad[MAX];
};
//Prototipos

int compararCadena(char c1[], char c2[]);
void codificarCadena(char c[], int offset);
void copiarCadena(char destino[], char origen[]);

int main(){
	struct RegCod registro[T];
	char cadena[MAX];
	int leidos, longitud, offset;

	printf("IIntroduzca cadena para codificar (\"fin\" para terminar): \n");
	leidos = read(0, cadena, MAX);
	cadena[leidos-1] = '\0';
	int posicionActual = 0;
	while (compararCadena(cadena, "fin") != 0){
		longitud = leidos -1;
		offset = (longitud % 2) + 1;
		codificarCadena(cadena, offset);
		printf("Cadena codificada: %s\n", cadena);
		registro[posicionActual].offset = offset;
		copiarCadena(registro[posicionActual].cad, cadena);
		posicionActual = (posicionActual + 1)%4;
		printf("Introduzca cadena para codificar (\"fin\" para terminar): \n");
		leidos = read(0, cadena, MAX);
		cadena[leidos-1] = '\0';
	}
	printf("Contenido del registro:\n");
	for(int i = 0; i < T; i++){
		printf("Registro %d\n", i);
		printf("\toffset = %d\n", registro[i].offset);
		printf("\tcodificacion = %s\n", registro[i].cad);
		}

}
int compararCadena(char c1[], char c2[]){
	int posicion = 0;
	while(c1[posicion] == c2[posicion]){
		if(c1[posicion] == '\0' || c2[posicion] == '\0'){
			break;
		}
		posicion++;
	}
	if(c1[posicion] == '\0' && c2[posicion] == '\0'){ // Las cadenas son iguales
		return 0;
	} else {
		return 1; }
}
void codificarCadena(char c[], int offset){
	int pos = 0;
	while(c[pos] != '\0'){
		if(offset == 1){	
			c[pos]+=1;
			pos++;
		} else { 
			c[pos]+=2;
			pos++;
		}
	}
}
void copiarCadena(char destino[], char origen[]){
	for(int i = 0; i < MAX; i++){
		destino[i] = origen[i];
	}	
}
