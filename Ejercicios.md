# Administración de memoria.
## 3.1 Política y filosofía.
### ¿Cuál es la diferencia entre fragmentación interna y externa?
La fragmentación interna ocurre cuando un bloque de memoria asignado a un proceso es más grande de lo necesario, dejando partes no utilizadas dentro del bloque. Aunque el espacio está reservado, no se puede usar para otros procesos. 
Esto afecta desperdiciando memoria dentro de los bloques asignados además de que afecta el rendimiento porque se reduce la cantidad de memoria disponible para otros procesos.

La fragmentación externa sucede cuando hay pequeños fragmentos de memoria libres dispersos entre bloques asignados. Estos fragmentos no son lo suficientemente grandes como para satisfacer nuevas solicitudes de memoria, aunque la suma total podría ser suficiente.
Esto afecta a la memoria generando más tiempo de procesamiento para encontrar bloques adecuados.
---------------------------------------------------------
### Investiga y explica las políticas de reemplazo de páginas en sistemas operativos. ¿Cuál consideras más eficiente y por qué?
En sistemas operativos que usan memoria virtual, las políticas de reemplazo de páginas determinan qué página se debe sacar de la memoria principal para dar espacio a una nueva cuando ocurre un fallo de página.
* FIFO (First In, First Out): La página que llegó primero a la memoria es la que se reemplaza.
Es simple de implementar y no requiere cálculos complejos.

* LRU (Least Recently Used): Se reemplaza la página que no se ha usado en el mayor tiempo. Es una aproximación práctica al uso futuro y tiene mejor rendimiento que FIFO en la mayoría de los casos.

* Optimal (OPT): Se reemplaza la página que no se necesitará por más tiempo en el futuro. Es teóricamente la política más eficiente y según minimiza los fallos de página.

* NRU (Not Recently Used): Clasifica las páginas según dos bits (referencia y modificación). Las páginas que no se han usado ni modificado recientemente son las candidatas a reemplazo. Es eficiente en tiempo y espacio.

* Clock (o Second Chance): Similar a FIFO, pero da una "segunda oportunidad" a las páginas si han sido usadas recientemente además de que usa un puntero en forma circular. Es una buena combinación entre eficiencia y simplicidad y se cree es más eficiente que FIFO y más fácil de implementar que LRU.

* Según mi opinión, ¿Cuál es más eficiente y por qué?
Según yo LRU suele ser más eficiente porque se basa en patrones de acceso recientes, lo cual es una buena estimación del comportamiento futuro.
Aunque Clock también se me hace buena opción, ya que tiene un balance entre rendimiento y complejidad, además de que mejora FIFO sin requerir la complejidad de LRU.
----------------------------------------------------
#  3.2 Memoria real
## Escribe un programa en C o Python que simule la administración de memoria mediante particiones fijas.

