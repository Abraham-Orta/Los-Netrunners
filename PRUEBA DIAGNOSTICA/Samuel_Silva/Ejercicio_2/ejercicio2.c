#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <math.h>

// Genera los coeficientes del binomio usando memoria dinamica (malloc)
double* calcular_pascal(int n) {
    double *coef = (double*)malloc((n + 1) * sizeof(double));
    if (!coef) {
        printf("Error de asignacion de memoria.\n");
        exit(1);
    }

    // Llenamos el triangulo de Pascal
    for (int i = 0; i <= n; i++) {
        coef[i] = 1.0;
        // Se actualiza de derecha a izquierda para no sobreescribir valores previos
        for (int j = i - 1; j > 0; j--) {
            coef[j] = coef[j] + coef[j - 1];
        }
    }
    return coef;
}

// Imprime el polinomio con formato limpio (omitiendo coeficiente 1 en x^k)
void mostrar_polinomio(int n, double *c) {
    for (int i = 0; i <= n; i++) {
        int pot = n - i;
        double valor = c[i];

        if (pot > 1) {
            if (valor == 1.0) printf("x^%d", pot);
            else              printf("%.0fx^%d", valor, pot);
        } else if (pot == 1) {
            if (valor == 1.0) printf("x");
            else              printf("%.0fx", valor);
        } else {
            printf("%.0f", valor);
        }

        if (i < n) printf(" + ");
    }
    printf("\n");
}

// Evalua el polinomio paso a paso con el valor de x ingresado
void evaluar_paso_a_paso(int n, double *c, int x) {
    printf("\nb) Calculando f(%d) = (%d + 1)^%d paso a paso:\n", x, x, n);
    double total = 0.0;

    for (int i = 0; i <= n; i++) {
        int pot = n - i;
        double base = pow((double)x, pot);
        double term = c[i] * base;
        total += term;

        double c_val = c[i];

        if (pot > 1) {
            if (c_val == 1.0) {
                printf("   Paso %d: Termino x^%d -> (%d)^%d = %.0f\n", i + 1, pot, x, pot, term);
            } else {
                printf("   Paso %d: Termino %.0fx^%d -> %.0f * (%d)^%d = %.0f\n", i + 1, c_val, pot, c_val, x, pot, term);
            }
        } else if (pot == 1) {
            if (c_val == 1.0) {
                printf("   Paso %d: Termino x -> (%d) = %.0f\n", i + 1, x, term);
            } else {
                printf("   Paso %d: Termino %.0fx -> %.0f * (%d) = %.0f\n", i + 1, c_val, c_val, x, term);
            }
        } else {
            printf("   Paso %d: Termino independiente -> %.0f\n", i + 1, term);
        }
    }

    printf("   Resultado final f(%d) = %.0f\n", x, total);
}

// Guarda la medicion en tiempos.txt preservando la de Python si existe
void guardar_tiempo_c(double tiempo_seg) {
    char ruta[MAX_PATH];
    GetModuleFileNameA(NULL, ruta, MAX_PATH);
    char *barra = strrchr(ruta, '\\');
    if (barra) {
        strcpy(barra + 1, "tiempos.txt");
    } else {
        strcpy(ruta, "tiempos.txt");
    }

    char linea_py[256] = "";
    FILE *f_lectura = fopen(ruta, "r");
    if (f_lectura) {
        char buf[256];
        while (fgets(buf, sizeof(buf), f_lectura)) {
            if (strstr(buf, "Python:")) {
                strcpy(linea_py, buf);
            }
        }
        fclose(f_lectura);
    }

    FILE *f_escritura = fopen(ruta, "w");
    if (f_escritura) {
        fprintf(f_escritura, "Tiempo de ejecucion para generar n=100 en C: %.8f segundos\n", tiempo_seg);
        if (linea_py[0] != '\0') {
            fprintf(f_escritura, "%s", linea_py);
            if (linea_py[strlen(linea_py) - 1] != '\n') {
                fprintf(f_escritura, "\n");
            }
        }
        fclose(f_escritura);
        printf("\n[OK] Tiempo de ejecucion para n=100 guardado en 'tiempos.txt'.\n");
    } else {
        printf("\nError al escribir tiempos.txt\n");
    }
}

int main(void) {
    int n, x;

    printf("Expansion de (x + 1)^n con Triangulo de Pascal \n");
    printf("Ingrese n (entero no negativo): ");
    if (scanf("%d", &n) != 1 || n < 0) {
        printf("Valor de n invalido.\n");
        system("pause");
        return 1;
    }

    printf("Ingrese x (entero): ");
    if (scanf("%d", &x) != 1) {
        printf("Valor de x invalido.\n");
        system("pause");
        return 1;
    }

    // 1. Demostracion para el n ingresado
    double *coef = calcular_pascal(n);

    printf("\nDEMOSTRACION PARA n=%d\n", n);
    printf("a) (x + 1)^%d = ", n);
    mostrar_polinomio(n, coef);
    evaluar_paso_a_paso(n, coef, x);

    // Liberamos memoria dinamica solicitada
    free(coef);

    // 2. Medicion de tiempo para n=100 con QueryPerformanceCounter
    LARGE_INTEGER freq, t0, t1;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&t0);

    double *coef100 = calcular_pascal(100);

    QueryPerformanceCounter(&t1);
    double tiempo = (double)(t1.QuadPart - t0.QuadPart) / freq.QuadPart;
    free(coef100);

    guardar_tiempo_c(tiempo);

    printf("\n");
    system("pause");
    return 0;
}
