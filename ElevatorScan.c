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