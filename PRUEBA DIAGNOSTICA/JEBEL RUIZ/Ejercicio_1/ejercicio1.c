// Prueba Diagnóstica - Ejercicio 1 (Lenguaje C)
//Validación de movimientos del caballo de ajedrez en tablero 8x8.
 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#define TABLERO_MIN_COL 'a'
#define TABLERO_MAX_COL 'h'
#define TABLERO_MIN_FIL '1'
#define TABLERO_MAX_FIL '8'

typedef struct {
    int col; 
    int fil; 
} Casilla;

/* Parsea una casilla de 2 caracteres, ej: "a1" */
int parse_casilla(const char *str, Casilla *out) {
    if (!str || strlen(str) != 2) return 0;
    char c = tolower((unsigned char)str[0]);
    char f = str[1];
    if (c < TABLERO_MIN_COL || c > TABLERO_MAX_COL) return 0;
    if (f < TABLERO_MIN_FIL || f > TABLERO_MAX_FIL) return 0;
    out->col = c - TABLERO_MIN_COL;
    out->fil = f - TABLERO_MIN_FIL;
    return 1;
}

/* Valida si el desplazamiento corresponde a la 'L' del caballo */
int es_movimiento_caballo(Casilla origen, Casilla destino) {
    int dc = abs(origen.col - destino.col);
    int df = abs(origen.fil - destino.fil);
    return (dc == 1 && df == 2) || (dc == 2 && df == 1);
}

/*
 * Valida la secuencia de movimientos en C.
 * Retorna: 1 si es correcto, 0 si hay error.
 * Llena mensaje_salida con la explicación exacta según el enunciado.
 */
int verificar_movimientos(const char *pos_inicial, const char *cadena_movimientos, char *mensaje_salida, size_t max_len) {
    Casilla actual;
    if (!parse_casilla(pos_inicial, &actual)) {
        snprintf(mensaje_salida, max_len, "ERROR: Casilla inicial '%s' invalida.", pos_inicial);
        return 0;
    }

    /* Usamos memoria dinámica para copiar la cadena y tokenizarla de forma segura */
    size_t len = strlen(cadena_movimientos);
    char *copia = (char *)malloc(len + 1);
    if (!copia) {
        snprintf(mensaje_salida, max_len, "ERROR: Falla de asignacion de memoria dinamica.");
        return 0;
    }
    strcpy(copia, cadena_movimientos);

    int mov_num = 0;
    char *token = strtok(copia, " \t\r\n");
    int es_valido = 1;

    while (token != NULL) {
        mov_num++;
        size_t tok_len = strlen(token);

        if (tok_len != 3) {
            snprintf(mensaje_salida, max_len, "INCORRECTO falla en el movimiento %d (formato invalido)", mov_num);
            es_valido = 0;
            break;
        }

        char pieza = token[0];
        if (pieza != 'K') {
            snprintf(mensaje_salida, max_len, "movimiento INCORRECTO falla letra %c no pertenece a un caballo.", pieza);
            es_valido = 0;
            break;
        }

        Casilla destino;
        if (!parse_casilla(token + 1, &destino)) {
            snprintf(mensaje_salida, max_len, "INCORRECTO falla en el movimiento %d (casilla fuera de tablero)", mov_num);
            es_valido = 0;
            break;
        }

        if (!es_movimiento_caballo(actual, destino)) {
            snprintf(mensaje_salida, max_len, "INCORRECTO falla en el movimiento %d.", mov_num);
            es_valido = 0;
            break;
        }

        actual = destino;
        token = strtok(NULL, " \t\r\n");
    }

    if (es_valido) {
        snprintf(mensaje_salida, max_len, "movimiento CORRECTO.");
    }

    free(copia);
    return es_valido;
}

// Secuencia del PDF 
const char *MOVS_PDF = 
    "Kb4 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 "
    "Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 "
    "Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 "
    "Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 "
    "Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 "
    "Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 "
    "Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 "
    "Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 "
    "Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 "
    "Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 "
    "Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 "
    "Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
    "Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 "
    "Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 "
    "Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 "
    "Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 "
    "Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 "
    "Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 "
    "Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 "
    "Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 "
    "Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 "
    "Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 "
    "Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 "
    "Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 "
    "Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 ";

void ejecutar_casos_ejemplo() {
    printf("======================================================================\n");
    printf("          EJERCICIO 1 (C): VALIDACION DE CASOS DE EJEMPLO DEL PDF     \n");
    printf("======================================================================\n");

    struct {
        const char *ak;
        int n;
        const char *c;
        const char *esperado;
    } casos[] = {
        {"a1", 4, "Kc2 Kd4 Kb3 Kc5", "movimiento CORRECTO."},
        {"a1", 4, "Kc2 Kd4 Kb4 Kc5", "INCORRECTO falla en el movimiento 3."},
        {"a2", 5, "Kc3 Ke4 Kf6 Ke8 Kg7", "movimiento CORRECTO."},
        {"a2", 5, "Kc8 Ke4 Kf6 Ke8 Kg7", "INCORRECTO falla en el movimiento 1."},
        {"a8", 2, "Kb6 Kd7", "movimiento CORRECTO."},
        {"a8", 2, "Kb6 Kb7", "INCORRECTO falla en el movimiento 2."},
        {"a1", 10, "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Kb4", "movimiento CORRECTO."},
        {"a1", 10, "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4", "movimiento INCORRECTO falla letra Q no pertenece a un caballo."},
        {"a1", 10, "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4", "movimiento INCORRECTO falla en el movimiento 9"}
    };

    int total = sizeof(casos) / sizeof(casos[0]);
    char msg[256];

    for (int i = 0; i < total; i++) {
        verificar_movimientos(casos[i].ak, casos[i].c, msg, sizeof(msg));
        printf("Ejemplo %d: Ak = %s, n = %d\n", i + 1, casos[i].ak, casos[i].n);
        printf("  Cadena:   %s\n", casos[i].c);
        printf("  Esperado: %s\n", casos[i].esperado);
        printf("  Obtenido: %s\n\n", msg);
    }
}

