#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  int val;
  struct node *next;
} node_t;
//Funcion para anadir al final de la lista
void addEnd(node_t *head, int val);
//Funcion para anadir al principio de la lista
void addStart(node_t **head, int val);
//Funcion para quitar al principio que nos devuelve el elemento que hemos borrado
int pop(node_t **head);
//Funcion para imprimir la lista
void print(node_t *head);



int main(){
  node_t *head = NULL;
  head = malloc(sizeof(node_t));
  if(head == NULL){ return 1; };
  head -> val = 1;
  head -> next = malloc(sizeof(node_t));
  head -> next -> val = 2;
  head -> next -> next = NULL;
  addStart(&head, 2);
  addEnd(head, 3);
  printf("Cuando haces pop no hay stop %d\n", pop(&head));
  print(head);
}

//Funcion para anadir al final de la lista
void addEnd(node_t *head, int val){
  node_t *current = head;
  while(current->next != NULL){
    current = current->next;
  }
  //Ahora ya estamos en el ultimo nodo
  current->next = malloc(sizeof(node_t));
  current->next->val = val;
  current->next->next = NULL;
}
//Funcion para anadir al principio de la lista
void addStart(node_t  **head, int val){
  node_t *nuevo_nodo;
  nuevo_nodo = malloc(sizeof(node_t));
  nuevo_nodo -> val = val; //Le asignamos su valor
  nuevo_nodo -> next = *head; //Asignamos a next un puntero a donde esta head
  *head = nuevo_nodo;//Ahora head es el nuevo nodo
}
//Funcion para imprimir la lista
void print(node_t *head){
  node_t *current = head;
  while(current != NULL){
    printf("%d\n",current->val);
    current = current->next;
  }
}
//Funcion para borrar al principio que nos devuelve el elemento borrado
int pop(node_t **head){
  int retval = -1; //Iniciamos el valor que se devuelve
  node_t *siguiente_nodo = NULL;
  //Si la lista esta vacia devolveremos -1
  if(*head == NULL){
    return -1;
  }
  siguiente_nodo = (*head)->next;//Nos movemos al segundo nodo
  retval = (*head)->val;//Guardamos el valor del primero para devolverlo
  free(*head); //Liberamos la memoria asignada a head
  *head = siguiente_nodo;//Asignamos a head el segundo nodo
  return retval;//Devolvemos el valor que hemos eliminado
}
