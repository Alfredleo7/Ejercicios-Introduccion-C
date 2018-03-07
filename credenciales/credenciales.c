#include <string.h>
#include <stdio.h>
struct User {
  char *usuario;
  char *password;
};
/*
 * Comprueba si un user tiene credenciales en la base de datos.
 * formato de cred = usuario@password
 * El programa recibe las credenciales como argumento tal como:
 * ./credenciales paco@luna123
 */
int comprobarCredenciales(char *cred, struct User *db, int dbSize);

int main(int argc, char *argv[]){
/* 1- Comprobar argumentos.
 * La condicion que aparece abajo comprueba que hay argumentos a parte
 * de ./credenciales. Si no los hay explica el uso del programa y devuelve -1
 */
  if (argc < 2){
    printf("Error! Uso: %s <usuario@password>", argv[0]);
    return -1;
  }
/* 2- Simulamos la base de datos */
  struct User database[] ={
    {"paco","luna"},
    {"juan","juan123"},
    {"mari","luz"}};
/* 3- Comprobamos que las credenciales facilitadas coinciden e imprimimos
 * el resultado de la comprobacion. */
  int resultado = comprobarCredenciales(argv[1],database,3);
  printf("resultado = %d\n",resultado);

}
int comprobarCredenciales(char *cred, struct User *db, int dbSize){
  /* 1- Buscamos @. El puntero declarado a continuacion utiliza la funcion
   * strstr() para buscar la cadena "@" en cred y apunta a donde esta alojado */
  char *p = strstr(cred, "@");
  if(p == NULL){ // No se ha encontrado @ en cred
    return 0;
  } else {
    *p = '\0'; // Sustituimos @ por \0
    char *usuario = cred;
    char *password = p+1;
  /* 2- Buscamos usuario y password dentro de db */
    for(int i = 0; i < dbSize; i++){
    int res1 = strcmp(db[i].usuario, usuario);
    int res2 = strcmp(db[i].password, password);
    if(res1 == 0 && res2 == 0){
      return 1;
    }
    }
  }
  return 0;
}
