//Nota importante pal profe: este programa funciona nomás en linux, pero para que 
//Aparezca también en el repositorio lo agregué aquí en visualStudio. pero ps va a dar error si lo intenta correr aquí jeje.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <aio.h>
#include <errno.h>
#include <unistd.h>

#define TAMANO_BUFFER 1024

void manejarError(const char* mensaje) {
    perror(mensaje);
    exit(EXIT_FAILURE);
}

void verificarEstado(struct aiocb* aiocbp) {
    while (aio_error(aiocbp) == EINPROGRESS) {
        printf("Operación en progreso...\n");
        usleep(100000); 
    }

    int resultado = aio_return(aiocbp);
    if (resultado == -1) {
        manejarError("Error en la operación de E/S");
    } else {
        printf("Operación completada con éxito (%d bytes procesados).\n", resultado);
    }
}

int main() {
    const char* archivo = "archivo_async.txt";
    char bufferEscritura[TAMANO_BUFFER] = "Este es un ejemplo de escritura asincrónica.\n";
    char bufferLectura[TAMANO_BUFFER];

    int fd = open(archivo, O_CREAT | O_RDWR, 0644);
    if (fd == -1) {
        manejarError("Error al abrir el archivo");
    }

    struct aiocb aioEscritura;
    memset(&aioEscritura, 0, sizeof(struct aiocb));
    aioEscritura.aio_fildes = fd;
    aioEscritura.aio_buf = bufferEscritura;
    aioEscritura.aio_nbytes = strlen(bufferEscritura);
    aioEscritura.aio_offset = 0;

    if (aio_write(&aioEscritura) == -1) {
        manejarError("Error al iniciar la escritura asincrónica");
    }
    printf("Escritura asincrónica iniciada...\n");

    verificarEstado(&aioEscritura);

    struct aiocb aioLectura;
    memset(&aioLectura, 0, sizeof(struct aiocb));
    aioLectura.aio_fildes = fd;
    aioLectura.aio_buf = bufferLectura;
    aioLectura.aio_nbytes = TAMANO_BUFFER;
    aioLectura.aio_offset = 0;

    if (aio_read(&aioLectura) == -1) {
        manejarError("Error al iniciar la lectura asincrónica");
    }
    printf("Lectura asincrónica iniciada...\n");

    verificarEstado(&aioLectura);

    printf("Contenido leído: %s\n", bufferLectura);

    close(fd);

    return 0;
}