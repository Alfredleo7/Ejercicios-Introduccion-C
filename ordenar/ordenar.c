#include <stdlib.h>
#include <stdio.h>

#define T 16 
#define MAX_PRI 256
#define MAX_PID 1024

struct ProcessInfo
{
    int prioridad;
    int pid;
};

void inicializar(struct ProcessInfo *p);
void swap(struct ProcessInfo *p1, struct ProcessInfo *p2);
void asignarValores(struct ProcessInfo *p);
void ordenar(struct ProcessInfo *p);
void mostrar(struct ProcessInfo *p);

int main()
{
    struct ProcessInfo *procesos;
    //Asignamos memoria para un array de T structs
    procesos = (struct ProcessInfo *)malloc(T * sizeof(struct ProcessInfo));
    //Inicializamos la estructura
    inicializar(procesos);
    //Asignamos valores
    asignarValores(procesos);
    //Ordenamos
    ordenar(procesos);
    //Mostramos
    mostrar(procesos);
}

void inicializar(struct ProcessInfo *p){
//Inicializamos todos los huecos con prioridad maxima y valor -1
    for(int i = 0; i < T; i++){
        p[i].prioridad = MAX_PRI;
        p[i].pid = -1;
    }
}
void swap(struct ProcessInfo *p1, struct ProcessInfo *p2){
    struct ProcessInfo *aux;
    aux = (struct ProcessInfo *)malloc(sizeof(struct ProcessInfo));
    aux->prioridad = p1->prioridad;
	aux->pid = p1->pid;
	p1->prioridad = p2->prioridad;
	p1->pid = p2->pid;
	p2->prioridad= aux->prioridad;
    p2->pid = aux->pid;

}
void asignarValores(struct ProcessInfo *p){
    for(int i = 0; i < T; i++){
        p[i].pid = rand() % MAX_PID;
		p[i].prioridad = rand() % MAX_PRI;
    }
}
void ordenar(struct ProcessInfo *p){
//Burbuja con ordenacion ascendente
for(int i = 0; i < (T - 1); i++){
    for(int j = 0; j < (T - i -1); j++){
        if(p[j].prioridad > p[j+1].prioridad){
            swap(&p[j], &p[j+1]);
        }
    }
}

}
void mostrar(struct ProcessInfo *p){
    for(int i = 0; i < (T-1); i++){
        printf("%d PRIORIDAD %d PID\n", p[i].prioridad, p[i].pid);
    }
}
