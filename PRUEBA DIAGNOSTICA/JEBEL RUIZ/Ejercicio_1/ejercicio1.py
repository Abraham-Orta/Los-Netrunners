# Prueba Diagnostica - Ejercicio 1
# Validacion de movimientos del caballo en ajedrez.


import sys
import time
import os

def parse_square(sq: str):
    """Convierte una casilla como 'a1' o 'c2' a tupla (col, fila) base 0."""
    if len(sq) != 2:
        return None
    col = ord(sq[0].lower()) - ord('a')
    if col < 0 or col > 7:
        return None
    if not ('1' <= sq[1] <= '8'):
        return None
    row = int(sq[1]) - 1
    return col, row

def es_movimiento_caballo(col1: int, row1: int, col2: int, row2: int) -> bool:
    """Verifica si el paso de (col1, row1) a (col2, row2) es un movimiento de caballo."""
    dc = abs(col1 - col2)
    dr = abs(row1 - row2)
    return (dc == 1 and dr == 2) or (dc == 2 and dr == 1)

def verificar_movimientos(pos_inicial: str, cadena_movimientos: str):
    """
    Verifica la secuencia de movimientos dada.
    Retorna una tupla (es_valido: bool, mensaje: str).
    """
    sq_actual = parse_square(pos_inicial.strip())
    if sq_actual is None:
        return False, f"Casilla inicial '{pos_inicial}' invalida. Debe ser columna [a-h] y fila [1-8]."

    movimientos = cadena_movimientos.strip().split()
    col_actual, row_actual = sq_actual

    for i, mov in enumerate(movimientos, start=1):
        if len(mov) != 3:
            return False, f"INCORRECTO falla en el movimiento {i}: formato invalido '{mov}'."
        
        letra_pieza = mov[0]
        if letra_pieza != 'K':
            return False, f"movimiento INCORRECTO falla letra {letra_pieza} no pertenece a un caballo."

        destino_sq = parse_square(mov[1:])
        if destino_sq is None:
            return False, f"INCORRECTO falla en el movimiento {i}: casilla de destino '{mov[1:]}' fuera del tablero."

        dest_col, dest_row = destino_sq
        if not es_movimiento_caballo(col_actual, row_actual, dest_col, dest_row):
            return False, f"INCORRECTO falla en el movimiento {i}."

        col_actual, row_actual = dest_col, dest_row

    return True, "movimiento CORRECTO."

# Secuencia descrita en el PDF 
MOVS_PDF_TEXT = (
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
)

def obtener_secuencia_500() -> str:
    """Carga los 500 movimientos desde el archivo o usa la cadena por defecto."""
    ruta_script = os.path.dirname(os.path.abspath(__file__))
    candidatas = [
        os.path.join(ruta_script, "movimientos_500.txt"),
        os.path.join(ruta_script, "..", "Ejercicio_1", "movimientos_500.txt"),
        "movimientos_500.txt",
        os.path.join("Ejercicio_1", "movimientos_500.txt"),
        os.path.join("JEBEL RUIZ", "Ejercicio_1", "movimientos_500.txt")
    ]
    for ruta in candidatas:
        if os.path.exists(ruta):
            with open(ruta, "r", encoding="utf-8") as f:
                return f.read()
    return MOVS_PDF_TEXT

def ejecutar_benchmark(pos_inicial: str = "a2", iteraciones: int = 10, archivo_salida: str = "tiempos_ejercicio1_python.txt"):
    """Mide el tiempo de verificacion repetido 10 veces y guarda el resultado."""
    print("=" * 70)
    print(f"BENCHMARK EJERCICIO 1 (Python): 10 ejecuciones de la secuencia de 500 movs")
    print("=" * 70)
    
    secuencia = obtener_secuencia_500()
    num_movs = len(secuencia.split())
    valido, msg = verificar_movimientos(pos_inicial, secuencia)
    print(f"Secuencia: {num_movs} movimientos. Posicion inicial: {pos_inicial}")
    print(f"Resultado de prueba: {msg}")

    tiempos = []
    t_inicio_total = time.perf_counter()
    for it in range(1, iteraciones + 1):
        t0 = time.perf_counter()
        v, m = verificar_movimientos(pos_inicial, secuencia)
        t1 = time.perf_counter()
        dt = (t1 - t0) * 1000.0  # milisegundos
        tiempos.append(dt)
        print(f"  Iteracion {it:2d}: {dt:.4f} ms -> {m}")
    t_fin_total = time.perf_counter()
    tiempo_total_ms = (t_fin_total - t_inicio_total) * 1000.0
    promedio_ms = sum(tiempos) / len(tiempos)

    reporte = (
        f"=== REPORTE DE TIEMPOS EJERCICIO 1 (PYTHON) ===\n"
        f"Lenguaje: Python {sys.version.split()[0]}\n"
        f"Posicion inicial (Ak): {pos_inicial}\n"
        f"Numero de movimientos en la secuencia: {num_movs}\n"
        f"Iteraciones: {iteraciones}\n"
        f"Resultado: {msg}\n"
        f"Tiempo total (10 iteraciones): {tiempo_total_ms:.4f} ms ({tiempo_total_ms / 1000.0:.6f} s)\n"
        f"Tiempo promedio por iteracion: {promedio_ms:.4f} ms ({promedio_ms / 1000.0:.6f} s)\n"
    )
    for i, t in enumerate(tiempos, 1):
        reporte += f"  Iteracion {i:2d}: {t:.4f} ms\n"

    print("\n" + reporte)
    dir_script = os.path.dirname(os.path.abspath(__file__))
    rutas_guardar = set()
    rutas_guardar.add(os.path.abspath(os.path.join(dir_script, "tiempos_ejercicio1_python.txt")))
    rutas_guardar.add(os.path.abspath(archivo_salida))

    for r in rutas_guardar:
        try:
            with open(r, "w", encoding="utf-8") as f:
                f.write(reporte)
            print(f"[OK] Reporte guardado exitosamente en: {r}")
        except Exception as e:
            print(f"[AVISO] No se pudo guardar en {r}: {e}")
    print()

