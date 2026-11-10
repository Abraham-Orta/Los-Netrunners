/*
 * Ejercicio 1 - Validador de movimientos de caballo en un tablero de ajedrez
 *
 * Notación del tablero:
 *   Filas:    1-8 (entero)
 *   Columnas: a-h (letra)
 *
 * Notación de movimiento: K<columna><fila>  ejemplo: Kc2
 *
 * El programa valida si cada movimiento de una secuencia es un
 * movimiento legal de caballo, deteniéndose en el primer movimiento
 * ilegal encontrado.
 *
 * Rendimiento: ejecuta la secuencia de 500 movimientos 10 veces y
 * reporta el tiempo transcurrido.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Letras de columna válidas */
static const char COLUMNAS[] = "abcdefgh";

/*
 * indice_columna - convierte la letra de columna a índice base 0 (a=0, h=7).
 * Retorna -1 si la letra no es una columna válida.
 */
static int indice_columna(char letra)
{
    for (int i = 0; i < 8; i++) {
        if (COLUMNAS[i] == letra) return i;
    }
    return -1;
}

/*
 * es_movimiento_valido_caballo - retorna 1 si el movimiento de (col1, fila1)
 * a (col2, fila2) es legal para un caballo (forma de L: 2+1 ó 1+2 casillas),
 * 0 en caso contrario.
 */
static int es_movimiento_valido_caballo(int col1, int fila1, int col2, int fila2)
{
    int diferencia_col  = abs(col2 - col1);
    int diferencia_fila = abs(fila2 - fila1);
    return (diferencia_col == 2 && diferencia_fila == 1)
        || (diferencia_col == 1 && diferencia_fila == 2);
}

/*
 * validar_secuencia - valida los n movimientos del arreglo 'movimientos'
 * partiendo de la posición inicial (col_actual, fila_actual).
 *
 * movimientos : arreglo de cadenas con formato "K<col><fila>"
 * cantidad    : número de movimientos
 * mensaje     : buffer provisto por el llamador para el resultado legible
 * tam_buffer  : tamaño del buffer 'mensaje'
 */
static void validar_secuencia(const char *posicion_inicial,
                               char **movimientos, int cantidad,
                               char *mensaje, int tam_buffer)
{
    /* Parsear posición inicial: letra + dígito, por ejemplo "a1" */
    int col_actual  = indice_columna(posicion_inicial[0]);
    int fila_actual = posicion_inicial[1] - '1'; /* base 0 */

    for (int i = 0; i < cantidad; i++) {
        const char *movimiento = movimientos[i];

        /* El identificador de pieza debe ser 'K' (caballo en inglés) */
        if (movimiento[0] != 'K') {
            snprintf(mensaje, tam_buffer,
                     "INCORRECTO falla letra %c no pertenece a un caballo.",
                     movimiento[0]);
            return;
        }

        /* La columna debe ser una letra válida */
        int siguiente_col = indice_columna(movimiento[1]);
        if (siguiente_col < 0) {
            snprintf(mensaje, tam_buffer,
                     "INCORRECTO falla en el movimiento %d (columna invalida).",
                     i + 1);
            return;
        }

        /* La fila debe ser un dígito entre '1' y '8' */
        if (movimiento[2] < '1' || movimiento[2] > '8') {
            snprintf(mensaje, tam_buffer,
                     "INCORRECTO falla en el movimiento %d (fila invalida).",
                     i + 1);
            return;
        }
        int siguiente_fila = movimiento[2] - '1'; /* base 0 */

        /* Verificar que el movimiento sea válido para un caballo */
        if (!es_movimiento_valido_caballo(col_actual, fila_actual,
                                          siguiente_col, siguiente_fila)) {
            snprintf(mensaje, tam_buffer,
                     "INCORRECTO falla en el movimiento %d.", i + 1);
            return;
        }

        /* Actualizar posición actual */
        col_actual  = siguiente_col;
        fila_actual = siguiente_fila;
    }

    snprintf(mensaje, tam_buffer, "movimiento CORRECTO.");
}

/* ------------------------------------------------------------------ */
/*  Datos para el benchmark                                             */
/* ------------------------------------------------------------------ */
static const char *POSICION_INICIAL_BENCH = "a2";

