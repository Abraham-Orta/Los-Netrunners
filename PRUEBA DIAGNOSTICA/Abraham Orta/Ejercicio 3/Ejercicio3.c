/*
 * Ejercicio 3 - Detector y Traductor de Palabras Reservadas de C
 *
 * Carga un archivo fuente en C en memoria dinámica, lo analiza en busca
 * de palabras reservadas del lenguaje C y:
 *   1. Lista cada keyword encontrada con su traducción al español.
 *   2. Muestra el archivo completo con las keywords reemplazadas por
 *      su equivalente en español.
 *
 * Uso:
 *   ./Ejercicio3 <archivo_fuente.c>
 *
 * El archivo se lee completo en un buffer dinámico (malloc) y se
 * tokeniza en el mismo lugar. La memoria se libera antes de terminar.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ------------------------------------------------------------------ */
/*  Tabla de palabras reservadas de C                                  */
/* ------------------------------------------------------------------ */

typedef struct {
    const char *palabra_clave;
    const char *traduccion_espanol;
} EntradaPalabraClave;

static const EntradaPalabraClave PALABRAS_CLAVE[] = {
    {"auto",           "automatico"},
    {"break",          "romper"},
    {"case",           "caso"},
    {"char",           "caracter"},
    {"const",          "constante"},
    {"continue",       "continuar"},
    {"default",        "por_defecto"},
    {"do",             "hacer"},
    {"double",         "doble_precision"},
    {"else",           "sino"},
    {"enum",           "enumeracion"},
    {"extern",         "externo"},
    {"float",          "flotante"},
    {"for",            "para"},
    {"goto",           "ir_a"},
    {"if",             "si"},
    {"inline",         "en_linea"},
    {"int",            "entero"},
    {"long",           "largo"},
    {"register",       "registro"},
    {"restrict",       "restringir"},
    {"return",         "retornar"},
    {"short",          "corto"},
    {"signed",         "con_signo"},
    {"sizeof",         "tamano_de"},
    {"static",         "estatico"},
    {"struct",         "estructura"},
    {"switch",         "segun"},
    {"typedef",        "definir_tipo"},
    {"union",          "union"},
    {"unsigned",       "sin_signo"},
    {"void",           "vacio"},
    {"volatile",       "volatil"},
    {"while",          "mientras"},
    {"_Bool",          "booleano"},
    {"_Complex",       "complejo"},
    {"_Imaginary",     "imaginario"},
};

#define TOTAL_PALABRAS_CLAVE (int)(sizeof(PALABRAS_CLAVE) / sizeof(PALABRAS_CLAVE[0]))

/* ------------------------------------------------------------------ */
/*  Funciones auxiliares                                                */
/* ------------------------------------------------------------------ */

/*
 * buscar_palabra_clave - busca la palabra en la tabla de palabras reservadas.
 * Retorna la entrada encontrada o NULL si no existe.
 */
static const EntradaPalabraClave *buscar_palabra_clave(const char *palabra)
{
    for (int i = 0; i < TOTAL_PALABRAS_CLAVE; i++) {
        if (strcmp(PALABRAS_CLAVE[i].palabra_clave, palabra) == 0) {
            return &PALABRAS_CLAVE[i];
        }
    }
    return NULL;
}

/*
 * es_caracter_identificador - retorna distinto de cero si el carácter
 * puede formar parte de un identificador o palabra clave.
 */
static int es_caracter_identificador(char caracter)
{
    return isalnum((unsigned char)caracter) || caracter == '_';
}

/*
 * cargar_archivo - lee el archivo completo en un buffer recién reservado.
 * Escribe el tamaño en *tam_salida.
 * Retorna el buffer (el llamador debe liberarlo) o NULL si hay error.
 */
