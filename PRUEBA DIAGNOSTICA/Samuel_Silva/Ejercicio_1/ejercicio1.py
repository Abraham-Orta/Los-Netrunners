import time
import sys
import os

# Movimientos del caballo (en L: 2 en un eje y 1 en el otro)
def es_salto_valido(dc, df):
    return abs(dc) * abs(df) == 2

def dentro_tablero(col, fila):
    return 'a' <= col <= 'h' and '1' <= fila <= '8'

# Buffer de entrada para permitir ingresar todo junto o linea por linea
_buffer_tokens = []
def pedir_token(mensaje=""):
    global _buffer_tokens
    while not _buffer_tokens:
        try:
            linea = input(mensaje).strip()
        except EOFError:
            return None
        if not linea:
            continue
        _buffer_tokens.extend(linea.split())
        mensaje = ""
    return _buffer_tokens.pop(0)

# Guarda el tiempo en tiempos.txt
def guardar_tiempo_py(duracion):
    carpeta = os.path.dirname(os.path.abspath(__file__))
    ruta = os.path.join(carpeta, "tiempos.txt")
    with open(ruta, "w", encoding="utf-8") as f:
        f.write(f"Tiempo de verificacion 10x500 movimientos en Python: {duracion:.8f} segundos\n")
    print("[OK] Tiempo de medicion guardado en 'tiempos.txt'.")

def main():
    print(" Validador de movimientos de caballo ")
    pos_ini = pedir_token("Posicion inicial (ej: a1): ")
    if not pos_ini or len(pos_ini) != 2 or not dentro_tablero(pos_ini[0], pos_ini[1]):
        print("Posicion inicial invalida.")
        return

    token_n = pedir_token("Cantidad de movimientos: ")
    try:
        n = int(token_n)
    except (ValueError, TypeError):
        print("Cantidad invalida.")
        return

    col_act = pos_ini[0]
    fil_act = pos_ini[1]
    paso_falla = 0
    letra_invalida = None

    if not _buffer_tokens:
        print(f"Ingrese los {n} movimientos (ej: Kc2 Kd4...):")

    for i in range(n):
        m = pedir_token()
        if not m:
            paso_falla = i + 1
            break

        if len(m) != 3:
            paso_falla = i + 1
            break

        if m[0] != 'K':
            letra_invalida = m[0]
            paso_falla = i + 1
            break

        col_dest = m[1]
        fil_dest = m[2]

        if not dentro_tablero(col_dest, fil_dest):
            paso_falla = i + 1
            break

        dc = ord(col_dest) - ord(col_act)
        df = int(fil_dest) - int(fil_act)

        if not es_salto_valido(dc, df):
            paso_falla = i + 1
            break

        col_act = col_dest
        fil_act = fil_dest

    # Resultado
    if paso_falla == 0:
        print("Resultado: movimiento CORRECTO.")
    elif letra_invalida is not None:
        print(f"Resultado: movimiento INCORRECTO falla letra '{letra_invalida}' no pertenece a un caballo.")
    else:
        print(f"Resultado: INCORRECTO falla en el movimiento {paso_falla}.")

    # Medicion de tiempo sobre los 500 movimientos
    sec500 = (
        "Kb4 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 "
        "Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 "
        "Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 "
        "Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 "
        "Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 "
        "Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 "
        "Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 "
        "Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 "
        "Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 "
        "Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 "
        "Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 "
        "Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
        "Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 "
        "Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 "
        "Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 "
        "Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 "
        "Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 "
        "Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 "
        "Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 "
        "Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 "
        "Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 "
        "Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 "
        "Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 "
        "Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 "
        "Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 "
    ).split()

    inicio = time.perf_counter()
    for _ in range(10):
        c_act, f_act = 'a', '2'
        for m in sec500:
            dc = ord(m[1]) - ord(c_act)
            df = int(m[2]) - int(f_act)
            if es_salto_valido(dc, df):
                c_act, f_act = m[1], m[2]
    fin = time.perf_counter()

    guardar_tiempo_py(fin - inicio)

if __name__ == "__main__":
    main()