static const char *MOVIMIENTOS_BENCH[] = {
    "Kb4","Ka6","Kc5","Ka4","Kc3","Ka2","Kc1","Ke2","Kg1","Kh3",
    "Kg5","Ke6","Kc7","Ka8","Kc7","Kb5","Kd6","Kb7","Ka5","Kc4",
    "Ka3","Kc2","Ka1","Kc2","Kb4","Ka6","Kb8","Kd7","Kb6","Ka8",
    "Kc7","Ka6","Kc5","Ka4","Kc3","Ka2","Kc1","Ke2","Kg3","Kh5",
    "Kg7","Ke8","Kc7","Ka8","Kc7","Kb5","Kd4","Kb3","Ka1","Kc2",
    "Ka3","Kc4","Ka5","Kc6","Ka7","Kc8","Ka7","Kc6","Ka5","Kc4",
    "Ka3","Kc2","Ka1","Kc2","Kb4","Kd5","Kb6","Ka8","Kc7","Ka6",
    "Kc5","Ka4","Kc3","Ka2","Kc1","Ke2","Kg1","Kh3","Kg5","Ke6",
    "Kc5","Ka4","Kc3","Ka2","Kc1","Ke2","Kg3","Kh5","Kg7","Ke8",
    "Kc7","Ka8","Kc7","Kb5","Kd6","Kb7","Ka5","Kc4","Ka3","Kc2",
    "Ka1","Kc2","Kb4","Ka6","Kb8","Kd7","Kb6","Ka8","Kc7","Ka6",
    "Kc5","Ka4","Kc3","Ka2","Kc1","Ke2","Kg3","Kh5","Kg7","Ke8",
    "Kc7","Ka8","Kc7","Kb5","Kd4","Kb3","Ka1","Kc2","Ka3","Kc4",
    "Ka5","Kc6","Ka7","Kc8","Ka7","Kc6","Ka5","Kc4","Ka3","Kc2",
    "Ka1","Kc2","Kb4","Kd5","Kb6","Ka8","Kc7","Ka6","Kc5","Ka4",
    "Kc3","Ka2","Kc1","Ke2","Kg3","Kh5","Kg7","Ke8","Kc7","Ka8",
    "Kc7","Kb5","Kd4","Kb3","Ka1","Kc2","Ka3","Kc4","Ka5","Kc6",
    "Ka7","Kc8","Ka7","Kc6","Ka5","Kc4","Ka3","Kc2","Ka1","Kc2",
    "Kb4","Kd5","Kb6","Ka8","Kc7","Ka6","Kc5","Ka4","Kc3","Ka2",
    "Kc1","Ke2","Kg1","Kh3","Kg5","Ke6","Kc5","Ka4","Kc3","Ka2",
    "Kc1","Ke2","Kg3","Kh5","Kg7","Ke8","Kc7","Ka8","Kc7","Kb5",
    "Kd6","Kb7","Ka5","Kc4","Ka3","Kc2","Ka1","Kc2","Kb4","Ka6",
    "Kb8","Kd7","Kb6","Ka8","Kc7","Ka6","Kc5","Ka4","Kc3","Ka2",
    "Kc1","Ke2","Kg3","Kh5","Kg7","Ke8","Kc7","Ka8","Kc7","Kb5",
    "Kd4","Kb3","Ka1","Kc2","Ka3","Kc4","Ka5","Kc6","Ka7","Kc8",
    "Ka7","Kc6","Ka5","Kc4","Ka3","Kc2","Ka1","Kc2","Kb4","Kd5",
    "Kb6","Ka8","Kc7","Ka6","Kc5","Ka4","Kc3","Ka2","Kc1","Ke2",
    "Kg1","Kh3","Kg5","Ke6","Kc5","Ka4","Kc3","Ka2","Kc1","Ke2",
    "Kg3","Kh5","Kg7","Ke8","Kc7","Ka8","Kc7","Kb5","Kd6","Kb7",
    "Ka5","Kc4","Ka3","Kc2","Ka1","Kc2","Kb4","Ka6","Kb8","Kd7",
    "Kb6","Ka8","Kc7","Ka6","Kc5","Ka4","Kc3","Ka2","Kc1","Ke2",
    "Kg3","Kh5","Kg7","Ke8","Kc7","Ka8","Kc7","Kb5","Kd4","Kb3",
    "Ka1","Kc2","Ka3","Kc4","Ka5","Kc6","Ka7","Kc8","Ka7","Kc6",
    "Ka5","Kc4","Ka3","Kc2","Ka1","Kc2","Kb4","Kd5","Kb6","Ka8",
    "Kc7","Ka6","Kc5","Ka4","Kc3","Ka2","Kc1","Ke2","Kg1","Kh3",
    "Kg5","Ke6","Kc5","Ka4","Kc3","Ka2","Kc1","Ke2","Kg3","Kh5",
    "Kg7","Ke8","Kc7","Ka8","Kc7","Kb5","Kd6","Kb7","Ka5","Kc4",
    "Ka3","Kc2","Ka1","Kc2","Kb4","Ka6","Kb8","Kd7","Kb6","Ka8",
    "Kc7","Ka6","Kc5","Ka4","Kc3","Ka2","Kc1","Ke2","Kg3","Kh5",
    "Kg7","Ke8","Kc7","Ka8","Kc7","Kb5","Kd4","Kb3","Ka1","Kc2",
    "Ka3","Kc4","Ka5","Kc6","Ka7","Kc8","Ka7","Kc6","Ka5","Kc4",
    "Ka3","Kc2","Ka1","Kc2","Kb4","Kd5","Kb6","Ka8","Kc7","Ka6",
    "Kc5","Ka4","Kc3","Ka2","Kc1","Ke2","Kg1","Kh3","Kg5","Ke6",
    "Kc5","Ka4","Kc3","Ka2","Kc1","Ke2","Kg3","Kh5","Kg7","Ke8",
    "Kc7","Ka8","Kc7","Kb5","Kd6","Kb7","Ka5","Kc4","Ka3","Kc2",
    "Ka1","Kc2","Kb4","Ka6","Kb8","Kd7","Kb6","Ka8","Kc7","Ka6",
    "Kc5","Ka4","Kc3","Ka2","Kc1","Ke2","Kg3","Kh5","Kg7","Ke8",
    "Kc7","Ka8","Kc7","Kb5","Kd4","Kb3","Ka1","Kc2","Ka3","Kc4",
    "Ka5","Kc6","Ka7","Kc8","Ka7","Kc6","Ka5","Kc4","Ka3","Kc2",
    "Ka1","Kc2","Kb4","Kd5","Kb6","Ka8","Kc7","Ka6","Kc5","Ka4"
};

