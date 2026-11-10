/*
 * programa_ejemplo.c
 *
 * Programa de ejemplo en C que se usa como entrada para el Ejercicio 3.
 * Contiene deliberadamente una variedad de palabras reservadas del lenguaje
 * para demostrar la detección y traducción al español.
 */

#include <stdio.h>
#include <stdlib.h>

/* Enumeración de estados posibles */
typedef enum {
    ACTIVO,
    INACTIVO,
    SUSPENDIDO
} EstadoSistema;

/* Estructura de un estudiante */
typedef struct {
    char   nombre[50];
    int    edad;
    float  promedio;
    int    aprobado;
} Estudiante;

/*
 * clasificar_promedio - clasifica el rendimiento según el promedio.
 * Retorna una cadena de texto con la categoría.
 */
static const char *clasificar_promedio(float promedio)
{
    if (promedio >= 18.0f) {
        return "Sobresaliente";
    } else if (promedio >= 14.0f) {
        return "Bueno";
    } else if (promedio >= 10.0f) {
        return "Regular";
    } else {
        return "Reprobado";
    }
}

/*
 * procesar_estudiantes - recibe un arreglo de estudiantes asignado
 * dinámicamente, lo procesa y libera la memoria al finalizar.
 */
static void procesar_estudiantes(int cantidad)
{
    /* Reservar memoria dinámica para el arreglo */
    Estudiante *lista = malloc(cantidad * sizeof(Estudiante));
    if (lista == NULL) {
        fprintf(stderr, "Error: no se pudo reservar memoria.\n");
        return;
    }

    /* Inicializar con datos de prueba */
    for (int i = 0; i < cantidad; i++) {
        lista[i].edad     = 20 + i;
        lista[i].promedio = 10.0f + (float)(i * 2);
        lista[i].aprobado = lista[i].promedio >= 10.0f ? 1 : 0;
    }

    /* Mostrar resultados usando switch para la clasificación */
    printf("%-5s %-6s %-10s %-15s\n", "N°", "Edad", "Promedio", "Clasificación");
    printf("----------------------------------------------\n");

    for (int i = 0; i < cantidad; i++) {
        printf("%-5d %-6d %-10.1f %-15s\n",
               i + 1,
               lista[i].edad,
               lista[i].promedio,
               clasificar_promedio(lista[i].promedio));
    }

    /* Contar aprobados con un while */
    int indice    = 0;
    int aprobados = 0;
    while (indice < cantidad) {
        if (lista[indice].aprobado) {
            aprobados++;
        }
        indice++;
    }
    printf("\nTotal aprobados: %d de %d\n", aprobados, cantidad);

    /* Liberar la memoria reservada */
    free(lista);
}

/* Función auxiliar con do-while y goto de ejemplo */
static int calcular_factorial(int n)
{
    if (n < 0) goto error;

    int resultado = 1;
    int contador  = n;
    do {
        resultado *= contador > 0 ? contador : 1;
        contador--;
    } while (contador > 0);

    return resultado;

error:
    fprintf(stderr, "Error: factorial no definido para negativos.\n");
    return -1;
}

/* Función con union para demostrar más palabras clave */
static void demostrar_union(void)
{
    union {
        int    entero;
        float  flotante;
        char   caracter;
    } dato;

    dato.entero = 65;
    printf("Union - como entero: %d, como char: %c\n",
           dato.entero, dato.caracter);
}

/* Función que usa variables de distintos tipos */
static void demostrar_tipos(void)
{
    signed   int   entero_con_signo  = -42;
    unsigned int   entero_sin_signo  = 42u;
    long           entero_largo      = 1000000L;
    short          entero_corto      = 32;
    double         doble_precision   = 3.14159265358979;
    const char    *cadena_constante  = "constante";
    volatile int   registro_volatil  = 0;

    printf("signed:   %d\n",  entero_con_signo);
    printf("unsigned: %u\n",  entero_sin_signo);
    printf("long:     %ld\n", entero_largo);
    printf("short:    %d\n",  entero_corto);
    printf("double:   %.5f\n", doble_precision);
    printf("const:    %s\n",  cadena_constante);
    printf("volatile: %d\n",  registro_volatil);

    printf("sizeof(int) = %zu bytes\n", sizeof(int));
}

/* ------------------------------------------------------------------ */
/*  Función principal                                                   */
/* ------------------------------------------------------------------ */
int main(void)
{
    printf("=== Programa de Ejemplo en C ===\n\n");

    /* Demostrar tipos de datos */
    printf("--- Tipos de datos ---\n");
    demostrar_tipos();

    /* Procesar lista de estudiantes */
    printf("\n--- Lista de Estudiantes ---\n");
    procesar_estudiantes(5);

    /* Calcular factorial usando do-while */
    printf("\n--- Factorial ---\n");
    for (int n = 0; n <= 7; n++) {
        int resultado = calcular_factorial(n);
        if (resultado >= 0) {
            printf("%d! = %d\n", n, resultado);
        }
    }

    /* Demostrar union */
    printf("\n--- Union ---\n");
    demostrar_union();

    /* Demostrar switch / case / break / default */
    printf("\n--- Switch / Case ---\n");
    EstadoSistema estado = ACTIVO;
    switch (estado) {
        case ACTIVO:
            printf("El sistema esta activo.\n");
            break;
        case INACTIVO:
            printf("El sistema esta inactivo.\n");
            break;
        case SUSPENDIDO:
            printf("El sistema esta suspendido.\n");
            break;
        default:
            printf("Estado desconocido.\n");
            break;
    }

    /* Demostrar continue dentro de un for */
    printf("\n--- Numeros impares del 1 al 10 ---\n");
    for (int i = 1; i <= 10; i++) {
        if (i % 2 == 0) continue;
        printf("%d ", i);
    }
    printf("\n");

    return 0;
}
