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