static char *cargar_archivo(const char *ruta, size_t *tam_salida)
{
    FILE *archivo = fopen(ruta, "rb");
    if (!archivo) { perror("fopen"); return NULL; }

    if (fseek(archivo, 0, SEEK_END) != 0) {
        perror("fseek"); fclose(archivo); return NULL;
    }
    long tamano = ftell(archivo);
    if (tamano < 0) { perror("ftell"); fclose(archivo); return NULL; }
    rewind(archivo);

    /* Reservar memoria dinámica: contenido + terminador nulo */
    char *contenido = malloc((size_t)tamano + 1);
    if (!contenido) { perror("malloc"); fclose(archivo); return NULL; }

    size_t bytes_leidos = fread(contenido, 1, (size_t)tamano, archivo);
    fclose(archivo);

    contenido[bytes_leidos] = '\0';
    *tam_salida = bytes_leidos;
    return contenido;
}

/* ------------------------------------------------------------------ */
/*  Conjunto para evitar duplicados en el resumen                      */
/* ------------------------------------------------------------------ */

typedef struct {
    const char **elementos;
    int          cantidad;
    int          capacidad;
} ConjuntoCadenas;

static ConjuntoCadenas *conjunto_nuevo(void)
{
    ConjuntoCadenas *conjunto = malloc(sizeof(ConjuntoCadenas));
    if (!conjunto) return NULL;
    conjunto->capacidad = 32;
    conjunto->cantidad  = 0;
    conjunto->elementos = malloc(conjunto->capacidad * sizeof(char *));
    if (!conjunto->elementos) { free(conjunto); return NULL; }
    return conjunto;
}

static int conjunto_contiene(const ConjuntoCadenas *conjunto, const char *elemento)
{
    for (int i = 0; i < conjunto->cantidad; i++) {
        if (strcmp(conjunto->elementos[i], elemento) == 0) return 1;
    }
    return 0;
}

static void conjunto_agregar(ConjuntoCadenas *conjunto, const char *elemento)
{
    if (conjunto_contiene(conjunto, elemento)) return;
    if (conjunto->cantidad == conjunto->capacidad) {
        conjunto->capacidad *= 2;
        conjunto->elementos = realloc(conjunto->elementos,
                                      conjunto->capacidad * sizeof(char *));
    }
    conjunto->elementos[conjunto->cantidad++] = elemento;
}

static void conjunto_liberar(ConjuntoCadenas *conjunto)
{
    if (conjunto) { free(conjunto->elementos); free(conjunto); }
}

/* ------------------------------------------------------------------ */
/*  Escáner / Traductor principal                                       */
/* ------------------------------------------------------------------ */

/*
 * procesar_fuente - realiza dos tareas en un solo recorrido del buffer:
 *   a) Acumula las keywords únicas encontradas para el resumen.
 *   b) Escribe el contenido traducido al archivo de salida (o stdout).
 *
 * Las cadenas de texto ("..."), literales de carácter ('...') y comentarios
 * se copian sin modificar para evitar falsos positivos.
 */
static void procesar_fuente(const char *fuente, FILE *salida,
                             ConjuntoCadenas *encontradas)
{
    const char *posicion = fuente;
    char token[128];

    while (*posicion) {

        /* --- Comentario de una línea: copiar tal cual --- */
        if (posicion[0] == '/' && posicion[1] == '/') {
            while (*posicion && *posicion != '\n') {
                fputc(*posicion++, salida);
            }
            continue;
        }

        /* --- Comentario multilínea: copiar tal cual --- */
        if (posicion[0] == '/' && posicion[1] == '*') {
            fputc(*posicion++, salida); /* '/' */
            fputc(*posicion++, salida); /* '*' */
            while (*posicion && !(posicion[0] == '*' && posicion[1] == '/')) {
                fputc(*posicion++, salida);
            }
            if (*posicion) {
                fputc(*posicion++, salida); /* '*' */
                fputc(*posicion++, salida); /* '/' */
            }
            continue;
        }

        /* --- Cadena de texto: copiar tal cual --- */
        if (*posicion == '"') {
            fputc(*posicion++, salida);
            while (*posicion && *posicion != '"') {
                if (*posicion == '\\') fputc(*posicion++, salida);
                if (*posicion) fputc(*posicion++, salida);
            }
            if (*posicion) fputc(*posicion++, salida); /* comilla de cierre */
            continue;
        }

        /* --- Literal de carácter: copiar tal cual --- */
        if (*posicion == '\'') {
            fputc(*posicion++, salida);
            while (*posicion && *posicion != '\'') {
                if (*posicion == '\\') fputc(*posicion++, salida);
                if (*posicion) fputc(*posicion++, salida);
            }
            if (*posicion) fputc(*posicion++, salida);
            continue;
        }

        /* --- Identificador o palabra clave --- */
        if (es_caracter_identificador(*posicion) &&
            !isdigit((unsigned char)*posicion))
        {
            int longitud = 0;
            while (es_caracter_identificador(*posicion) &&
                   longitud < (int)sizeof(token) - 1)
            {
                token[longitud++] = *posicion++;
            }
            token[longitud] = '\0';

            const EntradaPalabraClave *entrada = buscar_palabra_clave(token);
            if (entrada) {
                /* Registrar en el resumen (sin duplicados) */
                conjunto_agregar(encontradas, entrada->palabra_clave);
                /* Escribir la TRADUCCIÓN en lugar de la keyword original */
                fputs(entrada->traduccion_espanol, salida);
            } else {
                /* No es keyword: copiar el token sin cambios */
                fputs(token, salida);
            }
            continue;
        }

        /* --- Cualquier otro carácter: copiar tal cual --- */
        fputc(*posicion++, salida);
    }
}

