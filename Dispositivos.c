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