#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_PAGINAS 10    
#define TAMANO_PAGINA 256 
#define NUM_MARCOS 3      
#define NUM_ACCESOS 20  

typedef struct {
    int pagina;    
    int tiempoUso; 
    bool valido;  
} Marco;

Marco memoriaFisica[NUM_MARCOS];

void inicializarMemoria() {
    for (int i = 0; i < NUM_MARCOS; i++) {
        memoriaFisica[i].pagina = -1; 
        memoriaFisica[i].valido = false;
        memoriaFisica[i].tiempoUso = -1; 
    }
}

int encontrarLRU() {
    int lruIndice = 0, lruTiempo = memoriaFisica[0].tiempoUso;
    for (int i = 1; i < NUM_MARCOS; i++) {
        if (memoriaFisica[i].tiempoUso < lruTiempo) {
            lruIndice = i;
            lruTiempo = memoriaFisica[i].tiempoUso;
        }
    }
    return lruIndice;
}

void accederMemoria(int direccionLogica, int tiempo) {
    int pagina = direccionLogica / TAMANO_PAGINA;
    int desplazamiento = direccionLogica % TAMANO_PAGINA;

    if (pagina < 0 || pagina >= NUM_PAGINAS) {
        printf("Error: Dirección lógica %d fuera de rango.\n", direccionLogica);
        return;
    }

    for (int i = 0; i < NUM_MARCOS; i++) {
        if (memoriaFisica[i].valido && memoriaFisica[i].pagina == pagina) {
            printf("Acceso exitoso: Dirección lógica %d -> Página %d -> Marco %d\n",
                   direccionLogica, pagina, i);
            memoriaFisica[i].tiempoUso = tiempo; 
            return;
        }
    }
   
    printf("Fallo de página: Página %d no está en memoria física.\n", pagina);
   
    int marcoReemplazo = encontrarLRU();
    if (memoriaFisica[marcoReemplazo].valido) {
        printf("Reemplazando Página %d con Página %d en Marco %d.\n",
               memoriaFisica[marcoReemplazo].pagina, pagina, marcoReemplazo);
    } else {
        printf("Cargando Página %d en Marco %d.\n", pagina, marcoReemplazo);
    }

    memoriaFisica[marcoReemplazo].pagina = pagina;
    memoriaFisica[marcoReemplazo].valido = true;
    memoriaFisica[marcoReemplazo].tiempoUso = tiempo;
}

int main() {
    srand(time(NULL));

    inicializarMemoria();

    printf("--- Simulación de reemplazo de página LRU ---\n");

    for (int tiempo = 0; tiempo < NUM_ACCESOS; tiempo++) {
        int direccionLogica = rand() % (NUM_PAGINAS * TAMANO_PAGINA);
        accederMemoria(direccionLogica, tiempo);
    }

    return 0;
}