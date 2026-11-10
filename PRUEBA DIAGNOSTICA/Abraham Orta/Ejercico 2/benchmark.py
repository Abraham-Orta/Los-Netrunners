"""
Ejercicio 2 - Benchmark del Triángulo de Pascal (versión Python)

Genera fila_pascal(100) usando el mismo enfoque iterativo que la versión en C,
repetido 1000 veces. Comparar el tiempo transcurrido con la implementación C.
"""
import time


def fila_pascal(n: int) -> list[int]:
    """Retorna la n-ésima fila del triángulo de Pascal (n+1 elementos)."""
    fila = [1]
    for i in range(1, n + 1):
        fila_nueva = [1] * (i + 1)
        for j in range(1, i):
            # Cada elemento es la suma de los dos de arriba
            fila_nueva[j] = fila[j - 1] + fila[j]
        fila = fila_nueva
    return fila


def imprimir_polinomio(n: int, coeficientes: list[int]) -> None:
    """Muestra la forma expandida de (x+1)^n."""
    if n == 0:
        print("(x+1)^0 = 1")
        return

    terminos = []
    for i, coef in enumerate(coeficientes):
        potencia = n - i
        if potencia == 0:
            terminos.append(str(coef))
        elif potencia == 1:
            terminos.append("x" if coef == 1 else f"{coef}x")
        else:
            terminos.append(f"x^{potencia}" if coef == 1 else f"{coef}x^{potencia}")

    print(f"(x+1)^{n} = " + " + ".join(terminos))


def evaluar_polinomio(n: int, coeficientes: list[int], valor_x: int) -> int:
    """Evalúa f(x) = (x+1)^n paso a paso usando el método de Horner."""
    print(f"\nEvaluación de f({valor_x}) = (x+1)^{n} paso a paso (Horner):")
    if n == 0:
        print(f"  f({valor_x}) = 1")
        return 1

    acumulador = coeficientes[0]
    print(f"  paso 0 : acumulador = {acumulador}")
    for i in range(1, n + 1):
        acumulador = acumulador * valor_x + coeficientes[i]
        print(f"  paso {i} : acumulador = acumulador * {valor_x} + {coeficientes[i]} = {acumulador}")

    print(f"  f({valor_x}) = {acumulador}")
    return acumulador


if __name__ == "__main__":
    print("=== Triángulo de Pascal y Polinomio (x+1)^n  [Python] ===\n")

    # Mostrar filas 0 a 7
    print("--- Filas del triángulo de Pascal ---")
    for n in range(8):
        fila = fila_pascal(n)
        print(f"(n={n}): {' '.join(map(str, fila))}")

    print("\n--- Expansiones del polinomio ---")
    for n in range(8):
        imprimir_polinomio(n, fila_pascal(n))

    # Evaluación paso a paso de ejemplo
    print("\n--- Evaluación paso a paso ---")
    grado, valor_x = 4, 2
    fila = fila_pascal(grado)
    imprimir_polinomio(grado, fila)
    resultado = evaluar_polinomio(grado, fila, valor_x)
    verificacion = (valor_x + 1) ** grado
    estado = "[OK]" if resultado == verificacion else "[ERROR]"
    print(f"  Verificación directa (x+1)^{grado} = {verificacion} {estado}")

    # Benchmark: n=100, 1000 repeticiones
    cantidad_repeticiones = 1000
    print(f"\n=== Benchmark: generar fila_pascal(100) × {cantidad_repeticiones} repeticiones ===")
    inicio = time.perf_counter()
    for _ in range(cantidad_repeticiones):
        fila_pascal(100)
    tiempo_ms = (time.perf_counter() - inicio) * 1000

    print(f"Python - tiempo total   : {tiempo_ms:.4f} ms")
    print(f"Python - tiempo promedio: {tiempo_ms / cantidad_repeticiones:.6f} ms/rep")
    print("\nComparar con la implementación C (ejecutar ./Ejercicio2).")
