#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

#define MAX_INTERRUPTS 5

typedef struct {
    int interrupt_id;     
    char description[50]; 
} Interrupt;

Interrupt interrupt_queue[MAX_INTERRUPTS]; 
int interrupt_count = 0;                   

jmp_buf env;

void trigger_interrupt(int interrupt_id, const char* description) {
    if (interrupt_count < MAX_INTERRUPTS) {
        interrupt_queue[interrupt_count].interrupt_id = interrupt_id;
        snprintf(interrupt_queue[interrupt_count].description, sizeof(interrupt_queue[interrupt_count].description), "%s", description);
        interrupt_count++;
        printf("¡Interrupción %d: %s generada!\n", interrupt_id, description);
    } else {
        printf("Error: Cola de interrupciones llena.\n");
    }
}

void handle_interrupt() {
    while (interrupt_count > 0) {
        Interrupt interrupt = interrupt_queue[0];
        printf("Manejando interrupción ID %d: %s\n", interrupt.interrupt_id, interrupt.description);

        for (int i = 1; i < interrupt_count; i++) {
            interrupt_queue[i - 1] = interrupt_queue[i];
        }
        interrupt_count--;

        if (setjmp(env) == 0) {
            printf("Contexto guardado. Retornando al punto de ejecución previo...\n");
            longjmp(env, 1); 
        }
    }
}

void simulate_process() {
    printf("Proceso en ejecución. Esperando interrupciones...\n");
    while (1) {
        sleep(2); 
        printf("El proceso está trabajando...\n");

        if (rand() % 5 == 0) {
            trigger_interrupt(rand() % 100, "Evento inesperado");
        }

        if (interrupt_count > 0) {
            handle_interrupt();
        }
    }
}

int main() {
    
    signal(SIGINT, SIG_IGN); 

    simulate_process();

    return 0;
}