# Prueba Diagnostica - Ejercicio 2 (Python)


import sys
import time

def generar_triangulo_pascal(n: int):
    """
    Genera el Triangulo de Pascal hasta la fila n usando listas dinamicas en memoria.
    Retorna la lista de filas, donde la fila i tiene i + 1 coeficientes.
    """
    if n < 0:
        raise ValueError("n debe ser un entero no negativo.")

    triangulo = []
    for i in range(n + 1):
        fila = [1] * (i + 1)
        for j in range(1, i):
            fila[j] = triangulo[i - 1][j - 1] + triangulo[i - 1][j]
        triangulo.append(fila)
    return triangulo

def formatear_polinomio(coeficientes: list) -> str:
    """
    Formatea la lista de coeficientes [c_n, c_{n-1}, ..., c_0]
    como un polinomio algebraico legible en potencias decrecientes de x.
    Ejemplo n=3: x^3 + 3x^2 + 3x + 1
    """
    n = len(coeficientes) - 1
    if n == 0:
        return str(coeficientes[0])

    terminos = []
    for idx, c in enumerate(coeficientes):
        grado = n - idx
        if c == 0:
            continue

        if grado == 0:
            terminos.append(f"{c}")
        elif grado == 1:
            if c == 1:
                terminos.append("x")
            else:
                terminos.append(f"{c}x")
        else:
            if c == 1:
                terminos.append(f"x^{grado}")
            else:
                terminos.append(f"{c}x^{grado}")

    return " + ".join(terminos)

def evaluar_por_pasos(coeficientes: list, x: float):
    """
    Muestra paso a paso el calculo de f(x) = (x+1)^n segun el polinomio generado.
    """
    n = len(coeficientes) - 1
    print("\n" + "=" * 70)
    print(f"EVALUACION PASO A PASO: f({x}) = (x + 1)^{n}")
    print("=" * 70)
    print(f"Polinomio expandido: P(x) = {formatear_polinomio(coeficientes)}\n")

    suma_acumulada = 0.0
    for idx, c in enumerate(coeficientes):
        grado = n - idx
        potencia_x = x ** grado
        termino_valor = c * potencia_x
        suma_acumulada += termino_valor

        print(f"Paso {idx + 1:2d} (grado k = {grado:2d}):")
        if grado > 1:
            print(f"  Termino: {c} * ({x})^{grado} = {c} * {potencia_x} = {termino_valor}")
        elif grado == 1:
            print(f"  Termino: {c} * ({x}) = {termino_valor}")
        else:
            print(f"  Termino independiente: {c} * ({x})^0 = {termino_valor}")
        print(f"  Suma acumulada: {suma_acumulada}\n")

    valor_directo = (x + 1.0) ** n
    print("-" * 70)
    print(f"Resultado final segun polinomio:  P({x}) = {suma_acumulada}")
    print(f"Comprobacion directa (x + 1)^{n}:  f({x}) = {valor_directo}")
    print(f"Diferencia / Error absoluto:      {abs(suma_acumulada - valor_directo):.6e}")
    print("-" * 70)
    return suma_acumulada

