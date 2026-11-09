// Programa de prueba para el ejercicio 3.
// Incluye diversas palabras reservadas de C para verificar la traduccion.
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x;
    float y;
} Punto;

int sumar(int a, int b) {
    return a + b;
}

int main(void) {
    int i = 0;
    double pi = 3.14159;
    char letra = 'A';
    const int MAX = 10;
    unsigned int contador = 0;
    long total = 0;

    for (i = 0; i < MAX; i++) {
        if (i % 2 == 0) {
            contador++;
        } else {
            continue;
        }
    }

    int x = 5;
    switch (x) {
        case 1:
            break;
        case 5:
            total = sumar(x, (int)pi);
            break;
        default:
            break;
    }

    while (total > 0) {
        total--;
    }

    do {
        total++;
    } while (total < 3);

    static int cache = 0;
    void *ptr = malloc(sizeof(int));
    if (ptr == NULL) {
        return 1;
    }
    free(ptr);

    return 0;
}
