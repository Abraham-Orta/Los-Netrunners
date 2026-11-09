import sys
import os

# Diccionario de palabras reservadas en C y su traduccion al espanol
TABLA = {
    "auto": "automatico", "break": "romper", "case": "caso",
    "char": "caracter", "const": "constante", "continue": "continuar",
    "default": "por defecto", "do": "hacer", "double": "doble",
    "else": "sino", "enum": "enumeracion", "extern": "externo",
    "float": "flotante", "for": "para", "goto": "ir a",
    "if": "si", "inline": "en linea", "int": "entero",
    "long": "largo", "register": "registro", "restrict": "restringir",
    "return": "retornar", "short": "corto", "signed": "con signo",
    "sizeof": "tamano de", "static": "estatico", "struct": "estructura",
    "switch": "selector", "typedef": "definir tipo", "union": "union",
    "unsigned": "sin signo", "void": "vacio", "volatile": "volatil",
    "while": "mientras"
}

def procesar_codigo(contenido):
    num_linea = 1
    print(f"Linea {num_linea:3d}: ", end="")
    i = 0
    n = len(contenido)

    while i < n:
        if contenido[i:i+2] == '//':
            while i < n and contenido[i] != '\n':
                print(contenido[i], end="")
                i += 1
            continue

        if contenido[i:i+2] == '/*':
            print(contenido[i:i+2], end="")
            i += 2
            while i < n and contenido[i:i+2] != '*/':
                if contenido[i] == '\n':
                    print()
                    num_linea += 1
                    print(f"Linea {num_linea:3d}: ", end="")
                else:
                    print(contenido[i], end="")
                i += 1
            if i < n:
                print('*/', end="")
                i += 2
            continue

        if contenido[i] == '"':
            print(contenido[i], end="")
            i += 1
            while i < n and contenido[i] != '"':
                if contenido[i] == '\\':
                    print(contenido[i], end="")
                    i += 1
                if i < n:
                    print(contenido[i], end="")
                    i += 1
            if i < n:
                print('"', end="")
                i += 1
            continue

        if contenido[i] == "'":
            print(contenido[i], end="")
            i += 1
            while i < n and contenido[i] != "'":
                if contenido[i] == '\\':
                    print(contenido[i], end="")
                    i += 1
                if i < n:
                    print(contenido[i], end="")
                    i += 1
            if i < n:
                print("'", end="")
                i += 1
            continue

        if contenido[i] == '\n':
            print()
            num_linea += 1
            print(f"Linea {num_linea:3d}: ", end="")
            i += 1
            continue

        if contenido[i].isalpha() or contenido[i] == '_':
            inicio = i
            while i < n and (contenido[i].isalnum() or contenido[i] == '_'):
                i += 1
            palabra = contenido[inicio:i]
            if palabra in TABLA:
                print(f"[{TABLA[palabra]}]", end="")
            else:
                print(palabra, end="")
            continue

        print(contenido[i], end="")
        i += 1
    print()

def main():
    if len(sys.argv) >= 2:
        ruta = sys.argv[1]
    else:
        # Buscar en directorio actual o en la carpeta del script
        if os.path.exists("programa_prueba.c"):
            ruta = "programa_prueba.c"
        else:
            carpeta = os.path.dirname(os.path.abspath(__file__))
            ruta = os.path.join(carpeta, "programa_prueba.c")
        print(f"Usando '{ruta}' por defecto...\n")

    if not os.path.exists(ruta):
        print(f"Error: No se encontro el archivo '{ruta}'.")
        return

    with open(ruta, "r", encoding="utf-8", errors="replace") as f:
        texto = f.read()

    print(f"TRADUCCION DE PALABRAS RESERVADAS EN C: {ruta}\n")
    procesar_codigo(texto)

if __name__ == "__main__":
    main()