#define TOTAL_MOVIMIENTOS_BENCH 500
#define REPETICIONES_BENCH      10

/* ------------------------------------------------------------------ */
/*  Función principal                                                   */
/* ------------------------------------------------------------------ */
int main(void)
{
    /* ----- Casos de prueba del enunciado ----- */
    struct {
        const char *inicio;
        int         cantidad;
        const char *cadena_movimientos;
    } casos[] = {
        {"a1", 4,  "Kc2 Kd4 Kb3 Kc5"},
        {"a1", 4,  "Kc2 Kd4 Kb4 Kc5"},
        {"a2", 5,  "Kc3 Ke4 Kf6 Ke8 Kg7"},
        {"a2", 5,  "Kc8 Ke4 Kf6 Ke8 Kg7"},
        {"a8", 2,  "Kb6 Kd7"},
        {"a8", 2,  "Kb6 Kb7"},
        {"a1", 10, "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Kb4"},
        {"a1", 10, "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4"},
        {"a1", 10, "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4"},
    };
    int total_casos = (int)(sizeof(casos) / sizeof(casos[0]));

    printf("=== Validador de Movimientos de Caballo ===\n\n");

    for (int c = 0; c < total_casos; c++) {
        /* Separar la cadena de movimientos en un arreglo de punteros */
        char buffer[1024];
        strncpy(buffer, casos[c].cadena_movimientos, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';

        char *punteros_movimientos[64];
        int   cantidad = 0;
        char *token = strtok(buffer, " ");
        while (token && cantidad < 64) {
            punteros_movimientos[cantidad++] = token;
            token = strtok(NULL, " ");
        }

        char resultado[256];
        validar_secuencia(casos[c].inicio, punteros_movimientos,
                          casos[c].cantidad, resultado, sizeof(resultado));

        printf("Ak = %s, n = %d, C = %s\n  -> %s\n\n",
               casos[c].inicio, casos[c].cantidad,
               casos[c].cadena_movimientos, resultado);
    }

    /* ----- Benchmark de rendimiento: 500 movimientos × 10 repeticiones ----- */
    printf("=== Benchmark: %d movimientos × %d repeticiones ===\n",
           TOTAL_MOVIMIENTOS_BENCH, REPETICIONES_BENCH);

    char **movimientos_bench = (char **)MOVIMIENTOS_BENCH;

    /* Pasada de calentamiento (no se mide) */
    char resultado_bench[256];
    validar_secuencia(POSICION_INICIAL_BENCH, movimientos_bench,
                      TOTAL_MOVIMIENTOS_BENCH, resultado_bench,
                      sizeof(resultado_bench));

    /* Medición de tiempo con reloj de alta resolución */
    struct timespec inicio_tiempo, fin_tiempo;
    clock_gettime(CLOCK_MONOTONIC, &inicio_tiempo);

    for (int rep = 0; rep < REPETICIONES_BENCH; rep++) {
        validar_secuencia(POSICION_INICIAL_BENCH, movimientos_bench,
                          TOTAL_MOVIMIENTOS_BENCH, resultado_bench,
                          sizeof(resultado_bench));
    }

    clock_gettime(CLOCK_MONOTONIC, &fin_tiempo);

    double tiempo_total_ms =
        (fin_tiempo.tv_sec  - inicio_tiempo.tv_sec)  * 1000.0 +
        (fin_tiempo.tv_nsec - inicio_tiempo.tv_nsec) / 1e6;

    printf("Resultado de la secuencia: %s\n", resultado_bench);
    printf("Tiempo total  : %.4f ms\n", tiempo_total_ms);
    printf("Promedio/rep  : %.4f ms\n", tiempo_total_ms / REPETICIONES_BENCH);

    return 0;
}
