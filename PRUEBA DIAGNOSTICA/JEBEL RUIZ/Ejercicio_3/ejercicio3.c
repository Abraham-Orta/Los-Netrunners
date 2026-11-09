#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Estructura para registrar cada palabra reservada y su traducción */
typedef struct {
    const char *en;
    const char *es;
    int conteo;
    int *lineas; // Arreglo dinámico de líneas donde aparece
    int cap_lineas;
} EntradaPalabra;

/* Tabla completa de palabras reservadas estándar en C */
static EntradaPalabra TABLA_PALABRAS[] = {
    {"auto", "automatico", 0, NULL, 0},
    {"break", "romper", 0, NULL, 0},
    {"case", "caso", 0, NULL, 0},
    {"char", "caracter", 0, NULL, 0},
    {"const", "constante", 0, NULL, 0},
    {"continue", "continuar", 0, NULL, 0},
    {"default", "defecto", 0, NULL, 0},
    {"do", "hacer", 0, NULL, 0},
    {"double", "doble", 0, NULL, 0},
    {"else", "sino", 0, NULL, 0},
    {"enum", "enumeracion", 0, NULL, 0},
    {"extern", "externo", 0, NULL, 0},
    {"float", "flotante", 0, NULL, 0},
    {"for", "para", 0, NULL, 0},
    {"goto", "ira", 0, NULL, 0},
    {"if", "si", 0, NULL, 0},
    {"inline", "en_linea", 0, NULL, 0},
    {"int", "entero", 0, NULL, 0},
    {"long", "largo", 0, NULL, 0},
    {"register", "registro", 0, NULL, 0},
    {"restrict", "restringido", 0, NULL, 0},
    {"return", "retornar", 0, NULL, 0},
    {"short", "corto", 0, NULL, 0},
    {"signed", "con_signo", 0, NULL, 0},
    {"sizeof", "tamano_de", 0, NULL, 0},
    {"static", "estatico", 0, NULL, 0},
    {"struct", "estructura", 0, NULL, 0},
    {"switch", "segun", 0, NULL, 0},
    {"typedef", "definir_tipo", 0, NULL, 0},
    {"union", "union", 0, NULL, 0},
    {"unsigned", "sin_signo", 0, NULL, 0},
    {"void", "vacio", 0, NULL, 0},
    {"volatile", "volatil", 0, NULL, 0},
    {"while", "mientras", 0, NULL, 0},
    {"_Bool", "_Booleano", 0, NULL, 0},
    {"_Complex", "_Complejo", 0, NULL, 0},
    {"_Imaginary", "_Imaginario", 0, NULL, 0}
};

static const int NUM_PALABRAS = sizeof(TABLA_PALABRAS) / sizeof(TABLA_PALABRAS[0]);

/* Busca si un token es una palabra reservada */
int buscar_palabra_reservada(const char *tok) {
    for (int i = 0; i < NUM_PALABRAS; i++) {
        if (strcmp(TABLA_PALABRAS[i].en, tok) == 0) {
            return i;
        }
    }
    return -1;
}

/* Registra la ocurrencia y número de línea de una palabra */
void registrar_ocurrencia(int idx, int linea) {
    EntradaPalabra *e = &TABLA_PALABRAS[idx];
    e->conteo++;
    if (e->conteo > e->cap_lineas) {
        int nueva_cap = (e->cap_lineas == 0) ? 8 : e->cap_lineas * 2;
        int *nuevas_lineas = (int *)realloc(e->lineas, nueva_cap * sizeof(int));
        if (nuevas_lineas) {
            e->lineas = nuevas_lineas;
            e->cap_lineas = nueva_cap;
        }
    }
    if (e->lineas) {
        e->lineas[e->conteo - 1] = linea;
    }
}

/* Buffer dinámico autoexpansible para el código resultante */
typedef struct {
    char *data;
    size_t len;
    size_t cap;
} BufferDinamico;

void buffer_init(BufferDinamico *b, size_t cap_inicial) {
    b->cap = cap_inicial > 64 ? cap_inicial : 64;
    b->len = 0;
    b->data = (char *)malloc(b->cap);
    b->data[0] = '\0';
}

void buffer_append_str(BufferDinamico *b, const char *s) {
    size_t slen = strlen(s);
    if (b->len + slen + 1 > b->cap) {
        b->cap = (b->cap + slen) * 2;
        b->data = (char *)realloc(b->data, b->cap);
    }
    memcpy(b->data + b->len, s, slen);
    b->len += slen;
    b->data[b->len] = '\0';
}

void buffer_append_char(BufferDinamico *b, char c) {
    if (b->len + 2 > b->cap) {
        b->cap *= 2;
        b->data = (char *)realloc(b->data, b->cap);
    }
    b->data[b->len++] = c;
    b->data[b->len] = '\0';
}