def benchmark_n100(repeticiones: int = 100, archivo_salida: str = "tiempos_ejercicio2_python.txt"):
    """
    Calcula el tiempo necesario para generar n=100 en Python.
    Realiza multiples repeticiones para obtener precision estadistica.
    """
    print("=" * 70)
    print("BENCHMARK EJERCICIO 2 (PYTHON): Generacion de n=100")
    print("=" * 70)

    # Calentamiento inicial
    t100 = generar_triangulo_pascal(100)

    tiempos = []
    for _ in range(repeticiones):
        t0 = time.perf_counter()
        t100 = generar_triangulo_pascal(100)
        t1 = time.perf_counter()
        tiempos.append((t1 - t0) * 1000.0)  # ms

    tiempo_min_ms = min(tiempos)
    tiempo_prom_ms = sum(tiempos) / len(tiempos)
    tiempo_total_ms = sum(tiempos)

    coef_central = t100[100][50]

    reporte = (
        f"=== REPORTE DE TIEMPOS EJERCICIO 2 (PYTHON) ===\n"
        f"Lenguaje: Python {sys.version.split()[0]}\n"
        f"Grado del polinomio: n = 100 (Triangulo de Pascal de 101 filas, 5151 coeficientes)\n"
        f"Repeticiones del benchmark: {repeticiones}\n"
        f"Coeficiente central C(100, 50):\n  {coef_central}\n"
        f"Numero de digitos del coeficiente central: {len(str(coef_central))}\n"
        f"Tiempo total ({repeticiones} ejecuciones): {tiempo_total_ms:.4f} ms ({tiempo_total_ms / 1000.0:.6f} s)\n"
        f"Tiempo promedio por generacion: {tiempo_prom_ms:.4f} ms ({tiempo_prom_ms / 1000.0:.6f} s)\n"
        f"Tiempo minimo (mejor ejecucion): {tiempo_min_ms:.4f} ms ({tiempo_min_ms / 1000.0:.6f} s)\n"
    )

    print(reporte)
    with open(archivo_salida, "w", encoding="utf-8") as f:
        f.write(reporte)
    print(f"[OK] Reporte guardado exitosamente en '{archivo_salida}'\n")

def menu_interactivo():
    while True:
        print("\n" + "=" * 55)
        print("   EJERCICIO 2: POLINOMIO (x+1)^n Y TRIANGULO DE PASCAL")
        print("=" * 55)
        print("1. Generar polinomio (x+1)^n para un n dado")
        print("2. Evaluar f(x) = (x+1)^n paso a paso para un x dado")
        print("3. Mostrar primeras 8 filas del Triangulo de Pascal (n=0..7)")
        print("4. Ejecutar Benchmark n=100")
        print("5. Salir")
        opc = input("Seleccione una opcion (1-5): ").strip()

        if opc == "1":
            try:
                n = int(input("Ingrese n (entero >= 0): ").strip())
                tri = generar_triangulo_pascal(n)
                coefs = tri[n]
                print(f"\nCoeficientes de (x+1)^{n}: {coefs}")
                print(f"Polinomio expandido: {formatear_polinomio(coefs)}")
            except ValueError as e:
                print(f"Error: {e}")
        elif opc == "2":
            try:
                n = int(input("Ingrese n (entero >= 0): ").strip())
                x = float(input("Ingrese valor de x (ej. 2): ").strip())
                tri = generar_triangulo_pascal(n)
                evaluar_por_pasos(tri[n], x)
            except ValueError as e:
                print(f"Error: {e}")
        elif opc == "3":
            tri = generar_triangulo_pascal(7)
            print("\nTriangulo de Pascal (n=0..7):")
            for i, r in enumerate(tri):
                print(f"(n={i}): " + " ".join(f"{v:2d}" for v in r))
                print(f"  -> (x+1)^{i} = {formatear_polinomio(r)}")
        elif opc == "4":
            benchmark_n100()
        elif opc == "5":
            print("Saliendo...")
            break
        else:
            print("Opcion no valida.")

if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == "--benchmark":
        benchmark_n100()
    elif len(sys.argv) > 1 and sys.argv[1] == "--demo":
        # Demostración n=3, x=2
        tri = generar_triangulo_pascal(3)
        evaluar_por_pasos(tri[3], 2.0)
    else:
        # Ejecutar demo y benchmark por defecto
        tri = generar_triangulo_pascal(7)
        print("Triangulo de Pascal generado para n=0..7:")
        for i, r in enumerate(tri):
            print(f"(n={i}): (x+1)^{i} = {formatear_polinomio(r)}")
        print("\nDemostracion con n=3 y x=2:")
        evaluar_por_pasos(tri[3], 2.0)
        benchmark_n100()