```c
#include <stdio.h>
#include <stdbool.h>


#define NUM_PARTICIONES 5 
#define TAMANIOS_PARTICIONES {100, 200, 300, 400, 500} 


typedef struct {
    int id;
    int tamanio;
    bool asignado; 
} Particion;

typedef struct {
    int id;
    int tamanio;
} Proceso;


void inicializarParticiones(Particion particiones[], int tamanios[]);
void mostrarEstado(Particion particiones[]);
void asignarProceso(Particion particiones[], Proceso proceso);
void liberarParticion(Particion particiones[], int idParticion);


int main(int argc, char const *argv[]) {
    int tamanios[] = TAMANIOS_PARTICIONES; 
    Particion particiones[NUM_PARTICIONES];
    inicializarParticiones(particiones, tamanios);

    int opcion; 
    do {
        printf("\n--- Administración de Memoria: Particiones Fijas ---\n");
        printf("1. Mostrar estado de las particiones\n");
        printf("2. Asignar proceso\n");
        printf("3. Liberar partición\n");
        printf("4. Salir\n");
        printf("Elige una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
        case 1:
            mostrarEstado(particiones);
            break;
        case 2: {
            Proceso proceso;
            printf("Introduce el ID del proceso: ");
            scanf("%d", &proceso.id);
            printf("Introduce el tamaño del proceso: ");
            scanf("%d", &proceso.tamanio);
            asignarProceso(particiones, proceso);
            break;
        }
        case 3: {
            int idParticion;
            printf("Introduce el ID de la partición a liberar: ");
            scanf("%d", &idParticion);
            liberarParticion(particiones, idParticion);
            break;
        }
        case 4:
            printf("Saliendo del programa amonooos...\n");
            break;
        default:
            printf("Opción no válida. Intenta de nuevo.\n");
        }
    } while (opcion != 4);

    return 0;
}

// Implementación de funciones

void inicializarParticiones(Particion particiones[], int tamanios[]) {
    for (int i = 0; i < NUM_PARTICIONES; i++) {
        particiones[i].id = i + 1;
        particiones[i].tamanio = tamanios[i];
        particiones[i].asignado = false;
    }
}

void mostrarEstado(Particion particiones[]) {
    printf("\nEstado de las particiones:\n");
    for (int i = 0; i < NUM_PARTICIONES; i++) {
        printf("Partición %d: Tamaño %d, %s\n",
               particiones[i].id,
               particiones[i].tamanio,
               particiones[i].asignado ? "Ocupada" : "Libre");
    }
}

void asignarProceso(Particion particiones[], Proceso proceso) {
    for (int i = 0; i < NUM_PARTICIONES; i++) {
        if (!particiones[i].asignado && particiones[i].tamanio >= proceso.tamanio) {
            particiones[i].asignado = true;
            printf("Proceso %d (Tamaño %d) asignado a la partición %d\n",
                   proceso.id, proceso.tamanio, particiones[i].id);
            return;
        }
    }
    printf("No se puede asignar el Proceso %d (Tamaño %d): No hay particiones disponibles o suficientes.\n",
           proceso.id, proceso.tamanio);
}

void liberarParticion(Particion particiones[], int idParticion) {
    for (int i = 0; i < NUM_PARTICIONES; i++) {
        if (particiones[i].id == idParticion) {
            if (particiones[i].asignado) {
                particiones[i].asignado = false;
                printf("Partición %d liberada.\n", idParticion);
            } else {
                printf("La Partición %d ya está libre :D .\n", idParticion);
            }
            return;
        }
    }
    printf("No existe la Particin :c %d.\n", idParticion);
}
```
## Diseña un algoritmo para calcular qué procesos pueden ser asignados a un sistema con memoria real limitada utilizando el algoritmo de "primera cabida".

```c
#include <stdio.h>
#include <stdbool.h>

#define NUM_BLOQUES 5
#define NUM_PROCESOS 4

void primeraCabida(int bloques[], bool estado[], int numBloques, int procesos[], int numProcesos) {
    printf("\n--- Primera Cabida ---\n");
    for (int i = 0; i < numProcesos; i++) {
        bool asignado = false;
        for (int j = 0; j < numBloques; j++) {
            if (procesos[i] <= bloques[j] && !estado[j]) {
                printf("Proceso %d (Tamaño %d) asignado al Bloque %d (Tamaño %d)\n",
                       i + 1, procesos[i], j + 1, bloques[j]);
                estado[j] = true; 
                asignado = true;
                break;
            }
        }
        if (!asignado) {
            printf("Proceso %d (Tamaño %d) no pudo ser asignado\n", i + 1, procesos[i]);
        }
    }
}

int main() {
    int bloques[NUM_BLOQUES] = {100, 500, 200, 300, 600}; 
    bool estado[NUM_BLOQUES] = {false, false, false, false, false}; 
    int procesos[NUM_PROCESOS] = {212, 417, 112, 426}; 

    printf("Bloques de memoria disponibles:\n");
    for (int i = 0; i < NUM_BLOQUES; i++) {
        printf("Bloque %d: Tamaño %d, Estado: %s\n", i + 1, bloques[i], estado[i] ? "Ocupado" : "Libre");
    }

    printf("\nProcesos a asignar:\n");
    for (int i = 0; i < NUM_PROCESOS; i++) {
        printf("Proceso %d: Tamaño %d\n", i + 1, procesos[i]);
    }

    primeraCabida(bloques, estado, NUM_BLOQUES, procesos, NUM_PROCESOS);

    return 0;
}
```
-------------------------------------------------
# 3.3 Organización de memoria virtual
## Investiga y explica el concepto de "paginación" y "segmentación". ¿Cuáles son las ventajas y desventajas de cada técnica?
La paginación es una técnica de administración de memoria que divide tanto el espacio de direcciones lógicas de los procesos como la memoria física en bloques de tamaño fijo llamados páginas y marcos, respectivamente. Esto permite que las páginas de un proceso se almacenen en marcos dispersos de la memoria física, eliminando la necesidad de bloques contiguos y reduciendo la fragmentación externa, aunque puede provocar fragmentación interna si las páginas no se llenan por completo. Por otro lado, la segmentación organiza la memoria en segmentos de tamaño variable que representan partes lógicas del programa, como código, datos o pila, lo que facilita una representación más natural y estructurada del espacio de direcciones. A diferencia de la paginación, la segmentación evita la fragmentación interna al ajustar el tamaño del segmento a las necesidades del proceso, pero puede generar fragmentación externa y complicar la gestión debido a la variabilidad en el tamaño y la ubicación de los segmentos en la memoria.