void buffer_free(BufferDinamico *b) {
    if (b->data) {
        free(b->data);
        b->data = NULL;
    }
    b->len = b->cap = 0;
}

/* Carga dinámicamente un archivo completo en memoria */
char *cargar_archivo_en_memoria(const char *ruta, size_t *tam_out) {
    FILE *f = fopen(ruta, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long tam = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (tam < 0) {
        fclose(f);
        return NULL;
    }

    char *mem = (char *)malloc(tam + 1);
    if (!mem) {
        fclose(f);
        return NULL;
    }

    size_t leidos = fread(mem, 1, tam, f);
    mem[leidos] = '\0';
    fclose(f);

    if (tam_out) *tam_out = leidos;
    return mem;
}

int main(int argc, char *argv[]) {
    const char *ruta_entrada = (argc > 1) ? argv[1] : "ejemplo_entrada.c";
    const char *ruta_salida = (argc > 2) ? argv[2] : "ejemplo_traducido_c.c";

    printf("======================================================================\n");
    printf("        EJERCICIO 3 (C): TRADUCTOR DE PALABRAS RESERVADAS EN C        \n");
    printf("======================================================================\n");
    printf("Cargando archivo dinamicamente en memoria: '%s'\n", ruta_entrada);

    size_t tam_archivo = 0;
    char *codigo = cargar_archivo_en_memoria(ruta_entrada, &tam_archivo);
    if (!codigo) {
        fprintf(stderr, "ERROR: No se pudo abrir o cargar el archivo '%s'.\n", ruta_entrada);
        return 1;
    }

    printf("Tamano cargado en memoria dinamica: %zu bytes.\n\n", tam_archivo);

    BufferDinamico salida;
    buffer_init(&salida, tam_archivo * 2);

    size_t pos = 0;
    int linea = 1;

    /* Analizador léxico sobre la memoria dinámica */
    while (pos < tam_archivo) {
        char c = codigo[pos];

        /* 1. Comentario de línea // */
        if (c == '/' && pos + 1 < tam_archivo && codigo[pos + 1] == '/') {
            while (pos < tam_archivo && codigo[pos] != '\n') {
                buffer_append_char(&salida, codigo[pos++]);
            }
            continue;
        }

        /* 2. Comentario de bloque /* */
        if (c == '/' && pos + 1 < tam_archivo && codigo[pos + 1] == '*') {
            buffer_append_char(&salida, codigo[pos++]);
            buffer_append_char(&salida, codigo[pos++]);
            while (pos + 1 < tam_archivo && !(codigo[pos] == '*' && codigo[pos + 1] == '/')) {
                if (codigo[pos] == '\n') linea++;
                buffer_append_char(&salida, codigo[pos++]);
            }
            if (pos + 1 < tam_archivo) {
                buffer_append_char(&salida, codigo[pos++]);
                buffer_append_char(&salida, codigo[pos++]);
            }
            continue;
        }

        /* 3. Literal de cadena "..." */
        if (c == '"') {
            buffer_append_char(&salida, codigo[pos++]);
            while (pos < tam_archivo && codigo[pos] != '"') {
                if (codigo[pos] == '\\' && pos + 1 < tam_archivo) {
                    buffer_append_char(&salida, codigo[pos++]);
                    buffer_append_char(&salida, codigo[pos++]);
                } else {
                    if (codigo[pos] == '\n') linea++;
                    buffer_append_char(&salida, codigo[pos++]);
                }
            }
            if (pos < tam_archivo) {
                buffer_append_char(&salida, codigo[pos++]); // Comilla de cierre
            }
            continue;
        }

        /* 4. Literal de carácter '.' */
        if (c == '\'') {
            buffer_append_char(&salida, codigo[pos++]);
            while (pos < tam_archivo && codigo[pos] != '\'') {
                if (codigo[pos] == '\\' && pos + 1 < tam_archivo) {
                    buffer_append_char(&salida, codigo[pos++]);
                    buffer_append_char(&salida, codigo[pos++]);
                } else {
                    if (codigo[pos] == '\n') linea++;
                    buffer_append_char(&salida, codigo[pos++]);
                }
            }
            if (pos < tam_archivo) {
                buffer_append_char(&salida, codigo[pos++]);
            }
            continue;
        }

        /* 5. Identificador o Palabra Reservada */
        if (isalpha((unsigned char)c) || c == '_') {
            size_t inicio = pos;
            while (pos < tam_archivo && (isalnum((unsigned char)codigo[pos]) || codigo[pos] == '_')) {
                pos++;
            }
            size_t tok_len = pos - inicio;
            char *tok = (char *)malloc(tok_len + 1);
            memcpy(tok, codigo + inicio, tok_len);
            tok[tok_len] = '\0';

            int idx = buscar_palabra_reservada(tok);
            if (idx >= 0) {
                buffer_append_str(&salida, TABLA_PALABRAS[idx].es);
                registrar_ocurrencia(idx, linea);
            } else {
                buffer_append_str(&salida, tok);
            }
            free(tok);
            continue;
        }

        /* 6. Caracter ordinario */
        if (c == '\n') linea++;
        buffer_append_char(&salida, c);
        pos++;
    }

    /* Guardar archivo traducido con encabezado de compatibilidad para compilador y editor */
    FILE *f_out = fopen(ruta_salida, "wb");
    if (f_out) {
        const char *encabezado = 
            "/*\n"
            " * CODIGO TRADUCIDO AL ESPANOL (Ejercicio 3 - UNEG)\n"
            " * Se incluye 'c_espanol.h' para que el compilador y el editor reconozcan\n"
            " * las palabras traducidas y el programa sea 100% valido y ejecutable.\n"
            " */\n"
            "#include \"c_espanol.h\"\n\n";
        fwrite(encabezado, 1, strlen(encabezado), f_out);
        fwrite(salida.data, 1, salida.len, f_out);
        fclose(f_out);
        printf("[OK] Codigo traducido guardado exitosamente en '%s'\n\n", ruta_salida);
    }

    /* Mostrar reporte de palabras reservadas detectadas */
    printf("----------------------------------------------------------------------\n");
    printf("%-15s %-15s %-12s %s\n", "Palabra C", "Traduccion", "Frecuencia", "Lineas donde aparece");
    printf("----------------------------------------------------------------------\n");

    int total_detectadas = 0;
    int distintas = 0;

    FILE *f_rep = fopen("reporte_traduccion_c.txt", "w");
    if (f_rep) {
        fprintf(f_rep, "=== REPORTE EJERCICIO 3: PALABRAS RESERVADAS EN C (LENGUAJE C) ===\n");
        fprintf(f_rep, "Archivo analizado: %s (%zu bytes)\n\n", ruta_entrada, tam_archivo);
    }

    for (int i = 0; i < NUM_PALABRAS; i++) {
        if (TABLA_PALABRAS[i].conteo > 0) {
            distintas++;
            total_detectadas += TABLA_PALABRAS[i].conteo;
            printf("%-15s %-15s %-12d ", TABLA_PALABRAS[i].en, TABLA_PALABRAS[i].es, TABLA_PALABRAS[i].conteo);
            if (f_rep) {
                fprintf(f_rep, "- '%s' -> '%s': %d veces (Lineas: ", 
                        TABLA_PALABRAS[i].en, TABLA_PALABRAS[i].es, TABLA_PALABRAS[i].conteo);
            }

            int limite = TABLA_PALABRAS[i].conteo < 8 ? TABLA_PALABRAS[i].conteo : 8;
            for (int k = 0; k < limite; k++) {
                printf("%d%s", TABLA_PALABRAS[i].lineas[k], (k < limite - 1) ? ", " : "");
                if (f_rep) fprintf(f_rep, "%d%s", TABLA_PALABRAS[i].lineas[k], (k < limite - 1) ? ", " : "");
            }
            if (TABLA_PALABRAS[i].conteo > 8) {
                printf(" ... (+%d mas)", TABLA_PALABRAS[i].conteo - 8);
                if (f_rep) fprintf(f_rep, " ... (+%d mas)", TABLA_PALABRAS[i].conteo - 8);
            }
            printf("\n");
            if (f_rep) fprintf(f_rep, ")\n");
        }
    }

    printf("----------------------------------------------------------------------\n");
    printf("Total de palabras reservadas detectadas y traducidas: %d (de %d palabras distintas)\n", total_detectadas, distintas);
    printf("----------------------------------------------------------------------\n");

    if (f_rep) {
        fprintf(f_rep, "\nTotal palabras detectadas: %d\n", total_detectadas);
        fprintf(f_rep, "Total tipos de palabras distintas: %d\n", distintas);
        fclose(f_rep);
        printf("[OK] Reporte estadistico guardado en 'reporte_traduccion_c.txt'\n\n");
    }

    /* Limpieza de memoria dinámica */
    free(codigo);
    buffer_free(&salida);
    for (int i = 0; i < NUM_PALABRAS; i++) {
        if (TABLA_PALABRAS[i].lineas) {
            free(TABLA_PALABRAS[i].lineas);
            TABLA_PALABRAS[i].lineas = NULL;
        }
    }

    return 0;
}
