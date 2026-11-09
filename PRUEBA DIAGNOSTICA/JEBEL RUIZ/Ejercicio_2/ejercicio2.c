//  Prueba Diagnóstica - Ejercicio 2 (Lenguaje C)


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>

// Estructura para enteros de precisión arbitraria en memoria dinámica 
typedef struct {
    char *digits; // Dígitos en orden inverso (base 10): digits[0] es unidades
    int len;
} BigInt;

// Crea un BigInt a partir de un entero estándar
BigInt bigint_from_int(long long val) {
    BigInt b;
    char buf[64];
    snprintf(buf, sizeof(buf), "%lld", val);
    b.len = (int)strlen(buf);
    b.digits = (char *)malloc(b.len + 1);
    for (int i = 0; i < b.len; i++) {
        b.digits[i] = buf[b.len - 1 - i] - '0';
    }
    b.digits[b.len] = '\0';
    return b;
}

// Copia un BigInt 
BigInt bigint_copy(BigInt src) {
    BigInt b;
    b.len = src.len;
    b.digits = (char *)malloc(b.len + 1);
    memcpy(b.digits, src.digits, b.len + 1);
    return b;
}

// Suma dos BigInt: res = a + b en memoria dinámica 
BigInt bigint_add(BigInt a, BigInt b) {
    BigInt res;
    int max_l = (a.len > b.len) ? a.len : b.len;
    res.digits = (char *)malloc(max_l + 3);

    int carry = 0;
    int k = 0;
    for (int i = 0; i < max_l || carry; i++) {
        int sum = carry;
        if (i < a.len) sum += a.digits[i];
        if (i < b.len) sum += b.digits[i];
        res.digits[k++] = sum % 10;
        carry = sum / 10;
    }
    res.len = k;
    res.digits[k] = '\0';
    return res;
}

// Libera la memoria de un BigInt 
void bigint_free(BigInt *b) {
    if (b && b->digits) {
        free(b->digits);
        b->digits = NULL;
        b->len = 0;
    }
}

// Convierte un BigInt a cadena legible (orden normal) 
char *bigint_to_str(BigInt b) {
    char *str = (char *)malloc(b.len + 1);
    for (int i = 0; i < b.len; i++) {
        str[i] = b.digits[b.len - 1 - i] + '0';
    }
    str[b.len] = '\0';
    return str;
}

/* Convierte BigInt a double (para cálculos de evaluación) */
double bigint_to_double(BigInt b) {
    double val = 0.0;
    for (int i = b.len - 1; i >= 0; i--) {
        val = val * 10.0 + b.digits[i];
    }
    return val;
}

/*
 * Genera el Triángulo de Pascal hasta la fila n usando memoria dinámica.
 * triangulo[i] contiene una fila de i + 1 BigInts.
 */
BigInt **generar_triangulo_pascal(int n) {
    BigInt **triangulo = (BigInt **)malloc((n + 1) * sizeof(BigInt *));
    if (!triangulo) return NULL;

    for (int i = 0; i <= n; i++) {
        triangulo[i] = (BigInt *)malloc((i + 1) * sizeof(BigInt));
        for (int j = 0; j <= i; j++) {
            if (j == 0 || j == i) {
                triangulo[i][j] = bigint_from_int(1);
            } else {
                triangulo[i][j] = bigint_add(triangulo[i - 1][j - 1], triangulo[i - 1][j]);
            }
        }
    }
    return triangulo;
}

/* Libera toda la memoria del Triángulo de Pascal */
void liberar_triangulo_pascal(BigInt **triangulo, int n) {
    if (!triangulo) return;
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= i; j++) {
            bigint_free(&triangulo[i][j]);
        }
        free(triangulo[i]);
    }
    free(triangulo);
}

/* Muestra el polinomio expandido */
void mostrar_polinomio(BigInt *coefs, int n) {
    printf("(x+1)^%d = ", n);
    if (n == 0) {
        printf("1\n");
        return;
    }

    for (int i = 0; i <= n; i++) {
        int grado = n - i;
        char *str_c = bigint_to_str(coefs[i]);
        int es_uno = (strcmp(str_c, "1") == 0);

        if (grado == 0) {
            printf("%s", str_c);
        } else if (grado == 1) {
            if (es_uno) printf("x + ");
            else printf("%sx + ", str_c);
        } else {
            if (es_uno) printf("x^%d + ", grado);
            else printf("%sx^%d + ", str_c, grado);
        }
        free(str_c);
    }
    printf("\n");
}