**ventajas de la paginación**
* Elimina la fragmentación externa: Como las páginas tienen un tamaño fijo, cualquier marco libre puede ser utilizado.
* Facilita la multiprogramación: Los procesos no necesitan bloques continuos de memoria física.
* Simplifica la asignación: Se basa únicamente en el tamaño fijo de las páginas.

**Desventajas de la paginación**

* Fragmentación interna: Si un proceso no llena completamente una página, el espacio restante no se puede usar para otros procesos.
* Sobrecarga de la tabla de páginas: Cada proceso necesita su propia tabla, lo que aumenta el uso de memoria para grandes sistemas.
* Mayor tiempo de traducción: Se necesita hardware adicional, como una TLB (Translation Lookaside Buffer), para mejorar el rendimiento.

**Ventajas de la segmentación**
* Refleja la estructura lógica del programa: Ayuda a organizar la memoria de forma más natural (separando código, datos, pila, etc.).
* No hay fragmentación interna: El tamaño del segmento se ajusta exactamente a lo que el proceso requiere.
* Facilita la protección y el aislamiento: Los segmentos pueden tener diferentes permisos (por ejemplo, solo lectura para el código).

**Desventajas de la segmentación**
* Fragmentación externa: A medida que los segmentos se asignan y liberan, pueden quedar espacios no utilizables entre ellos.
* Sobrecarga de gestión: Requiere más esfuerzo para mantener la tabla de segmentos y la memoria libre.
* No es tan eficiente como la paginación para asignaciones pequeñas. 
-----------------------------------------------
  
## Escribe un programa que simule una tabla de páginas para procesos con acceso aleatorio a memoria virtual.

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define NUM_PAGINAS 10    
#define TAMANO_PAGINA 256 
#define NUM_MARCOS 5      
#define NUM_ACCESOS 20    

typedef struct {
    int pagina;   
    bool valido;  
} Marco;


int tablaPaginas[NUM_PAGINAS];


Marco memoriaFisica[NUM_MARCOS];


void inicializarTabla() {
    for (int i = 0; i < NUM_PAGINAS; i++) {
        tablaPaginas[i] = -1; 
    }
    for (int i = 0; i < NUM_MARCOS; i++) {
        memoriaFisica[i].pagina = -1;
        memoriaFisica[i].valido = false;
    }
}


void accederMemoria(int direccionLogica) {
    int pagina = direccionLogica / TAMANO_PAGINA;
    int desplazamiento = direccionLogica % TAMANO_PAGINA;

    if (pagina < 0 || pagina >= NUM_PAGINAS) {
        printf("Error: Dirección lógica %d fuera de rango.\n", direccionLogica);
        return;
    }

    int marco = tablaPaginas[pagina];
    if (marco != -1 && memoriaFisica[marco].valido) {
        
        int direccionFisica = marco * TAMANO_PAGINA + desplazamiento;
        printf("Acceso exitoso: Dirección lógica %d -> Página %d -> Dirección física %d\n", 
               direccionLogica, pagina, direccionFisica);
    } else {
        
        printf("Fallo de página: Página %d no está en memoria física.\n", pagina);

        
        static int siguienteMarco = 0;
        int marcoReemplazo = siguienteMarco;
        siguienteMarco = (siguienteMarco + 1) % NUM_MARCOS;

        if (memoriaFisica[marcoReemplazo].valido) {
            int paginaAntigua = memoriaFisica[marcoReemplazo].pagina;
            tablaPaginas[paginaAntigua] = -1; 
            printf("Reemplazando Página %d con Página %d en Marco %d.\n", 
                   paginaAntigua, pagina, marcoReemplazo);
        } else {
            printf("Cargando Página %d en Marco %d.\n", pagina, marcoReemplazo);
        }

        memoriaFisica[marcoReemplazo].pagina = pagina;
        memoriaFisica[marcoReemplazo].valido = true;
        tablaPaginas[pagina] = marcoReemplazo;
    }
}

