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