/* Muestra por pasos el cálculo de f(x) = (x+1)^n */
void evaluar_por_pasos(BigInt *coefs, int n, double x) {
    printf("\n======================================================================\n");
    printf("     CALCULO PASO A PASO: f(%.2f) = (%.2f + 1)^%d\n", x, x, n);
    printf("======================================================================\n");
    mostrar_polinomio(coefs, n);
    printf("\n");

    double suma_acumulada = 0.0;
    for (int i = 0; i <= n; i++) {
        int grado = n - i;
        double c_val = bigint_to_double(coefs[i]);
        double potencia = pow(x, grado);
        double termino = c_val * potencia;
        suma_acumulada += termino;

        char *str_c = bigint_to_str(coefs[i]);
        printf("Paso %2d (grado %2d):\n", i + 1, grado);
        if (grado > 1) {
            printf("  Termino: %s * (%.2f)^%d = %.4f * %.4f = %.4f\n", str_c, x, grado, c_val, potencia, termino);
        } else if (grado == 1) {
            printf("  Termino: %s * (%.2f) = %.4f\n", str_c, x, termino);
        } else {
            printf("  Termino independiente: %s = %.4f\n", str_c, termino);
        }
        printf("  Suma acumulada: %.4f\n\n", suma_acumulada);
        free(str_c);
    }

    double valor_directo = pow(x + 1.0, n);
    printf("----------------------------------------------------------------------\n");
    printf("Resultado segun polinomio generado:  P(%.2f) = %.6f\n", x, suma_acumulada);
    printf("Comprobacion directa (x + 1)^%d:     f(%.2f) = %.6f\n", n, x, valor_directo);
    printf("Diferencia / Error absoluto:        %.6e\n", fabs(suma_acumulada - valor_directo));
    printf("----------------------------------------------------------------------\n");
}

/* Benchmark de generación de n=100 */
void benchmark_n100(int repeticiones) {
    printf("======================================================================\n");
    printf("        BENCHMARK EJERCICIO 2 (C): GENERACION DE n=100                \n");
    printf("======================================================================\n");

    LARGE_INTEGER freq, t_start, t_end;
    QueryPerformanceFrequency(&freq);

    int n = 100;
    double total_ms = 0.0;
    double min_ms = 1e9;

    BigInt **triangulo_final = NULL;

    for (int r = 0; r < repeticiones; r++) {
        QueryPerformanceCounter(&t_start);
        BigInt **tri = generar_triangulo_pascal(n);
        QueryPerformanceCounter(&t_end);

        double ms = (double)(t_end.QuadPart - t_start.QuadPart) * 1000.0 / (double)freq.QuadPart;
        total_ms += ms;
        if (ms < min_ms) min_ms = ms;

        if (r == repeticiones - 1) {
            triangulo_final = tri;
        } else {
            liberar_triangulo_pascal(tri, n);
        }
    }

    double prom_ms = total_ms / repeticiones;

    char *coef_central = bigint_to_str(triangulo_final[100][50]);
    printf("Grado del polinomio: n = %d\n", n);
    printf("Filas del Triangulo de Pascal: 101 (5151 coeficientes generados)\n");
    printf("Repeticiones evaluadas: %d\n", repeticiones);
    printf("Coeficiente central C(100, 50):\n  %s\n", coef_central);
    printf("Numero de digitos del coeficiente central: %d\n", (int)strlen(coef_central));
    printf("Tiempo total (%d ejecuciones): %.5f ms (%.7f s)\n", repeticiones, total_ms, total_ms / 1000.0);
    printf("Tiempo promedio por generacion: %.5f ms (%.7f s)\n", prom_ms, prom_ms / 1000.0);
    printf("Tiempo minimo (mejor ejecucion): %.5f ms (%.7f s)\n", min_ms, min_ms / 1000.0);

    FILE *fout = fopen("tiempos_ejercicio2_c.txt", "w");
    if (fout) {
        fprintf(fout, "=== REPORTE DE TIEMPOS EJERCICIO 2 (LENGUAJE C) ===\n");
        fprintf(fout, "Compilador: GCC MinGW (Optimizacion -O2)\n");
        fprintf(fout, "Grado del polinomio: n = %d (Triangulo de Pascal con memoria dinamica y BigInt)\n", n);
        fprintf(fout, "Filas: 101, Coeficientes generados: 5151\n");
        fprintf(fout, "Repeticiones del benchmark: %d\n", repeticiones);
        fprintf(fout, "Coeficiente central C(100, 50):\n  %s\n", coef_central);
        fprintf(fout, "Numero de digitos: %d\n", (int)strlen(coef_central));
        fprintf(fout, "Tiempo total (%d ejecuciones): %.5f ms (%.7f s)\n", repeticiones, total_ms, total_ms / 1000.0);
        fprintf(fout, "Tiempo promedio por generacion: %.5f ms (%.7f s)\n", prom_ms, prom_ms / 1000.0);
        fprintf(fout, "Tiempo minimo (mejor ejecucion): %.5f ms (%.7f s)\n", min_ms, min_ms / 1000.0);
        fclose(fout);
        printf("[OK] Reporte guardado exitosamente en 'tiempos_ejercicio2_c.txt'\n\n");
    }

    free(coef_central);
    liberar_triangulo_pascal(triangulo_final, n);
}

void mostrar_demostracion() {
    printf("======================================================================\n");
    printf("      EJERCICIO 2 (C): GENERACION TRIANGULO DE PASCAL (n=0..7)        \n");
    printf("======================================================================\n");
    BigInt **p = generar_triangulo_pascal(7);
    for (int i = 0; i <= 7; i++) {
        mostrar_polinomio(p[i], i);
    }

    evaluar_por_pasos(p[3], 3, 2.0);
    liberar_triangulo_pascal(p, 7);
}

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "--benchmark") == 0) {
        benchmark_n100(100);
        return 0;
    }
    if (argc > 1 && strcmp(argv[1], "--demo") == 0) {
        mostrar_demostracion();
        return 0;
    }

    mostrar_demostracion();
    benchmark_n100(100);
    return 0;
}
