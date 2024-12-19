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

# 4.4 Operaciones de Entrada/Salida
## Diseña un flujo que describa el proceso de lectura de un archivo desde un disco magnético. Acompáñalo con un programa básico que simule el proceso.
![Lectura de disco](https://mdm.usta.edu.co/remos_downloads/oev/logica_de_programacion/graficos/disco1.jpg)

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_SECTORES 10
#define TAMANO_SECTOR 512

typedef struct {
    int id;
    char datos[TAMANO_SECTOR];
} Sector;

typedef struct {
    char nombre[20];
    int sectorInicio;
    int tamano;
} Archivo;

Sector disco[NUM_SECTORES];

Archivo tablaArchivos[NUM_SECTORES];
int numArchivos = 0;

void inicializarDisco() {
    for (int i = 0; i < NUM_SECTORES; i++) {
        disco[i].id = i;
        snprintf(disco[i].datos, TAMANO_SECTOR, "Datos en el sector %d", i);
    }
    printf("Disco inicializado con %d sectores.\n", NUM_SECTORES);
}

void crearArchivo(const char* nombre, int tamano) {
    if (numArchivos >= NUM_SECTORES) {
        printf("Error: No hay espacio disponible en el disco.\n");
        return;
    }

    int sectorInicio = numArchivos; 
    tablaArchivos[numArchivos].sectorInicio = sectorInicio;
    tablaArchivos[numArchivos].tamano = tamano;
    strncpy(tablaArchivos[numArchivos].nombre, nombre, sizeof(tablaArchivos[numArchivos].nombre) - 1);
    tablaArchivos[numArchivos].nombre[sizeof(tablaArchivos[numArchivos].nombre) - 1] = '\0';

    printf("Archivo '%s' creado en el sector %d con tamaño %d bytes.\n",
           nombre, sectorInicio, tamano);
    numArchivos++;
}

int buscarArchivo(const char* nombre) {
    for (int i = 0; i < numArchivos; i++) {
        if (strcmp(tablaArchivos[i].nombre, nombre) == 0) {
            return i;
        }
    }
    return -1;
}

void leerArchivo(const char* nombre) {
    int indice = buscarArchivo(nombre);
    if (indice == -1) {
        printf("Error: El archivo '%s' no existe.\n", nombre);
        return;
    }

    Archivo archivo = tablaArchivos[indice];
    Sector sector = disco[archivo.sectorInicio];

    printf("Leyendo archivo '%s' desde el sector %d:\n", nombre, archivo.sectorInicio);
    printf("Contenido: %s\n", sector.datos);
}

int main() {
    inicializarDisco();

    crearArchivo("archivo1.txt", 256);
    crearArchivo("archivo2.txt", 128);

    printf("\n--- Lectura de archivos ---\n");
    leerArchivo("archivo1.txt");
    leerArchivo("archivo2.txt");
    leerArchivo("archivo_inexistente.txt");

    return 0;
}
```

---

## Implementa un programa en Python, C o java que realice operaciones de entrada/salida asíncronas usando archivos.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <aio.h>
#include <errno.h>
#include <unistd.h>

#define TAMANO_BUFFER 1024

void manejarError(const char* mensaje) {
    perror(mensaje);
    exit(EXIT_FAILURE);
}

void verificarEstado(struct aiocb* aiocbp) {
    while (aio_error(aiocbp) == EINPROGRESS) {
        printf("Operación en progreso...\n");
        usleep(100000); 
    }

    int resultado = aio_return(aiocbp);
    if (resultado == -1) {
        manejarError("Error en la operación de E/S");
    } else {
        printf("Operación completada con éxito (%d bytes procesados).\n", resultado);
    }
}

int main() {
    const char* archivo = "archivo_async.txt";
    char bufferEscritura[TAMANO_BUFFER] = "Este es un ejemplo de escritura asincrónica.\n";
    char bufferLectura[TAMANO_BUFFER];

    int fd = open(archivo, O_CREAT | O_RDWR, 0644);
    if (fd == -1) {
        manejarError("Error al abrir el archivo");
    }

    struct aiocb aioEscritura;
    memset(&aioEscritura, 0, sizeof(struct aiocb));
    aioEscritura.aio_fildes = fd;
    aioEscritura.aio_buf = bufferEscritura;
    aioEscritura.aio_nbytes = strlen(bufferEscritura);
    aioEscritura.aio_offset = 0;

    if (aio_write(&aioEscritura) == -1) {
        manejarError("Error al iniciar la escritura asincrónica");
    }
    printf("Escritura asincrónica iniciada...\n");

    verificarEstado(&aioEscritura);

    struct aiocb aioLectura;
    memset(&aioLectura, 0, sizeof(struct aiocb));
    aioLectura.aio_fildes = fd;
    aioLectura.aio_buf = bufferLectura;
    aioLectura.aio_nbytes = TAMANO_BUFFER;
    aioLectura.aio_offset = 0;

    if (aio_read(&aioLectura) == -1) {
        manejarError("Error al iniciar la lectura asincrónica");
    }
    printf("Lectura asincrónica iniciada...\n");

    verificarEstado(&aioLectura);

    printf("Contenido leído: %s\n", bufferLectura);

    close(fd);

    return 0;
}
```

------------

# Integración

## Escribe un programa que implemente el algoritmo de planificación de discos "Elevator (SCAN)".

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SOLICITUDES 100

void ordenar(int solicitudes[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (solicitudes[j] > solicitudes[j + 1]) {
                int temp = solicitudes[j];
                solicitudes[j] = solicitudes[j + 1];
                solicitudes[j + 1] = temp;
            }
        }
    }
}

