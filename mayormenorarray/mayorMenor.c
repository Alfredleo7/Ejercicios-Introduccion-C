#include <stdio.h>
int main(){
    int actual;
    int menor = 0;
    int mayor = 0;
    printf("Introduzca una secuencia de numeros terminada en cero (0): ");
    scanf(" %d", &actual);
    menor = actual;
    mayor = actual;
    while(actual!=0){
        if(actual<menor){
            menor = actual;
        }
        if(actual > mayor){
            mayor = actual;
        }
        scanf("%d", &actual);
    }
    if(menor == 0 && mayor == 0){
        printf("Secuencia vacia");
        return 0;
    }
    printf("Menor: %d; Mayor: %d\n", menor, mayor);
    return 0;
}