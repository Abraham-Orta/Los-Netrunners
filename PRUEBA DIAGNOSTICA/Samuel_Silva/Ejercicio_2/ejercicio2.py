import time
import os

# Genera los coeficientes del binomio con el triangulo de Pascal
def calcular_pascal(n):
    fila = [1]
    for _ in range(n):
        nueva = [1]
        for j in range(len(fila) - 1):
            nueva.append(fila[j] + fila[j + 1])
        nueva.append(1)
        fila = nueva
    return fila

# Formatea el polinomio como x^4 + 4x^3 + 6x^2 + 4x + 1
def formatear_polinomio(coef):
    n = len(coef) - 1
    terminos = []
    for i, c in enumerate(coef):
        pot = n - i
        if pot > 1:
            terminos.append(f"x^{pot}" if c == 1 else f"{c}x^{pot}")
        elif pot == 1:
            terminos.append("x" if c == 1 else f"{c}x")
        else:
            terminos.append(str(c))
    return " + ".join(terminos)

# Muestra la evaluacion paso a paso para un valor de x
def evaluar_paso_a_paso(coef, x):
    n = len(coef) - 1
    print(f"\nb) Calculando f({x}) = ({x} + 1)^{n} paso a paso:")
    total = 0
    for i, c in enumerate(coef):
        pot = n - i
        valor = c * (x ** pot)
        total += valor
        if pot > 1:
            if c == 1:
                print(f"   Paso {i+1}: Termino x^{pot} -> ({x})^{pot} = {valor}")
            else:
                print(f"   Paso {i+1}: Termino {c}x^{pot} -> {c} * ({x})^{pot} = {valor}")
        elif pot == 1:
            if c == 1:
                print(f"   Paso {i+1}: Termino x -> ({x}) = {valor}")
            else:
                print(f"   Paso {i+1}: Termino {c}x -> {c} * ({x}) = {valor}")
        else:
            print(f"   Paso {i+1}: Termino independiente -> {valor}")
    print(f"   Resultado final f({x}) = {total}\n")

# Guarda el tiempo en tiempos.txt preservando la medicion de C si ya existe
def guardar_tiempo_py(duracion):
    carpeta = os.path.dirname(os.path.abspath(__file__))
    ruta = os.path.join(carpeta, "tiempos.txt")

    linea_c = ""
    if os.path.exists(ruta):
        with open(ruta, "r", encoding="utf-8") as f:
            for l in f:
                if "en C:" in l:
                    linea_c = l.strip()

    linea_py = f"Tiempo de ejecucion para generar n=100 en Python: {duracion:.8f} segundos"

    with open(ruta, "w", encoding="utf-8") as f:
        if linea_c:
            f.write(linea_c + "\n")
        f.write(linea_py + "\n")

    print("[OK] Tiempo de ejecucion para n=100 guardado en 'tiempos.txt'.")

def main():
    print("Expansion de (x + 1)^n con Triangulo de Pascal")
    try:
        n = int(input("Ingrese n (entero no negativo): "))
        x = int(input("Ingrese x (entero): "))
    except ValueError:
        print("Error: Ingrese valores enteros validos.")
        return

    coef = calcular_pascal(n)
    print(f"\nDEMOSTRACION PARA n={n}")
    print(f"a) (x + 1)^{n} = {formatear_polinomio(coef)}")
    evaluar_paso_a_paso(coef, x)

    # Medicion de tiempo para n=100
    inicio = time.perf_counter()
    _ = calcular_pascal(100)
    fin = time.perf_counter()

    guardar_tiempo_py(fin - inicio)

if __name__ == "__main__":
    main()
