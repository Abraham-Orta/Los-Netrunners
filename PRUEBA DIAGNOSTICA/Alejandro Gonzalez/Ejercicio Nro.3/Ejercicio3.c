//Ejercicio Nro.3: Traductor de palabras reservadas en C a español
//Estudiante: Alejandro Gonzalez
//Lenguaje: C



//+++++++++Librerias++++++++++++
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


//+++++++++Estructuras++++++++++++

// Estructura de equivalencias entre palabras reservadas
typedef struct {
    const char *ingles;
    const char *espanol;
} Traduccion;

//+++++++++Variables Globales++++++++++++
Traduccion palabras[] = {
    {"auto", "automatico"},
    {"break", "romper"},
    {"case", "caso"},
    {"char", "caracter"},
    {"const", "constante"},
    {"continue", "continuar"},
    {"default", "predeterminado"},
    {"do", "hacer"},
    {"double", "doble"},
    {"else", "sino"},
    {"enum", "enumeracion"},
    {"extern", "externo"},
    {"float", "flotante"},
    {"for", "para"},
    {"goto", "ir_a"},
    {"if", "si"},
    {"inline", "en_linea"},
    {"int", "entero"},
    {"long", "largo"},
    {"register", "registro"},
    {"restrict", "restringido"},
    {"return", "retornar"},
    {"short", "corto"},
    {"signed", "con_signo"},
    {"sizeof", "tamano"},
    {"static", "estatico"},
    {"struct", "estructura"},
    {"switch", "seleccionar"},
    {"typedef", "definir_tipo"},
    {"union", "union"},
    {"unsigned", "sin_signo"},
    {"void", "vacio"},
    {"volatile", "volatil"},
    {"while", "mientras"},
    {"_Alignas", "alinear"},
    {"_Alignof", "alineacion"},
    {"_Atomic", "atomico"},
    {"_Bool", "booleano"},
    {"_Complex", "complejo"},
    {"_Generic", "generico"},
    {"_Imaginary", "imaginario"},
    {"_Noreturn", "sin_retorno"},
    {"_Static_assert", "afirmacion_estatica"},
    {"_Thread_local", "local_al_hilo"}
};

//+++++++++Funciones++++++++++++

#define TOTAL_PALABRAS (sizeof(palabras) / sizeof(palabras[0]))

// Busca una palabra reservada y devuelve su traduccion.
const char *traducirPalabra(const char *palabra, size_t longitud) {

    for (size_t i = 0; i < TOTAL_PALABRAS; i++) {

        if (strlen(palabras[i].ingles) == longitud &&
            strncmp(palabra, palabras[i].ingles, longitud) == 0) {

            return palabras[i].espanol;
        }
    }

    return NULL;
}

// Carga el archivo completo en memoria dinamica.
char *cargarArchivo(const char *nombreArchivo) {

    FILE *archivo = fopen(nombreArchivo, "r");

    if (archivo == NULL) {
        printf("Error: no se pudo abrir el archivo.\n");
        return NULL;
    }

    size_t capacidad = 100;
    size_t longitud = 0;

    char *contenido = malloc(capacidad);

    if (contenido == NULL) {
        printf("Error al reservar memoria.\n");
        fclose(archivo);
        return NULL;
    }

    int caracter;

    while ((caracter = fgetc(archivo)) != EOF) {

        if (longitud + 1 >= capacidad) {

            capacidad *= 2;

            char *temporal = realloc(contenido, capacidad);

            if (temporal == NULL) {
                printf("Error al ampliar la memoria.\n");
                free(contenido);
                fclose(archivo);
                return NULL;
            }

            contenido = temporal;
        }

        contenido[longitud++] = (char)caracter;
    }

    contenido[longitud] = '\0';

    fclose(archivo);

    return contenido;
}

// Traduce las palabras reservadas del codigo fuente.
void traducirCodigo(const char *codigo) {

    size_t i = 0;

    while (codigo[i] != '\0') {

        // Evitar traducir comentarios de una linea.
        if (codigo[i] == '/' && codigo[i + 1] == '/') {

            while (codigo[i] != '\0') {
                putchar(codigo[i]);

                if (codigo[i++] == '\n') {
                    break;
                }
            }

        }

        // Evitar traducir comentarios de varias lineas.
        else if (codigo[i] == '/' && codigo[i + 1] == '*') {

            while (codigo[i] != '\0') {

                putchar(codigo[i]);

                if (codigo[i] == '*' && codigo[i + 1] == '/') {
                    i++;
                    putchar(codigo[i]);
                    i++;
                    break;
                }

                i++;
            }

        }

        // Evitar traducir el contenido de cadenas de texto.
        else if (codigo[i] == '"' || codigo[i] == '\'') {

            char comilla = codigo[i];

            do {
                char actual = codigo[i];
                putchar(actual);
                i++;

                if (actual == '\\' && codigo[i] != '\0') {
                    putchar(codigo[i]);
                    i++;
                }
                else if (actual == comilla) {
                    break;
                }

            } while (codigo[i] != '\0');

        }

        // Identificar palabras e identificadores.
        else if (isalpha((unsigned char)codigo[i]) || codigo[i] == '_') {

            size_t inicio = i;

            while (isalnum((unsigned char)codigo[i]) ||
                   codigo[i] == '_') {
                i++;
            }

            size_t longitud = i - inicio;

            const char *traduccion =
                traducirPalabra(&codigo[inicio], longitud);

            if (traduccion != NULL) {
                printf("%s", traduccion);
            } else {
                printf("%.*s", (int)longitud, &codigo[inicio]);
            }

        }

        // Copiar espacios, numeros y simbolos.
        else {
            putchar(codigo[i]);
            i++;
        }
    }
}

int main() {

    char *codigo = cargarArchivo("Prueba.c");

    if (codigo == NULL) {
        return 1;
    }

    printf("===== CODIGO TRADUCIDO =====\n\n");

    traducirCodigo(codigo);

    printf("\n\n===== FIN DE LA TRADUCCION =====\n");

    free(codigo);

    return 0;
}

