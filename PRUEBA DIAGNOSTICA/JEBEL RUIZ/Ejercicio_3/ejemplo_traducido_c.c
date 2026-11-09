/*
 * CODIGO TRADUCIDO AL ESPANOL (Ejercicio 3 - UNEG)
 * Se incluye 'c_espanol.h' para que el compilador y el editor reconozcan
 * las palabras traducidas y el programa sea 100% valido y ejecutable.
 */
#include "c_espanol.h"

//Archivo de prueba en lenguaje C para el Ejercicio 3


#include <stdio.h>
#include <stdlib.h>

// Funcion para calcular la suma de los primeros n numeros
entero calcular_suma(entero n) {
    entero acumulador = 0; // variable entera
    para (entero i = 1; i <= n; i++) {
        acumulador += i;
    }
    retornar acumulador;
}

// Funcion principal
vacio ejecutar_proceso() {
    flotante radio = 5.5;
    doble area = 3.14159 * radio * radio;

    si (area > 50.0) {
        printf("Area mayor a 50: %f\n", area);
    } sino {
        printf("Area menor o igual a 50\n");
    }

    entero contador = 3;
    mientras (contador > 0) {
        // La palabra 'while' o 'if' dentro de esta cadena NO debe traducirse:
        printf("Mensaje con texto: while y if dentro de string literal\n");
        contador--;
    }
}

entero main() {
    entero n = 10;
    entero resultado = calcular_suma(n);
    printf("Resultado: %d\n", resultado);
    ejecutar_proceso();
    retornar 0;
}