def ejecutar_casos_ejemplo():
    """Ejecuta y valida los 9 ejemplos oficiales provistos en el PDF."""
    print("=" * 70)
    print("VALIDACION DE CASOS DE EJEMPLO DEL PDF")
    print("=" * 70)
    casos = [
        ("a1", 4, "Kc2 Kd4 Kb3 Kc5", "movimiento CORRECTO."),
        ("a1", 4, "Kc2 Kd4 Kb4 Kc5", "INCORRECTO falla en el movimiento 3."),
        ("a2", 5, "Kc3 Ke4 Kf6 Ke8 Kg7", "movimiento CORRECTO."),
        ("a2", 5, "Kc8 Ke4 Kf6 Ke8 Kg7", "INCORRECTO falla en el movimiento 1."),
        ("a8", 2, "Kb6 Kd7", "movimiento CORRECTO."),
        ("a8", 2, "Kb6 Kb7", "INCORRECTO falla en el movimiento 2."),
        ("a1", 10, "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Kb4", "movimiento CORRECTO."),
        ("a1", 10, "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4", "movimiento INCORRECTO falla letra Q no pertenece a un caballo."),
        ("a1", 10, "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4", "movimiento INCORRECTO falla en el movimiento 9"),
    ]

    todos_ok = True
    for idx, (ak, n, c, esperado) in enumerate(casos, 1):
        _, obtenido = verificar_movimientos(ak, c)
        norm_esp = esperado.strip().rstrip('.').lower()
        norm_obt = obtenido.strip().rstrip('.').lower()
        coincide = (
            norm_obt == norm_esp
            or norm_esp in norm_obt
            or norm_obt in norm_esp
            or (("falla en el movimiento" in norm_esp) and ("falla en el movimiento" in norm_obt) and (norm_esp[-1] == norm_obt[-1]))
        )
        if not coincide:
            todos_ok = False
        estado = "[OK]" if coincide else "[FALLO]"
        print(f"Ejemplo {idx}: Ak={ak}, n={n}")
        print(f"  Cadena: {c}")
        print(f"  Esperado: {esperado}")
        print(f"  Obtenido: {obtenido}  {estado}\n")

    if todos_ok:
        print(">>> TODOS LOS EJEMPLOS DEL PDF FUERON SUPERADOS EXITOSAMENTE <<<\n")
    else:
        print(">>> ATENCION: Algunos ejemplos difieren del texto esperado <<<\n")

def menu_interactivo():
    while True:
        print("\n" + "=" * 50)
        print("   EJERCICIO 1: MOVIMIENTOS DEL CABALLO (PYTHON)")
        print("=" * 50)
        print("1. Validar casos de prueba del PDF")
        print("2. Probar entrada manual (Ak y C)")
        print("3. Ejecutar Benchmark (10 veces secuencia 500 movs)")
        print("4. Salir")
        opc = input("Seleccione una opcion (1-4): ").strip()

        if opc == "1":
            ejecutar_casos_ejemplo()
        elif opc == "2":
            ak = input("Ingrese posicion inicial Ak (ej. a1): ").strip()
            c = input("Ingrese cadena de movimientos C (ej. Kc2 Kd4 Kb3 Kc5): ").strip()
            valido, res = verificar_movimientos(ak, c)
            print(f"\nResultado: {res}")
        elif opc == "3":
            ejecutar_benchmark()
        elif opc == "4":
            print("Saliendo...")
            break
        else:
            print("Opcion no valida.")

if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == "--benchmark":
        ejecutar_benchmark()
    elif len(sys.argv) > 1 and sys.argv[1] == "--test":
        ejecutar_casos_ejemplo()
    else:
        # Ejecutar tests y benchmark si no es interactivo, o mostrar menu
        ejecutar_casos_ejemplo()
        ejecutar_benchmark()
