/*
 * Ejercicio 2 - Triángulo de Pascal y expansión del polinomio (x+1)^n
 *
 * Funcionalidades:
 *   a) Genera los coeficientes de (x+1)^n usando el triángulo de Pascal
 *      con asignación dinámica de memoria (malloc/free)
 *   b) Evalúa f(x) = (x+1)^n paso a paso usando el polinomio generado
 *
 * Benchmark: mide el tiempo de generar n=100 en C (comparar con Python).
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * fila_pascal - reserva memoria y retorna la n-ésima fila del triángulo
 * de Pascal (n+1 elementos). El llamador es responsable de liberar la memoria.
 *
 * Fila 0: [1]
 * Fila 1: [1, 1]
 * Fila 2: [1, 2, 1]
 * ...
 *
 * Retorna NULL si la asignación de memoria falla.
 */
static long long *fila_pascal(int n)
{
    /* Solo se necesitan dos filas a la vez para construir iterativamente */
    long long *fila_anterior = malloc(sizeof(long long)); /* fila 0 */
    if (!fila_anterior) return NULL;
    fila_anterior[0] = 1;

    for (int i = 1; i <= n; i++) {
        long long *fila_actual = malloc((i + 1) * sizeof(long long));
        if (!fila_actual) { free(fila_anterior); return NULL; }

        fila_actual[0] = 1;
        for (int j = 1; j < i; j++) {
            /* Cada elemento es la suma de los dos de arriba */
            fila_actual[j] = fila_anterior[j - 1] + fila_anterior[j];
        }
        fila_actual[i] = 1;

        free(fila_anterior);   /* liberar fila anterior ya no necesaria */
        fila_anterior = fila_actual;
    }

    return fila_anterior; /* el llamador debe liberar esta memoria */
}

/*
 * imprimir_polinomio - muestra la forma expandida de (x+1)^n
 * usando los coeficientes. coeficientes[0] es el término líder (x^n).
 */
static void imprimir_polinomio(int n, const long long *coeficientes)
{
    printf("(x+1)^%d = ", n);
    if (n == 0) { printf("1\n"); return; }

    int primer_termino = 1;
    for (int i = 0; i <= n; i++) {
        int potencia    = n - i;
        long long coef  = coeficientes[i];

        if (!primer_termino) printf(" + ");
        primer_termino = 0;

        if (potencia == 0) {
            printf("%lld", coef);
        } else if (potencia == 1) {
            if (coef == 1) printf("x");
            else           printf("%lldx", coef);
        } else {
            if (coef == 1) printf("x^%d", potencia);
            else           printf("%lldx^%d", coef, potencia);
        }
    }
    printf("\n");
}

/*
 * evaluar_polinomio - evalúa f(x) = (x+1)^n paso a paso usando el
 * método de Horner aplicado a los coeficientes de Pascal.
 *
 * El polinomio es: c0*x^n + c1*x^(n-1) + ... + c(n-1)*x + cn
 * Horner: (((...(c0)*x + c1)*x + c2)*x + ...)
 *
 * Este método reduce la cantidad de multiplicaciones de O(n^2) a O(n).
 */
static long long evaluar_polinomio(int n, const long long *coeficientes,
                                   long long valor_x)
{
    printf("\nEvaluación de f(%lld) = (x+1)^%d paso a paso (Horner):\n",
           valor_x, n);

    if (n == 0) {
        printf("  f(%lld) = 1\n", valor_x);
        return 1;
    }

    long long acumulador = coeficientes[0];
    printf("  paso 0 : acumulador = %lld\n", acumulador);

    for (int i = 1; i <= n; i++) {
        acumulador = acumulador * valor_x + coeficientes[i];
        printf("  paso %d : acumulador = acumulador * %lld + %lld = %lld\n",
               i, valor_x, coeficientes[i], acumulador);
    }

    printf("  f(%lld) = %lld\n", valor_x, acumulador);
    return acumulador;
}

/* ------------------------------------------------------------------ */
/*  Funciones de benchmark                                              */
/* ------------------------------------------------------------------ */

/*
 * medir_tiempo_pascal - genera fila_pascal(n) repetidamente durante
 * 'repeticiones' iteraciones y retorna el tiempo total en milisegundos.
 */
static double medir_tiempo_pascal(int n, int repeticiones)
{
    struct timespec inicio, fin;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int i = 0; i < repeticiones; i++) {
        long long *fila = fila_pascal(n);
        free(fila); /* liberar memoria después de cada generación */
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);
    return (fin.tv_sec  - inicio.tv_sec)  * 1000.0
         + (fin.tv_nsec - inicio.tv_nsec) / 1e6;
}

/* ------------------------------------------------------------------ */
/*  Función principal                                                   */
/* ------------------------------------------------------------------ */
int main(void)
{
    printf("=== Triángulo de Pascal y Polinomio (x+1)^n ===\n\n");

    /* Mostrar las filas 0 a 7 con sus coeficientes */
    printf("--- Filas del triángulo de Pascal ---\n");
    for (int n = 0; n <= 7; n++) {
        long long *fila = fila_pascal(n);
        if (!fila) { fprintf(stderr, "Error: malloc falló\n"); return 1; }

        printf("(n=%d): ", n);
        for (int i = 0; i <= n; i++) {
            printf("%lld ", fila[i]);
        }
        printf("\n");
        free(fila);
    }

    printf("\n--- Expansiones del polinomio ---\n");
    for (int n = 0; n <= 7; n++) {
        long long *fila = fila_pascal(n);
        if (!fila) { fprintf(stderr, "Error: malloc falló\n"); return 1; }
        imprimir_polinomio(n, fila);
        free(fila);
    }

    /* Ejemplo de evaluación paso a paso */
    printf("\n--- Evaluación paso a paso ---\n");
    {
        int grado = 4;
        long long valor_x = 2;
        long long *fila = fila_pascal(grado);
        if (!fila) { fprintf(stderr, "Error: malloc falló\n"); return 1; }
        imprimir_polinomio(grado, fila);
        long long resultado = evaluar_polinomio(grado, fila, valor_x);

        /* Verificación directa para confirmar el resultado */
        long long verificacion = 1;
        for (int i = 0; i < grado; i++) verificacion *= (valor_x + 1);
        printf("  Verificación directa (x+1)^%d = %lld %s\n",
               grado, verificacion,
               resultado == verificacion ? "[OK]" : "[ERROR]");
        free(fila);
    }

    /* ---- Benchmark C: n=100, 1000 repeticiones ---- */
    int cantidad_repeticiones = 1000;
    printf("\n=== Benchmark: generar fila_pascal(100) × %d repeticiones ===\n",
           cantidad_repeticiones);
    double tiempo_ms = medir_tiempo_pascal(100, cantidad_repeticiones);
    printf("C - tiempo total   : %.4f ms\n", tiempo_ms);
    printf("C - tiempo promedio: %.6f ms/rep\n",
           tiempo_ms / cantidad_repeticiones);
    printf("\nComparar con la implementación Python (ver benchmark.py).\n");

    return 0;
}
