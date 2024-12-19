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