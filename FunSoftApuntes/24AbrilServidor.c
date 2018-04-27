void procesarFifo(int fifo, int fd, int *conectados, char *prefijo);
int main(int argc, char *argv[]){
    // Procesar argumentos(nombres de las FIFOs) con las que nos conectamos a
    // los clientes.
    if(argc < 3){
	printf("Uso.... \n");
	return -1;
    }

    // Apertura de FIFOs y ficheros
    int fifo1, fifo2, fd;

    if((fifo1 = open(argv[1], O_RDONLY)) < 0){
	perror("open");
	return -1;
    }
    if((fifo2 = open(argv[2], O_RDONLY)) < 0){
	perror("open");
	return -1;
    }
    if((fd = open("logs.txt", O_WRONLY | O_CREAT | O_TRUNC)) < 0){
	perror("open");
	return -1;
    }

    // Iteramos mientras haya clientes conectados
    int conectados = 2;
    while(conectados > 0){
	// Leer fifo1

	// Escribir en fichero con el prefijo
	procesarFifo(fifo1, fd, &conectados, "Cliente 1: ");
	// Leer fifo2

	// Escribir en fichero con el prefijo
	procesarFifo(fifo1, fd, &conectados, "Cliente 2: ");
    }

    // Cerramos FIFOs y fichero
    return 0;
}
void procesarFifo(int fifo, int fd, int *conectados, char *prefijo){
    // Leemos de la fifo
    int leidos, longitud;
    char buffer[T];
    // Leemos la longitud del mensaje
    leidos = read_n(fifo, &longitud, sizeof(int));
    // Que valores puede devolver leer de una FIFO?
    if(leidos < 0){
	perror("read n longitud");
	exit(-1);
    } else if(leidos == 0) { // El otro extremo ha desconectado, asi q leemos 0
	(*conectados)--;
    } else if(leidos != sizeof(int)){ // No hemos leido la longitud correctamente
	perror("read n longitud");
	exit(-1);
    } else {
	// Estoy seguro de haber leido la longitud de la cadena
	leidos = read_n(fifo, buffer, longitud);
	if(leidos < 0){
	    
	} else if (leidos != longitud){

	} else if (leidos == 0){

	} else {
	    // He recibido bien la cadena, por lo que escribo
	    int l = strlen(prefijo);
	    if(write_n(fd, prefijo, l) != l){
		perror("write");
		exit(-1);
	    }

	    if(write_n(fd, buffer, longitud) != longitud){
		perror("write");
		exit(-1);
	    }

	    if(write_n(fd, "\n", 1) != 1){
		perror("write");
		exit(-1);
	    }
	}
    }
    
}