/* ------------------------------------------------------------------ */
/*  Función principal                                                   */
/* ------------------------------------------------------------------ */
int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <archivo_fuente.c>\n", argv[0]);
        return 1;
    }

    /* Cargar el archivo fuente en memoria dinámica */
    size_t tamano_archivo  = 0;
    char  *contenido_fuente = cargar_archivo(argv[1], &tamano_archivo);
    if (!contenido_fuente) return 1;

    ConjuntoCadenas *encontradas = conjunto_nuevo();
    if (!encontradas) { free(contenido_fuente); return 1; }

    /* ---- Paso 1: resumen de keywords encontradas ---- */
    printf("=== Detector de Palabras Reservadas de C ===\n");
    printf("Archivo : %s\n", argv[1]);
    printf("Tamaño  : %zu bytes\n\n", tamano_archivo);
    printf("Palabra clave (inglés)   ->  Traducción (español)\n");
    printf("--------------------------------------------------\n");

    /*
     * Primera pasada: solo para construir el resumen de keywords únicas.
     * Se descarta la salida enviándola a /dev/null.
     */
    FILE *descarte = fopen("/dev/null", "w");
    if (!descarte) { perror("fopen /dev/null"); descarte = stderr; }
    procesar_fuente(contenido_fuente, descarte, encontradas);
    if (descarte != stderr) fclose(descarte);

    /* Mostrar el resumen ordenado según aparecen en la tabla */
    for (int i = 0; i < TOTAL_PALABRAS_CLAVE; i++) {
        if (conjunto_contiene(encontradas, PALABRAS_CLAVE[i].palabra_clave)) {
            printf("  %-16s ->  %s\n",
                   PALABRAS_CLAVE[i].palabra_clave,
                   PALABRAS_CLAVE[i].traduccion_espanol);
        }
    }

    if (encontradas->cantidad == 0) {
        printf("  (no se encontraron palabras reservadas de C)\n");
    } else {
        printf("\nTotal de palabras clave únicas encontradas: %d\n",
               encontradas->cantidad);
    }

    /* Limpiar el conjunto para la segunda pasada */
    conjunto_liberar(encontradas);
    encontradas = conjunto_nuevo();

    /* ---- Paso 2: mostrar el archivo con las keywords traducidas ---- */
    printf("\n");
    printf("==================================================\n");
    printf("  ARCHIVO TRADUCIDO AL ESPAÑOL\n");
    printf("==================================================\n\n");

    procesar_fuente(contenido_fuente, stdout, encontradas);

    printf("\n==================================================\n");
    printf("  FIN DEL ARCHIVO TRADUCIDO\n");
    printf("==================================================\n");

    /* Liberar toda la memoria dinámica reservada */
    conjunto_liberar(encontradas);
    free(contenido_fuente);
    return 0;
}
