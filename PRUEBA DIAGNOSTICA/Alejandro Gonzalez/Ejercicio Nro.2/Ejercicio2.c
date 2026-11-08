//Ejercicio Nro.2: Polinomios  
//Estudiante: Alejandro Gonzalez
//Lenguaje: C

//++++++++++++++Librerias++++++++++++++++++++
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <math.h>


//++++++++++++++Funciones++++++++++++++++++++

//Creacion de los Coeficientes utilizando el Triángulo de Pascal
__uint128_t* generarCoeficientes(int n) {

    // Reservar memoria dinámica
    __uint128_t *coeficientes;
    coeficientes = malloc((n + 1) * sizeof(__uint128_t));
    
    if (coeficientes == NULL) {
        printf("Error al reservar espacio de memoria\n");
        return NULL;
    }

    // Inicializar el array, y actualizar los coeficientes
    for (int i = 0; i <= n; i++) {
        coeficientes[i] = 1; 
        
        for (int j = i - 1; j > 0; j--) {
            coeficientes[j] += coeficientes[j - 1];
        }
    }

    return coeficientes;
}

//Usamos la variable __uint128_t para almacenar los coeficientes del polinomio.
//Para mostrar los coeficientes, necesitamos una función dedicada  
void mostrarNumero(__uint128_t numero) {

    if (numero >= 10) {
        mostrarNumero(numero / 10);
    }
    putchar('0' + numero % 10);
}

//Recoremos el array y mostramos el polinomio 
void mostrarPolinomio(__uint128_t *coeficientes, int n) {

    printf("\n+ Polinomio generado:\n");

    for (int i = 0; i <= n; i++) {

        int exponente = n - i;

        if (i > 0) {
            printf(" + ");
        }

        if (exponente == 0 || coeficientes[i] != 1) {
            mostrarNumero(coeficientes[i]);
        }

        if (exponente > 0) {
            printf("x");

            if (exponente > 1) {
                printf("^%d", exponente);
            }
        }
    }

    printf("\n");
}


//Función para evaluar el polinomio paso a paso
void evaluarPolinomio(__uint128_t *coeficientes, int n, double x) {

    double suma = 0;

    printf("\n+ Evaluacion paso a paso:\n");

    for (int i = 0; i <= n; i++) {

        int exponente = n - i;

        double potencia = pow(x, exponente);

        double termino = (double)coeficientes[i] * potencia;

        printf("- Termino %d: ", i + 1);

        mostrarNumero(coeficientes[i]);

        printf(" * %.2lf^%d = %.2lf\n", x, exponente, termino);

        suma += termino;
    }

    printf("\n+ Resultado final: %.2lf\n", suma);
}

//Función para medir el tiempo de ejecución de la generación de coeficientes
double medirTiempoGeneracion(int n, int repeticiones) {

    clock_t inicio = clock();

    volatile __uint128_t verificacion = 0;

    for (int i = 0; i < repeticiones; i++) {

        __uint128_t *coeficientes = generarCoeficientes(n);

        if (coeficientes == NULL) {
            return -1;
        }

        verificacion += coeficientes[n / 2];

        free(coeficientes);
    }

    clock_t fin = clock();

    double tiempoTotal = (double)(fin - inicio) / CLOCKS_PER_SEC;

    return tiempoTotal / repeticiones;
}


//++++++++++++++Funcion Principal++++++++++++++++++++
int main() {

    int n;
    double x;
    printf("\n\t\tCalculo del Polinomio (x+1)^n (C)\n\n");

    //Recibimos el valor de n y x, validando que n sea no negativo
    
    do{
        printf("+ Ingrese el valor de N (Valor del exponente): ");
        scanf("%d", &n);
        if (n < 0) {
            printf("- Error: N debe ser no negativo.\n+ Intente nuevamente.\n");
        }
    }while (n < 0);

    do{
        printf("+ Ingrese el valor de X (Valor de la variable): ");
        if (scanf("%lf", &x) != 1) {
            printf("- Error al leer el valor de X.\n+ Intente nuevamente.\n");
        }
    }while (x < 0);

    // Generamos los coeficientes del polinomio (x+1)^n
    //El tipo de variable __uint128_t nos permite manejar coeficientes muy grandes

    __uint128_t *coeficientes = generarCoeficientes(n);

    if (coeficientes == NULL) {
        return 1;
    }

    // Mostramos el polinomio generado 
    mostrarPolinomio(coeficientes, n);

    evaluarPolinomio(coeficientes, n, x);

    printf("\n--- Medicion de rendimiento ---\n");

    int repeticiones = 1000;

    double tiempoPromedio = medirTiempoGeneracion(n, repeticiones);

    if (tiempoPromedio < 0) {
        printf("Error durante la medicion.\n");
    } else {
        printf("Valor de n: %d\n", n);
        printf("Repeticiones: %d\n", repeticiones);
        printf("Tiempo promedio: %.10f segundos\n", tiempoPromedio);
    }

    free(coeficientes);

    return 0;
}
