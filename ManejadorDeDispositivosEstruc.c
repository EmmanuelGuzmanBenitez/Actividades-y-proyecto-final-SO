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