int main() {
    srand(time(NULL)); 

    inicializarTabla();

    printf("--- Simulación de tabla de páginas ---\n");
    for (int i = 0; i < NUM_ACCESOS; i++) {
        int direccionLogica = rand() % (NUM_PAGINAS * TAMANO_PAGINA);
        accederMemoria(direccionLogica);
    }

    return 0;
}
```
--------------------------------------------------
# 3.4 Administración de memoria virtual

## Escribe un código que implemente el algoritmo de reemplazo de página "Least Recently Used" (LRU).

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_PAGINAS 10    
#define TAMANO_PAGINA 256 
#define NUM_MARCOS 3      
#define NUM_ACCESOS 20  

typedef struct {
    int pagina;    
    int tiempoUso; 
    bool valido;  
} Marco;

Marco memoriaFisica[NUM_MARCOS];

void inicializarMemoria() {
    for (int i = 0; i < NUM_MARCOS; i++) {
        memoriaFisica[i].pagina = -1; 
        memoriaFisica[i].valido = false;
        memoriaFisica[i].tiempoUso = -1; 
    }
}

int encontrarLRU() {
    int lruIndice = 0, lruTiempo = memoriaFisica[0].tiempoUso;
    for (int i = 1; i < NUM_MARCOS; i++) {
        if (memoriaFisica[i].tiempoUso < lruTiempo) {
            lruIndice = i;
            lruTiempo = memoriaFisica[i].tiempoUso;
        }
    }
    return lruIndice;
}

void accederMemoria(int direccionLogica, int tiempo) {
    int pagina = direccionLogica / TAMANO_PAGINA;
    int desplazamiento = direccionLogica % TAMANO_PAGINA;

    if (pagina < 0 || pagina >= NUM_PAGINAS) {
        printf("Error: Dirección lógica %d fuera de rango.\n", direccionLogica);
        return;
    }

    for (int i = 0; i < NUM_MARCOS; i++) {
        if (memoriaFisica[i].valido && memoriaFisica[i].pagina == pagina) {
            printf("Acceso exitoso: Dirección lógica %d -> Página %d -> Marco %d\n",
                   direccionLogica, pagina, i);
            memoriaFisica[i].tiempoUso = tiempo; 
            return;
        }
    }
   
    printf("Fallo de página: Página %d no está en memoria física.\n", pagina);
   
    int marcoReemplazo = encontrarLRU();
    if (memoriaFisica[marcoReemplazo].valido) {
        printf("Reemplazando Página %d con Página %d en Marco %d.\n",
               memoriaFisica[marcoReemplazo].pagina, pagina, marcoReemplazo);
    } else {
        printf("Cargando Página %d en Marco %d.\n", pagina, marcoReemplazo);
    }

    memoriaFisica[marcoReemplazo].pagina = pagina;
    memoriaFisica[marcoReemplazo].valido = true;
    memoriaFisica[marcoReemplazo].tiempoUso = tiempo;
}

int main() {
    srand(time(NULL));

    inicializarMemoria();

    printf("--- Simulación de reemplazo de página LRU ---\n");

    for (int tiempo = 0; tiempo < NUM_ACCESOS; tiempo++) {
        int direccionLogica = rand() % (NUM_PAGINAS * TAMANO_PAGINA);
        accederMemoria(direccionLogica, tiempo);
    }

    return 0;
}
```
--------------------------------------------------
## Diseña un diagrama que represente el proceso de traducción de direcciones virtuales a físicas en un sistema con memoria virtual.
![Diagrama:](https://upload.wikimedia.org/wikipedia/commons/3/32/Virtual_address_space_and_physical_address_space_relationship.svg)

---------------------------------------------------
## Integración. 
### Analiza un sistema operativo moderno (por ejemplo, Linux o Windows) e identifica cómo administra la memoria virtual

Sistema operativo que yo analicé fue xubuntu.
Cada proceso en Xubuntu tiene su propio espacio de direcciones virtuales, que está dividido en secciones, como código, datos, pila, y espacio para bibliotecas compartidas. Esto aísla a los procesos entre sí, mejorando la seguridad y estabilidad.
Xubuntu utiliza un modelo basado en paginación. La unidad mínima de almacenamiento es una página (generalmente de 4 KB). Las direcciones virtuales se traducen a físicas mediante una tabla de páginas, que el hardware administra en conjunto con el kernel.
Cuando la memoria física se agota, Xubuntu utiliza un archivo o partición de intercambio para mover temporalmente las páginas menos utilizadas de la RAM al disco. Aunque el acceso al disco es más lento, esto permite manejar cargas de trabajo más grandes de lo que permite la memoria física.
Para seleccionar qué páginas mover al archivo de intercambio, Xubuntu usa algoritmos avanzados como Least Recently Used (LRU) con optimizaciones como Clock Algorithm. Esto garantiza que las páginas menos utilizadas sean las primeras en ser intercambiadas.
Xubuntu puede mapear archivos directamente al espacio de direcciones de un proceso, lo que optimiza el acceso y reduce el uso de memoria física.

-----------------------------------------
### Realiza una simulación en cualquier lenguaje de programación que emule el swapping de procesos en memoria virtual.

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MEMORIA_FISICA 100 
#define MEMORIA_SWAP 200    
#define MAX_PROCESOS 10    
#define UNIDADES_PROCESO 30

typedef struct {
    int id;             
    int tamano;          
    bool enMemoriaFisica; 
} Proceso;

Proceso procesos[MAX_PROCESOS];
int memoriaFisicaUsada = 0;
int memoriaSwapUsada = 0;


void inicializarProcesos() {
    for (int i = 0; i < MAX_PROCESOS; i++) {
        procesos[i].id = i + 1;
        procesos[i].tamano = (rand() % UNIDADES_PROCESO) + 10; 
        procesos[i].enMemoriaFisica = false;
    }
}

void mostrarEstado() {
    printf("\n--- Estado de la memoria ---\n");
    printf("Memoria Física: %d/%d\n", memoriaFisicaUsada, MEMORIA_FISICA);
    printf("Memoria Swap: %d/%d\n", memoriaSwapUsada, MEMORIA_SWAP);
    printf("Procesos en Memoria Física:\n");
    for (int i = 0; i < MAX_PROCESOS; i++) {
        if (procesos[i].enMemoriaFisica) {
            printf("  - Proceso %d (Tamaño: %d)\n", procesos[i].id, procesos[i].tamano);
        }
    }
    printf("Procesos en Swap:\n");
    for (int i = 0; i < MAX_PROCESOS; i++) {
        if (!procesos[i].enMemoriaFisica) {
            printf("  - Proceso %d (Tamaño: %d)\n", procesos[i].id, procesos[i].tamano);
        }
    }
}

bool cargarEnMemoriaFisica(int indice) {
    if (memoriaFisicaUsada + procesos[indice].tamano <= MEMORIA_FISICA) {
        procesos[indice].enMemoriaFisica = true;
        memoriaFisicaUsada += procesos[indice].tamano;
        return true;
    }
    return false;
}

void moverASwap(int indice) {
    if (memoriaSwapUsada + procesos[indice].tamano <= MEMORIA_SWAP) {
        memoriaFisicaUsada -= procesos[indice].tamano;
        procesos[indice].enMemoriaFisica = false;
        memoriaSwapUsada += procesos[indice].tamano;
        printf("Proceso %d movido a Swap.\n", procesos[indice].id);
    } else {
        printf("Error: No hay espacio suficiente en Swap para el Proceso %d.\n", procesos[indice].id);
    }
}

void simularSwapping() {
    for (int i = 0; i < MAX_PROCESOS; i++) {
        printf("\nIntentando cargar Proceso %d (Tamaño: %d)...\n", procesos[i].id, procesos[i].tamano);


        if (!cargarEnMemoriaFisica(i)) {
            printf("Memoria física insuficiente. Realizando swapping...\n");

           
            for (int j = 0; j < MAX_PROCESOS; j++) {
                if (procesos[j].enMemoriaFisica) {
                    moverASwap(j);
                    break;
                }
            }

          
            if (!cargarEnMemoriaFisica(i)) {
                printf("Error: No se pudo cargar el Proceso %d en memoria física.\n", procesos[i].id);
            }
        } else {
            printf("Proceso %d cargado en memoria física.\n", procesos[i].id);
        }

        
        mostrarEstado();
    }
}

int main() {
    srand(time(NULL)); 

    inicializarProcesos();
    mostrarEstado();
    simularSwapping();

    return 0;
}
```
--------------------------------------------

# **Administración de entrada y salida.**

# 4.1 Dispositivos y manejadores de dispositivos
## Explica la diferencia entre dispositivos de bloque y dispositivos de carácter. Da un ejemplo de cada uno.

Los dispositivos de bloque y los dispositivos de carácter se diferencian en la forma en que manejan los datos. Los dispositivos de bloque almacenan y acceden a los datos en bloques de tamaño fijo, lo que permite operaciones aleatorias y eficientes, ya que los datos pueden ser leídos o escritos en cualquier orden. Un ejemplo típico de dispositivo de bloque es un disco duro o una unidad SSD, donde los datos se manejan en sectores o bloques lógicos. Por otro lado, los dispositivos de carácter manejan los datos como un flujo continuo, procesando cada carácter de manera secuencial sin posibilidad de acceso aleatorio directo, lo que los hace ideales para dispositivos como teclados, ratones o puertos serie, donde los datos llegan en un flujo constante y en el orden en que se generan.

--------------------------------

##  Diseña un programa que implemente un manejador de dispositivos sencillo para un dispositivo virtual de entrada.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANO_BUFFER 256 


typedef struct {
    char buffer[TAMANO_BUFFER]; 
    int indice;                
} DispositivoVirtual;

void inicializarDispositivo(DispositivoVirtual *dispositivo) {
    memset(dispositivo->buffer, 0, TAMANO_BUFFER);
    dispositivo->indice = 0;
    printf("Dispositivo inicializado.\n");
}

void escribirDispositivo(DispositivoVirtual *dispositivo, char entrada) {
    if (dispositivo->indice < TAMANO_BUFFER - 1) {
        dispositivo->buffer[dispositivo->indice++] = entrada;
        dispositivo->buffer[dispositivo->indice] = '\0'; 
        printf("Carácter '%c' almacenado en el dispositivo.\n", entrada);
    } else {
        printf("Error: El búfer del dispositivo está lleno.\n");
    }
}

void leerDispositivo(DispositivoVirtual *dispositivo) {
    if (dispositivo->indice > 0) {
        printf("Contenido del búfer: %s\n", dispositivo->buffer);
    } else {
        printf("El búfer está vacío.\n");
    }
}


void limpiarDispositivo(DispositivoVirtual *dispositivo) {
    memset(dispositivo->buffer, 0, TAMANO_BUFFER);
    dispositivo->indice = 0;
    printf("El búfer del dispositivo ha sido limpiado.\n");
}


int main() {
    DispositivoVirtual dispositivo;
    inicializarDispositivo(&dispositivo);

    char opcion;
    char entrada;

    do {
        printf("\n--- Menú del Manejador de Dispositivo ---\n");
        printf("1. Escribir en el dispositivo\n");
        printf("2. Leer del dispositivo\n");
        printf("3. Limpiar el dispositivo\n");
        printf("4. Salir\n");
        printf("Seleccione una opción: ");
        scanf(" %c", &opcion);

        switch (opcion) {
            case '1':
                printf("Ingrese un carácter: ");
                scanf(" %c", &entrada);
                escribirDispositivo(&dispositivo, entrada);
                break;
            case '2':
                leerDispositivo(&dispositivo);
                break;
            case '3':
                limpiarDispositivo(&dispositivo);
                break;
            case '4':
                printf("Saliendo del programa.\n");
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }
    } while (opcion != '4');

    return 0;
}
```

------------------------------

# 4.2 Mecanismos y funciones de los manejadores de dispositivos.

## Investiga qué es la interrupción por E/S y cómo la administra el sistema operativo. Escribe un ejemplo en pseudocódigo para simular este proceso.

Una interrupción por E/S ocurre cuando un dispositivo de entrada/salida (como un teclado, disco o impresora) requiere atención del procesador. En lugar de que el procesador espere pasivamente a que el dispositivo complete su tarea, el dispositivo genera una señal de interrupción para notificar al sistema operativo que ha terminado su operación o necesita atención. 
El sistema operativo responde deteniendo temporalmente la ejecución del proceso actual, manejando la interrupción mediante un controlador, y luego reanuda la ejecución del proceso interrumpido. 
Este mecanismo mejora la eficiencia al permitir que el procesador realice otras tareas mientras espera la operación de E/S.

**Pseudocódigo:**
* Inicializar sistema:
Establecer estado del procesador en "Ejecutando proceso principal".
Establecer cola de interrupciones vacía.

* Proceso principal:
Mientras el sistema esté activo: 
Realizar cálculos.
Si se genera una solicitud de E/S:
Iniciar operación de E/S en el dispositivo.
Continuar con cálculos mientras se completa la E/S.

* Interrupción por E/S:
Cuando el dispositivo finaliza la operación:
Generar señal de interrupción.
Pausar el proceso principal.
Ejecutar manejador de interrupciones:
Guardar el estado del proceso principal.
Leer datos del dispositivo o confirmar finalización.
Actualizar estructuras de datos del sistema operativo.
Restaurar el estado del proceso principal.
Reanudar el proceso principal.

* Simulación:
Iniciar sistema.
Simular operaciones de cálculo y solicitudes de E/S.
Generar interrupciones al azar y verificar su manejo.
Terminar cuando se completen todas las operaciones.

----------------------------------------

## Escribe un programa que utilice el manejo de interrupciones en un sistema básico de simulación.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

#define MAX_INTERRUPTS 5

typedef struct {
    int interrupt_id;     
    char description[50]; 
} Interrupt;

Interrupt interrupt_queue[MAX_INTERRUPTS]; 
int interrupt_count = 0;                   

jmp_buf env;

void trigger_interrupt(int interrupt_id, const char* description) {
    if (interrupt_count < MAX_INTERRUPTS) {
        interrupt_queue[interrupt_count].interrupt_id = interrupt_id;
        snprintf(interrupt_queue[interrupt_count].description, sizeof(interrupt_queue[interrupt_count].description), "%s", description);
        interrupt_count++;
        printf("¡Interrupción %d: %s generada!\n", interrupt_id, description);
    } else {
        printf("Error: Cola de interrupciones llena.\n");
    }
}

void handle_interrupt() {
    while (interrupt_count > 0) {
        Interrupt interrupt = interrupt_queue[0];
        printf("Manejando interrupción ID %d: %s\n", interrupt.interrupt_id, interrupt.description);

        for (int i = 1; i < interrupt_count; i++) {
            interrupt_queue[i - 1] = interrupt_queue[i];
        }
        interrupt_count--;

        if (setjmp(env) == 0) {
            printf("Contexto guardado. Retornando al punto de ejecución previo...\n");
            longjmp(env, 1); 
        }
    }
}

void simulate_process() {
    printf("Proceso en ejecución. Esperando interrupciones...\n");
    while (1) {
        sleep(2); 
        printf("El proceso está trabajando...\n");

        if (rand() % 5 == 0) {
            trigger_interrupt(rand() % 100, "Evento inesperado");
        }

        if (interrupt_count > 0) {
            handle_interrupt();
        }
    }
}

int main() {
    
    signal(SIGINT, SIG_IGN); 

    simulate_process();

    return 0;
}
```

----------------

# 4.3 Estructuras de datos para manejo de dispositivos.

## Investiga y explica qué es una cola de E/S. Diseña una simulación de una cola con prioridad.

Una cola de E/S (Entrada/Salida) es una estructura de datos utilizada en sistemas operativos para gestionar las solicitudes de operaciones de entrada y salida de los procesos. Cuando un proceso necesita realizar una operación de E/S, como leer o escribir en un dispositivo, la solicitud se coloca en una cola específica para ese dispositivo. El sistema operativo procesa estas solicitudes en el orden en que se reciben, garantizando un acceso ordenado y eficiente a los recursos de E/S. Este mecanismo es esencial para la gestión de dispositivos de E/S, ya que permite coordinar múltiples solicitudes y optimizar el rendimiento del sistema. 
Las colas con prioridad, son estructuras de datos que permiten organizar elementos según su prioridad, de modo que los elementos con mayor prioridad se procesen antes que los de menor prioridad. Este tipo de cola es útil en escenarios donde ciertas operaciones o procesos requieren atención inmediata, como en sistemas de planificación de procesos en sistemas operativos.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;             
    int prioridad;      
    char descripcion[50]; 
} SolicitudEoS;

typedef struct Nodo {
    SolicitudEoS solicitud;
    struct Nodo* siguiente;
} Nodo;

Nodo* crearNodo(int id, int prioridad, const char* descripcion) {
    Nodo* nuevoNodo = (Nodo*)malloc(sizeof(Nodo));
    nuevoNodo->solicitud.id = id;
    nuevoNodo->solicitud.prioridad = prioridad;
    strncpy(nuevoNodo->solicitud.descripcion, descripcion, sizeof(nuevoNodo->solicitud.descripcion) - 1);
    nuevoNodo->solicitud.descripcion[sizeof(nuevoNodo->solicitud.descripcion) - 1] = '\0';
    nuevoNodo->siguiente = NULL;
    return nuevoNodo;
}

void insertarSolicitud(Nodo** cabeza, int id, int prioridad, const char* descripcion) {
    Nodo* nuevoNodo = crearNodo(id, prioridad, descripcion);
    if (*cabeza == NULL || (*cabeza)->solicitud.prioridad > prioridad) {
        nuevoNodo->siguiente = *cabeza;
        *cabeza = nuevoNodo;
    } else {
        Nodo* actual = *cabeza;
        while (actual->siguiente != NULL && actual->siguiente->solicitud.prioridad <= prioridad) {
            actual = actual->siguiente;
        }
        nuevoNodo->siguiente = actual->siguiente;
        actual->siguiente = nuevoNodo;
    }
}

void procesarSolicitud(Nodo** cabeza) {
    if (*cabeza == NULL) {
        printf("No hay solicitudes en la cola.\n");
        return;
    }
    Nodo* nodoAProcesar = *cabeza;
    *cabeza = (*cabeza)->siguiente;
    printf("Procesando solicitud ID %d: %s\n", nodoAProcesar->solicitud.id, nodoAProcesar->solicitud.descripcion);
    free(nodoAProcesar);
}

void mostrarCola(Nodo* cabeza) {
    if (cabeza == NULL) {
        printf("La cola está vacía.\n");
        return;
    }
    Nodo* actual = cabeza;
    while (actual != NULL) {
        printf("ID: %d, Prioridad: %d, Descripción: %s\n", actual->solicitud.id, actual->solicitud.prioridad, actual->solicitud.descripcion);
        actual = actual->siguiente;
    }
}

int main() {
    Nodo* cola = NULL;

    insertarSolicitud(&cola, 1, 2, "Leer archivo de datos");
    insertarSolicitud(&cola, 2, 1, "Escribir en disco");
    insertarSolicitud(&cola, 3, 3, "Imprimir documento");
    insertarSolicitud(&cola, 4, 1, "Leer desde red");

    printf("Contenido de la cola de E/S:\n");
    mostrarCola(cola);

    printf("\nProcesando solicitudes:\n");
    while (cola != NULL) {
        procesarSolicitud(&cola);
    }

    return 0;
}
```

-------------------------

## Escribe un programa que simule las operaciones de un manejador de dispositivos utilizando una tabla de estructuras.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;                 
    char nombre[20];       
    int estado;             
    void (*abrir)(void);    
    void (*leer)(void);    
    void (*escribir)(void); 
    void (*cerrar)(void);   
} Dispositivo;

