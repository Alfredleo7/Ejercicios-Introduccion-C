
# Table of Contents

1.  [Tema 3: Comunicaciones en red](#orgee4ead1)
    1.  [Introduccion:](#orgf90fccc)
        1.  [Direcciones IP y Puertos:](#org9283a51)
        2.  [Tipos de enteros (std C99):](#orgd34b6dc)
        3.  [Tipos de arquitectura:](#org11bfed9)
    2.  [Sockets:](#org93ca994)
        1.  [Tipos:](#org6747746)
        2.  [Direcciones:](#org2a45751)


<a id="orgee4ead1"></a>

# Tema 3: Comunicaciones en red


<a id="orgf90fccc"></a>

## Introduccion:


<a id="org9283a51"></a>

### Direcciones IP y Puertos:

-   Direccion IP: Nos sirve como direccion de la maquina.
    -   Se compone de 4 numeros de 8 bits, formando un total de
        32 bits.
        = 192.168.1.1
-   Puertos: Identifica el proceso dentro de la maquina que 
    atiende a las peticiones.
    -   Son de 16 bits.
    -   Sus valores van desde 0 hasta 65535.
    -   Identifican al proceso, ya que **dos procesos no pueden**
        **escuchar el mismo puerto**.
    -   Algunos estan predefinidos por el sistema.
        -   8080: Servidor web.
        -   21: FTP.

Al tener ambos procesos en distintas maquinas, pueden surgir
conflictos:

-   Un int en la maquina A puede valer 4 bytes y en la maquina B
    8 bytes.


<a id="orgd34b6dc"></a>

### Tipos de enteros (std C99):

Para evitar problemas de compatibilidad evitamos usar tipos como
int, long, short, etc&#x2026;

-   **#include <stdint.h>**:
    -   Nos da tipos de enteros estandar como:
        -   uint8<sub>t</sub> / int8<sub>t</sub>
        -   uint16<sub>t</sub> / int16<sub>t</sub>
        -   uint32<sub>t</sub> / int32<sub>t</sub>
        -   uint64<sub>t</sub> / int64<sub>t</sub>
-   **#include <inttypes.h>**:
    -   Nos permite imprimir los tipos de enteros estandar:
        
            uint64_t v = 6148914690091192593;
            printf("Valor = %"PRIu64" \n";


<a id="org11bfed9"></a>

### Tipos de arquitectura:

Existen los tipos **big-endian** y **little-endian** (network-byte-order).

Si no se especifica lo contrario, los datos de mas de 1 byte se
transforman a big-endian al transmitirse.
Al recibirlo, el destino transforma de big-endian a su formato
(host-byte-order)

1.  Representacion en memoria:

    Vamos a representar el numero hexadecimal 0x0A0B0C0D. Es un **ejemplo**.
    
    <table border="2" cellspacing="0" cellpadding="6" rules="groups" frame="hsides">
    
    
    <colgroup>
    <col  class="org-left" />
    
    <col  class="org-right" />
    
    <col  class="org-right" />
    
    <col  class="org-right" />
    
    <col  class="org-right" />
    </colgroup>
    <thead>
    <tr>
    <th scope="col" class="org-left">&#xa0;</th>
    <th scope="col" class="org-right">0x1100</th>
    <th scope="col" class="org-right">0x1101</th>
    <th scope="col" class="org-right">0x1102</th>
    <th scope="col" class="org-right">0x1103</th>
    </tr>
    </thead>
    
    <tbody>
    <tr>
    <td class="org-left">big - endian</td>
    <td class="org-right">0x0A</td>
    <td class="org-right">0x0B</td>
    <td class="org-right">0x0C</td>
    <td class="org-right">0x0D</td>
    </tr>
    
    
    <tr>
    <td class="org-left">little - endian</td>
    <td class="org-right">0x0D</td>
    <td class="org-right">0x0C</td>
    <td class="org-right">0x0B</td>
    <td class="org-right">0x0A</td>
    </tr>
    </tbody>
    </table>
    
    La mayor parte de procesadores de 16, 32 y 64 bits no permiten almacenar
    palabras en cualquier offset de memoria.
    
    Como resultado de esto **todo struct se ha de enviar campo a campo, nunca entero**.

2.  Funciones de conversion:

    <table border="2" cellspacing="0" cellpadding="6" rules="groups" frame="hsides">
    
    
    <colgroup>
    <col  class="org-left" />
    
    <col  class="org-left" />
    </colgroup>
    <tbody>
    <tr>
    <td class="org-left">h</td>
    <td class="org-left">host-byte-order</td>
    </tr>
    
    
    <tr>
    <td class="org-left">n</td>
    <td class="org-left">network-byte-order</td>
    </tr>
    
    
    <tr>
    <td class="org-left">s</td>
    <td class="org-left">short (16 bytes)</td>
    </tr>
    
    
    <tr>
    <td class="org-left">l</td>
    <td class="org-left">long (32 bytes)</td>
    </tr>
    </tbody>
    </table>
    
        uint16_t htons(uint16_t); /* Transforma del short de la maquina al short estandar para red */
        
        uint16_t ntohs(uint16_t); /* Transforma del short de estandar para red al short de la maquina */
        
        uint16_t htons(uint16_t); /* Transforma del long de la maquina al long estandar para red */
        
        uint32_t ntohs(uint16_t); /* Transforma del long del estandar de red al slong de la maquina*/
        
        /* Ejemplo */
        uint16_t datoParaEnviar = 0xFF00;
        uint16_t datoEnBigEndian = htons(datoParaEnviar);     


<a id="org93ca994"></a>

## Sockets:

Son puntos de comunicacion por los cuales un proceso puede enviar
o recibir info a/desde otro proceso en una red.  
*Esto se hace por un canal bidireccional entre dos procesos y lo proporciona el SO.*

Son un conjunto de funciones para utilizar la capa de transporte (Nivel 4) de
TCP/IP. Es un estandar compartido entre todos los SO.


<a id="org6747746"></a>

### Tipos:

1.  UDP:

    Se utilizan para intercambiar mensajes completos, pero sin garantizar fiabilidad.
    Puede que la red pierda o desordene alguno de los mensajes.
    
    -   No hace falta ninguna conexion previa con el destino, el mensaje se envia a su
        direccion utilizando los servicios de transporte (pero no hay garantia de si 
        se recibira este o no).
    -   Por cada nuevo envio, se tiene que indicar la direccion del destino.
    -   Por un mismo socket se puede enviar informacion a distintos destinatarios.
    -   Por un mismo socket se puede recibir informacion de distintos emisores (es un buzon).
        -   Utilizar un socket **UDP** es similar a **mandar una carta**.

2.  TCP:

    Se utiliza para intercambiar secuencias de bytes de forma fiable. No hay que manejar
    errores introducidos por la red.
    
    -   El servidor tiene que estar esperando una solicitud de conexion.
    -   Solo hay que **indicar la direccion del destinatario al realizar la conexiond** (una sola vez).
    -   Un socket solo sirve como canal bidireccional entre dos procesos que estan conectados
        (siempre los mismos mientras dure la conexion entre ellos).
        -   Un socket **TCP** es similar a realizar una **llamada telefonica**.


<a id="org2a45751"></a>

### Direcciones:

Las aplicaciones que utilizan sockets necesitan poder especificar que IP y que puertos
van a utilizar para comunicarse. Para esto tenemos el struct generico **sockaddr**.

    struct sockaddr{
    sa_family_t sa_family; /* Familia del socket: AF_XXX */
    uint8_t sa_data[14]; /* La direccion */
    };

Este struct nos permite indicar el tipo de direccion en el campo sa<sub>family</sub>.

-   *Si el valor de esta es AF<sub>INET</sub> (el que usaremos), el struct contendra una IP*
    *version 4 y un puerto*

Las funciones del socket aceptan el tipo generico sockaddr. Por comodidad utilizaremos
**sockaddr<sub>in</sub>**, ya que viene con los campos listos para escribir direcciones IPv4 y puerto.

    struct in_addr {
    uint32_t s_addr; /* Direccion IP */
    };
    struct sockaddr_in{
    sa_family_t sin_family; /* La familia, que es AF_INET */
    uint16_t sin_port; /* Numero del puerto */
    struct in_addr sin_addr; /* Direccion IP */
    uint8_t sin_zero[8]; /* Relleno para cumplir tabla de datos, no se usa */
    };

**Ejemplo**:

    /* Pasos:
     * 1. Declarar la direccion
     * 2. Asignarle valores
     * 3. Declarar la IP (4 x 8 bytes)
     * 4. Pasar la IP a la direccion
     * 5. Castear para obtener la direccion generica que usaremos con socket */
    
    /* Declaramos la direccion */
    struct sockaddr_in dir_socket;
    /* Inicializamos la direccion */
    memset(&dir_socket, 0, sizeof(dir_socket));
    /* Asignamos el valor de la familia */
    dir_socket.sin_family = AF_INET;
    /* Transformamos la direccion del socket a network-byte-order */
    dir_socket.sin_port = htons(80);
    /* Declaramos un array de 4 ints de 8 bytes, segun el estandar */
    uint8_t dir_IP[4] = {198, 168, 3, 1};
    /* Pasamos la direccion IP a dir_socket */
    memcpy(&dir_socket.sin_addr, dir_IP, 4);
    /* Por ultimo obtenemos la direccion generica, que es lo que entrara
     * en las funciones de socket. Esta se obtiene haciendo un casting */
    struct sockaddr * dir_generica = (struct sockaddr *)&dirsocket;

-   Dado que hay problemas de compatibilidad entre sistemas, todos los campos del
    struct sockaddr<sub>in</sub> han de almacenarse en formato big-endian.
-   Cualquier dato que ocupe **mas de un byte** debera mandarse con el formato big-endian.
    -   Aunque esto no es aplicable a las cadenas de caracteres, ya que cada char ocupa 1 byte.
-   En la direccion se puede especificar una IP, un puerto, ambos o ninguno.
    -   Por ejemplo, si queremos solo el puerto le damos el valor: INADDR<sub>ANY</sub> (cualquier IP).

