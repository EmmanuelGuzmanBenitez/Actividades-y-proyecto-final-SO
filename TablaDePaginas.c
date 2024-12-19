#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define NUM_PAGINAS 10    
#define TAMANO_PAGINA 256 
#define NUM_MARCOS 5      
#define NUM_ACCESOS 20    

typedef struct {
    int pagina;   
    bool valido;  
} Marco;


int tablaPaginas[NUM_PAGINAS];


Marco memoriaFisica[NUM_MARCOS];


void inicializarTabla() {
    for (int i = 0; i < NUM_PAGINAS; i++) {
        tablaPaginas[i] = -1; 
    }
    for (int i = 0; i < NUM_MARCOS; i++) {
        memoriaFisica[i].pagina = -1;
        memoriaFisica[i].valido = false;
    }
}


void accederMemoria(int direccionLogica) {
    int pagina = direccionLogica / TAMANO_PAGINA;
    int desplazamiento = direccionLogica % TAMANO_PAGINA;

    if (pagina < 0 || pagina >= NUM_PAGINAS) {
        printf("Error: Dirección lógica %d fuera de rango.\n", direccionLogica);
        return;
    }

    int marco = tablaPaginas[pagina];
    if (marco != -1 && memoriaFisica[marco].valido) {
        
        int direccionFisica = marco * TAMANO_PAGINA + desplazamiento;
        printf("Acceso exitoso: Dirección lógica %d -> Página %d -> Dirección física %d\n", 
               direccionLogica, pagina, direccionFisica);
    } else {
        
        printf("Fallo de página: Página %d no está en memoria física.\n", pagina);

        
        static int siguienteMarco = 0;
        int marcoReemplazo = siguienteMarco;
        siguienteMarco = (siguienteMarco + 1) % NUM_MARCOS;

        if (memoriaFisica[marcoReemplazo].valido) {
            int paginaAntigua = memoriaFisica[marcoReemplazo].pagina;
            tablaPaginas[paginaAntigua] = -1; 
            printf("Reemplazando Página %d con Página %d en Marco %d.\n", 
                   paginaAntigua, pagina, marcoReemplazo);
        } else {
            printf("Cargando Página %d en Marco %d.\n", pagina, marcoReemplazo);
        }

        memoriaFisica[marcoReemplazo].pagina = pagina;
        memoriaFisica[marcoReemplazo].valido = true;
        tablaPaginas[pagina] = marcoReemplazo;
    }
}

int main() {
    srand(time(NULL)); 

    inicializarTabla();

    printf("--- Simulación de tabla de páginas ---\n");
    for (int i = 0; i < NUM_ACCESOS; i++) {
        int direccionLogica = rand() % (NUM_PAGINAS * TAMANO_PAGINA);
        accederMemoria(direccionLogica);
    }

    return 0;
}