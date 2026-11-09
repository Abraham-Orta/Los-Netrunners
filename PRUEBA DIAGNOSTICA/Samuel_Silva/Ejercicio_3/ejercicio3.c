#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

// Estructura para la tabla de palabras reservadas
typedef struct {
    const char *clave;
    const char *espanol;
} Palabra;

// Tabla con las 34 palabras reservadas de C
static const Palabra TABLA[] = {
    {"auto", "automatico"}, {"break", "romper"}, {"case", "caso"},
    {"char", "caracter"}, {"const", "constante"}, {"continue", "continuar"},
    {"default", "por defecto"}, {"do", "hacer"}, {"double", "doble"},
    {"else", "sino"}, {"enum", "enumeracion"}, {"extern", "externo"},
    {"float", "flotante"}, {"for", "para"}, {"goto", "ir a"},
    {"if", "si"}, {"inline", "en linea"}, {"int", "entero"},
    {"long", "largo"}, {"register", "registro"}, {"restrict", "restringir"},
    {"return", "retornar"}, {"short", "corto"}, {"signed", "con signo"},
    {"sizeof", "tamano de"}, {"static", "estatico"}, {"struct", "estructura"},
    {"switch", "selector"}, {"typedef", "definir tipo"}, {"union", "union"},
    {"unsigned", "sin signo"}, {"void", "vacio"}, {"volatile", "volatil"},
    {"while", "mientras"}, {NULL, NULL}
};

// Busca si una palabra es reservada y retorna su traduccion
const char* traducir(const char *tok) {
    for (int i = 0; TABLA[i].clave != NULL; i++) {
        if (strcmp(TABLA[i].clave, tok) == 0) {
            return TABLA[i].espanol;
        }
    }
    return NULL;
}

// Carga el archivo completo usando memoria dinamica (malloc)
char* leer_archivo(const char *ruta) {
    FILE *f = fopen(ruta, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long tam = ftell(f);
    rewind(f);

    char *buf = (char*)malloc(tam + 1);
    if (!buf) {
        fclose(f);
        return NULL;
    }

    fread(buf, 1, tam, f);
    buf[tam] = '\0';
    fclose(f);
    return buf;
}

// Procesa el texto aplicando el analisis lexico
void procesar_codigo(const char *src) {
    const char *p = src;
    int num_linea = 1;

    printf("Linea %3d: ", num_linea);

    while (*p) {
        // Comentario de una sola linea: //
        if (p[0] == '/' && p[1] == '/') {
            while (*p && *p != '\n') {
                putchar(*p++);
            }
            continue;
        }

        // Comentario de bloque: /* ... */
        if (p[0] == '/' && p[1] == '*') {
            putchar(*p++);
            putchar(*p++);
            while (*p && !(p[0] == '*' && p[1] == '/')) {
                if (*p == '\n') {
                    putchar('\n');
                    num_linea++;
                    printf("Linea %3d: ", num_linea);
                } else {
                    putchar(*p);
                }
                p++;
            }
            if (*p) { putchar(*p++); putchar(*p++); }
            continue;
        }

        // Literales de texto (cadenas con comillas dobles)
        if (*p == '"') {
            putchar(*p++);
            while (*p && *p != '"') {
                if (*p == '\\') putchar(*p++);
                putchar(*p++);
            }
            if (*p) putchar(*p++);
            continue;
        }

        // Literales de caracteres (comillas simples)
        if (*p == '\'') {
            putchar(*p++);
            while (*p && *p != '\'') {
                if (*p == '\\') putchar(*p++);
                putchar(*p++);
            }
            if (*p) putchar(*p++);
            continue;
        }

        // Salto de linea
        if (*p == '\n') {
            putchar('\n');
            num_linea++;
            printf("Linea %3d: ", num_linea);
            p++;
            continue;
        }

        // Identificadores o palabras reservadas
        if (isalpha((unsigned char)*p) || *p == '_') {
            char palabra[64];
            int i = 0;
            while ((isalnum((unsigned char)*p) || *p == '_') && i < 63) {
                palabra[i++] = *p++;
            }
            palabra[i] = '\0';

            const char *traduccion = traducir(palabra);
            if (traduccion) {
                printf("[%s]", traduccion);
            } else {
                printf("%s", palabra);
            }
            continue;
        }

        // Cualquier otro simbolo (operadores, llaves, espacios, etc.)
        putchar(*p++);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    char ruta[MAX_PATH];

    if (argc >= 2) {
        strncpy(ruta, argv[1], sizeof(ruta) - 1);
        ruta[sizeof(ruta) - 1] = '\0';
    } else {
        // Primero intentamos en el directorio actual
        FILE *f_test = fopen("programa_prueba.c", "r");
        if (f_test) {
            fclose(f_test);
            strcpy(ruta, "programa_prueba.c");
        } else {
            // Si no esta en el dir actual, buscamos junto al ejecutable
            GetModuleFileNameA(NULL, ruta, MAX_PATH);
            char *barra = strrchr(ruta, '\\');
            if (barra) {
                strcpy(barra + 1, "programa_prueba.c");
            } else {
                strcpy(ruta, "programa_prueba.c");
            }
        }
        printf("Usando '%s' por defecto...\n\n", ruta);
    }

    char *codigo = leer_archivo(ruta);
    if (!codigo) {
        printf("Error: No se pudo abrir o leer el archivo '%s'.\n", ruta);
        printf("Asegurese de que el archivo exista.\n");
        system("pause");
        return 1;
    }

    printf("TRADUCCION DE PALABRAS RESERVADAS EN C: %s\n\n", ruta);
    procesar_codigo(codigo);

    free(codigo);

    printf("\n");
    system("pause");
    return 0;
}
