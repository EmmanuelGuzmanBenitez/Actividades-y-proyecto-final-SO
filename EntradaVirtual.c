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