void abrirDispositivo() {
    printf("Dispositivo abierto.\n");
}

void leerDispositivo() {
    printf("Leyendo datos del dispositivo...\n");
}

void escribirDispositivo() {
    printf("Escribiendo datos en el dispositivo...\n");
}

void cerrarDispositivo() {
    printf("Dispositivo cerrado.\n");
}

Dispositivo* tablaDispositivos[10]; 

void inicializarDispositivo(int id, const char* nombre) {
    Dispositivo* nuevoDispositivo = (Dispositivo*)malloc(sizeof(Dispositivo));
    nuevoDispositivo->id = id;
    strncpy(nuevoDispositivo->nombre, nombre, sizeof(nuevoDispositivo->nombre) - 1);
    nuevoDispositivo->estado = 0;
    nuevoDispositivo->abrir = abrirDispositivo;
    nuevoDispositivo->leer = leerDispositivo;
    nuevoDispositivo->escribir = escribirDispositivo;
    nuevoDispositivo->cerrar = cerrarDispositivo;

    tablaDispositivos[id] = nuevoDispositivo;
}

void manejarDispositivo(int id) {
    Dispositivo* dispositivo = tablaDispositivos[id];
    if (dispositivo == NULL) {
        printf("Dispositivo no encontrado.\n");
        return;
    }

    if (dispositivo->estado == 0) {
        dispositivo->abrir();
        dispositivo->estado = 1; 
    }

    dispositivo->leer();
    dispositivo->escribir();

    dispositivo->cerrar();
    dispositivo->estado = 0; 
}

int main() {
    inicializarDispositivo(0, "Dispositivo A");
    inicializarDispositivo(1, "Dispositivo B");

    printf("Manejando Dispositivo A:\n");
    manejarDispositivo(0);

    printf("\nManejando Dispositivo B:\n");
    manejarDispositivo(1);

    free(tablaDispositivos[0]);
    free(tablaDispositivos[1]);

    return 0;
}
```

--------------------------------


