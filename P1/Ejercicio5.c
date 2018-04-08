#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
int estadof(char *f);

int main(int argc, char *argv[]){
    int res = estadof(argv[1]);
    return 0;
}

int estadof(char *f){
    struct stat archivo;
    // Comprobamos su size
    if((stat(f, &archivo)) < 0){
	printf("ERROR!\n");
	return -1;
    }
    printf("Size: %ld\n", archivo.st_size);
    // Comprobamos si es un directorio o no
    if(S_ISDIR(archivo.st_mode) != 0){
	printf("Directory.\n");
    } else {
	printf("File.\n");
    }
    // 
    return 0;
}