void scan(int solicitudes[], int n, int inicio, int direccion, int limiteDisco) {
    ordenar(solicitudes, n);

    int recorrido = 0;
    printf("Orden de acceso: ");

    int i = 0;
    while (i < n && solicitudes[i] < inicio) {
        i++;
    }

    if (direccion == 1) {
        for (int j = i; j < n; j++) {
            printf("%d ", solicitudes[j]);
            recorrido += abs(solicitudes[j] - inicio);
            inicio = solicitudes[j];
        }
        if (inicio < limiteDisco) {
            recorrido += abs(limiteDisco - inicio);
            inicio = limiteDisco;
        }

        for (int j = i - 1; j >= 0; j--) {
            printf("%d ", solicitudes[j]);
            recorrido += abs(solicitudes[j] - inicio);
            inicio = solicitudes[j];
        }
    } else { 
        for (int j = i - 1; j >= 0; j--) {
            printf("%d ", solicitudes[j]);
            recorrido += abs(solicitudes[j] - inicio);
            inicio = solicitudes[j];
        }
        if (inicio > 0) {
            recorrido += abs(inicio);
            inicio = 0;
        }

        for (int j = i; j < n; j++) {
            printf("%d ", solicitudes[j]);
            recorrido += abs(solicitudes[j] - inicio);
            inicio = solicitudes[j];
        }
    }

    printf("\nRecorrido total del cabezal: %d\n", recorrido);
}

int main() {
    int solicitudes[MAX_SOLICITUDES], n;
    int inicio, direccion, limiteDisco;

    printf("Ingrese el número de solicitudes: ");
    scanf("%d", &n);

    printf("Ingrese las posiciones de las solicitudes: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &solicitudes[i]);
    }

    printf("Ingrese la posición inicial del cabezal: ");
    scanf("%d", &inicio);

    printf("Ingrese el límite del disco: ");
    scanf("%d", &limiteDisco);

    printf("Ingrese la dirección inicial (1 = ascendente, 0 = descendente): ");
    scanf("%d", &direccion);

    printf("\nSimulación del algoritmo SCAN:\n");
    scan(solicitudes, n, inicio, direccion, limiteDisco);

    return 0;
}
```

----

##  Diseña un sistema que maneje múltiples dispositivos simulados (disco duro, impresora, teclado) y muestra cómo se realiza la comunicación entre ellos.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    char dispositivo[20]; 
    char operacion[50];   
} Solicitud;

#define MAX_COLA 10
Solicitud colaSolicitudes[MAX_COLA];
int inicio = 0, fin = 0, numSolicitudes = 0;

pthread_mutex_t mutexCola = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condColaLlena = PTHREAD_COND_INITIALIZER;
pthread_cond_t condColaVacia = PTHREAD_COND_INITIALIZER;

void agregarSolicitud(Solicitud solicitud) {
    pthread_mutex_lock(&mutexCola);
    while (numSolicitudes == MAX_COLA) {
        pthread_cond_wait(&condColaLlena, &mutexCola);
    }
    colaSolicitudes[fin] = solicitud;
    fin = (fin + 1) % MAX_COLA;
    numSolicitudes++;
    pthread_cond_signal(&condColaVacia);
    pthread_mutex_unlock(&mutexCola);
}

Solicitud procesarSolicitud() {
    pthread_mutex_lock(&mutexCola);
    while (numSolicitudes == 0) {
        pthread_cond_wait(&condColaVacia, &mutexCola);
    }
    Solicitud solicitud = colaSolicitudes[inicio];
    inicio = (inicio + 1) % MAX_COLA;
    numSolicitudes--;
    pthread_cond_signal(&condColaLlena);
    pthread_mutex_unlock(&mutexCola);
    return solicitud;
}

void* manejadorCentral(void* arg) {
    while (1) {
        Solicitud solicitud = procesarSolicitud();
        printf("[Manejador] Procesando solicitud del dispositivo '%s': %s\n",
               solicitud.dispositivo, solicitud.operacion);
        sleep(1); 
    }
    return NULL;
}

void* dispositivoDiscoDuro(void* arg) {
    Solicitud solicitud = {"Disco Duro", "Leer datos del sector 5"};
    agregarSolicitud(solicitud);
    sleep(2);
    solicitud = (Solicitud){"Disco Duro", "Escribir datos en el sector 10"};
    agregarSolicitud(solicitud);
    return NULL;
}

void* dispositivoImpresora(void* arg) {
    Solicitud solicitud = {"Impresora", "Imprimir documento 'reporte.pdf'"};
    agregarSolicitud(solicitud);
    sleep(3);
    solicitud = (Solicitud){"Impresora", "Imprimir documento 'factura.txt'"};
    agregarSolicitud(solicitud);
    return NULL;
}

void* dispositivoTeclado(void* arg) {
    Solicitud solicitud = {"Teclado", "Capturar texto del usuario"};
    agregarSolicitud(solicitud);
    sleep(1);
    solicitud = (Solicitud){"Teclado", "Detección de pulsación de tecla"};
    agregarSolicitud(solicitud);
    return NULL;
}

int main() {
    pthread_t hiloManejador, hiloDiscoDuro, hiloImpresora, hiloTeclado;

    pthread_create(&hiloManejador, NULL, manejadorCentral, NULL);

    pthread_create(&hiloDiscoDuro, NULL, dispositivoDiscoDuro, NULL);
    pthread_create(&hiloImpresora, NULL, dispositivoImpresora, NULL);
    pthread_create(&hiloTeclado, NULL, dispositivoTeclado, NULL);

    pthread_join(hiloDiscoDuro, NULL);
    pthread_join(hiloImpresora, NULL);
    pthread_join(hiloTeclado, NULL);

    pthread_cancel(hiloManejador);
    pthread_mutex_destroy(&mutexCola);
    pthread_cond_destroy(&condColaLlena);
    pthread_cond_destroy(&condColaVacia);

    printf("Simulación finalizada.\n");
    return 0;
}
```

