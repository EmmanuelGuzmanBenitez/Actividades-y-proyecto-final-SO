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