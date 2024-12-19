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