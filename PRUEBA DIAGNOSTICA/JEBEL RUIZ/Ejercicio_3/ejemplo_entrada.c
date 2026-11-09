//Archivo de prueba en lenguaje C para el Ejercicio 3


#include <stdio.h>
#include <stdlib.h>

// Funcion para calcular la suma de los primeros n numeros
int calcular_suma(int n) {
    int acumulador = 0; // variable entera
    for (int i = 1; i <= n; i++) {
        acumulador += i;
    }
    return acumulador;
}

// Funcion principal
void ejecutar_proceso() {
    float radio = 5.5;
    double area = 3.14159 * radio * radio;

    if (area > 50.0) {
        printf("Area mayor a 50: %f\n", area);
    } else {
        printf("Area menor o igual a 50\n");
    }

    int contador = 3;
    while (contador > 0) {
        // La palabra 'while' o 'if' dentro de esta cadena NO debe traducirse:
        printf("Mensaje con texto: while y if dentro de string literal\n");
        contador--;
    }
}

int main() {
    int n = 10;
    int resultado = calcular_suma(n);
    printf("Resultado: %d\n", resultado);
    ejecutar_proceso();
    return 0;
}
