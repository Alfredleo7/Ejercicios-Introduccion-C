#include <stdio.h>
int main(){
    int actual;
    int MAX = 10;
    int frecuencia[MAX];
    for(int i = 0; i < MAX; i++){
        frecuencia[i] = 0;
    }
printf("Introduzca una secuencia de números entre 0 y 9 terminada en un número negativo: ");
    scanf(" %d", &actual);
    while(actual>=0){
        frecuencia[actual]++;
        scanf("%d", &actual);
    }
    for(int i = 0; i < MAX; i++){
        printf("%d ",i);
        for (int j = 0; j < frecuencia[i]; j++){
            printf("*");
        }
        printf("\n");
    }
    return 0;
}