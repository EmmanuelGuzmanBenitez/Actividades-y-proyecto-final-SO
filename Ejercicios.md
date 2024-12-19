# Administración de memoria.
## 3.1 Política y filosofía.
### ¿Cuál es la diferencia entre fragmentación interna y externa?
La fragmentación interna ocurre cuando un bloque de memoria asignado a un proceso es más grande de lo necesario, dejando partes no utilizadas dentro del bloque. Aunque el espacio está reservado, no se puede usar para otros procesos. 
Esto afecta desperdiciando memoria dentro de los bloques asignados además de que afecta el rendimiento porque se reduce la cantidad de memoria disponible para otros procesos.

La fragmentación externa sucede cuando hay pequeños fragmentos de memoria libres dispersos entre bloques asignados. Estos fragmentos no son lo suficientemente grandes como para satisfacer nuevas solicitudes de memoria, aunque la suma total podría ser suficiente.
Esto afecta a la memoria generando más tiempo de procesamiento para encontrar bloques adecuados.

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