char *cargar_archivo_texto(const char *nombre) {
    const char *rutas[] = {
        nombre,
        "movimientos_500.txt",
        "Ejercicio_1/movimientos_500.txt",
        "JEBEL RUIZ/Ejercicio_1/movimientos_500.txt",
        "../movimientos_500.txt"
    };
    int n_rutas = sizeof(rutas) / sizeof(rutas[0]);
    FILE *f = NULL;
    for (int i = 0; i < n_rutas; i++) {
        f = fopen(rutas[i], "rb");
        if (f) break;
    }
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buf = (char *)malloc(sz + 1);
    if (buf) {
        size_t leidos = fread(buf, 1, sz, f);
        buf[leidos] = '\0';
    }
    fclose(f);
    return buf;
}

void ejecutar_benchmark() {
    printf("======================================================================\n");
    printf("     BENCHMARK EJERCICIO 1 (C): 10 VERIFICACIONES SECUENCIA 500 MOVS  \n");
    printf("======================================================================\n");

    LARGE_INTEGER freq, t_start, t_end;
    QueryPerformanceFrequency(&freq);

    char msg[256];
    const char *ak = "a2";
    int iteraciones = 10;
    double tiempos[10];
    double total_ms = 0.0;

    char *sec_din = cargar_archivo_texto("movimientos_500.txt");
    const char *sec = sec_din ? sec_din : MOVS_PDF;

    printf("Posicion inicial Ak: %s\n", ak);
    verificar_movimientos(ak, sec, msg, sizeof(msg));
    printf("Resultado inicial: %s\n\n", msg);

    for (int i = 0; i < iteraciones; i++) {
        QueryPerformanceCounter(&t_start);
        verificar_movimientos(ak, sec, msg, sizeof(msg));
        QueryPerformanceCounter(&t_end);

        double ms = (double)(t_end.QuadPart - t_start.QuadPart) * 1000.0 / (double)freq.QuadPart;
        tiempos[i] = ms;
        total_ms += ms;
        printf("  Iteracion %2d: %.5f ms -> %s\n", i + 1, ms, msg);
    }

    double promedio_ms = total_ms / iteraciones;
    printf("\nTiempo total (10 iteraciones): %.5f ms (%.7f s)\n", total_ms, total_ms / 1000.0);
    printf("Tiempo promedio por iteracion: %.5f ms (%.7f s)\n", promedio_ms, promedio_ms / 1000.0);

    char rep_buf[4096];
    int offset = snprintf(rep_buf, sizeof(rep_buf),
        "=== REPORTE DE TIEMPOS EJERCICIO 1 (LENGUAJE C) ===\n"
        "Compilador: GCC MinGW (optimizado -O2)\n"
        "Posicion inicial (Ak): %s\n"
        "Secuencia evaluada: 500 movimientos oficiales del PDF\n"
        "Iteraciones: %d\n"
        "Resultado: %s\n"
        "Tiempo total (10 iteraciones): %.5f ms (%.7f s)\n"
        "Tiempo promedio por iteracion: %.5f ms (%.7f s)\n\n",
        ak, iteraciones, msg, total_ms, total_ms / 1000.0, promedio_ms, promedio_ms / 1000.0);

    for (int i = 0; i < iteraciones && offset < (int)sizeof(rep_buf) - 64; i++) {
        offset += snprintf(rep_buf + offset, sizeof(rep_buf) - offset, "  Iteracion %2d: %.5f ms\n", i + 1, tiempos[i]);
    }

    char exe_path[MAX_PATH];
    char ruta_exe[MAX_PATH] = "";
    if (GetModuleFileNameA(NULL, exe_path, MAX_PATH) > 0) {
        char *s = strrchr(exe_path, '\\');
        if (s) {
            *s = '\0';
            snprintf(ruta_exe, sizeof(ruta_exe), "%s\\tiempos_ejercicio1_c.txt", exe_path);
        }
    }

    const char *destinos[] = {
        ruta_exe,
        "tiempos_ejercicio1_c.txt",
        "Ejercicio_1/tiempos_ejercicio1_c.txt",
        "JEBEL RUIZ/Ejercicio_1/tiempos_ejercicio1_c.txt"
    };

    int n_dest = sizeof(destinos) / sizeof(destinos[0]);
    for (int d = 0; d < n_dest; d++) {
        if (!destinos[d] || strlen(destinos[d]) == 0) continue;
        FILE *f = fopen(destinos[d], "wb");
        if (f) {
            fputs(rep_buf, f);
            fclose(f);
        }
    }

    printf("[OK] Reporte guardado exitosamente en 'tiempos_ejercicio1_c.txt'\n\n");

    if (sec_din) free(sec_din);
}

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "--benchmark") == 0) {
        ejecutar_benchmark();
        return 0;
    }
    if (argc > 1 && strcmp(argv[1], "--test") == 0) {
        ejecutar_casos_ejemplo();
        return 0;
    }

    ejecutar_casos_ejemplo();
    ejecutar_benchmark();
    return 0;
}