-----

# Avanzados.

##  Explica cómo los sistemas operativos modernos optimizan las operaciones de entrada/salida con el uso de memoria caché.

Los sistemas operativos modernos optimizan las operaciones de entrada/salida (E/S) mediante el uso de memoria caché, un mecanismo esencial para reducir la brecha de rendimiento entre la CPU y los dispositivos de almacenamiento.
La memoria caché es una pequeña cantidad de almacenamiento rápido que se sitúa entre el procesador y los dispositivos más lentos, como los discos duros. Su objetivo principal es mantener los datos más utilizados o aquellos que probablemente se requerirán en el futuro, evitando accesos innecesarios a dispositivos lentos.
Cuando una aplicación solicita datos, el sistema operativo primero verifica si esos datos están en la caché. Si están disponibles, el sistema los entrega directamente, acelerando el proceso. En caso contrario, se produce un fallo en caché, lo que obliga al sistema a leer los datos del dispositivo de almacenamiento, cargarlos en la caché y luego proporcionarlos a la aplicación. Para mejorar la eficiencia de estas operaciones, los sistemas operativos emplean una técnica conocida como prefetching o lectura anticipada. Por ejemplo, si se detecta que un archivo está siendo leído de manera secuencial, se cargan bloques adicionales en la caché con la expectativa de que serán necesarios.
En cuanto a las escrituras, los datos generados por las aplicaciones pueden ser almacenados temporalmente en la caché antes de ser escritos en el dispositivo de almacenamiento. Este método, conocido como escritura diferida o write-back, permite agrupar múltiples operaciones de escritura en una sola transacción, reduciendo el número de accesos al dispositivo. Por otro lado, existe la técnica de escritura directa o write-through, que consiste en escribir los datos simultáneamente en la caché y en el almacenamiento físico para garantizar la consistencia de los datos, aunque a expensas de un menor rendimiento. Periódicamente, el sistema sincroniza los datos almacenados en la caché con el dispositivo subyacente mediante una operación conocida como flush, minimizando así la posible pérdida de información en caso de fallos.
Para administrar eficientemente la memoria caché, los sistemas operativos implementan algoritmos de reemplazo que determinan qué datos mantener y cuáles descartar cuando la caché está llena. Algoritmos como LRU (Least Recently Used), LFU (Least Frequently Used) y Clock son populares por su capacidad de seleccionar inteligentemente los datos menos necesarios para liberar espacio. Además, los sistemas operativos modernos utilizan técnicas como la caché de búfer, que almacena bloques de datos del disco, y la caché de páginas, que administra los datos de archivos y permite tratar las operaciones de E/S como operaciones de memoria, incrementando la eficiencia. Algunos sistemas unifican estas cachés para evitar duplicaciones y maximizar el uso de la memoria disponible. Además de los datos, se almacenan metadatos, como información sobre los archivos y directorios, para acelerar tareas como listar directorios o abrir archivos.
El uso de memoria caché ofrece múltiples ventajas. Reduce significativamente el tiempo de acceso a los datos, disminuye la carga en los dispositivos de almacenamiento, agrupa operaciones pequeñas en grandes transacciones y permite un mejor aprovechamiento de los recursos del sistema. Sin embargo, presenta desafíos como mantener la consistencia de los datos en caso de fallos, decidir qué datos conservar en la caché y manejar adecuadamente la memoria asignada para ella. Además, el tamaño de los archivos puede limitar los beneficios, especialmente en casos de archivos muy grandes que exceden la capacidad de la caché.
Por ejemplo, al guardar un documento en un procesador de texto, los cambios se escriben primero en la caché del disco y posteriormente se sincronizan con el almacenamiento físico. Si el usuario necesita volver a acceder al archivo, los datos pueden recuperarse directamente desde la caché, acelerando el proceso.
----------