#Parte 2 del Ejercicio Nro.2: Implementación en Python

#++++++++++Librerías++++++++++
import time

#++++++++++Funciones++++++++++

#FunciOn para generar los coeficientes del polinomio usando a Pascal
def generarCoeficientes(n):
    #Inicializamos
    coeficientes = [0] * (n + 1)

    for i in range(n + 1):
        coeficientes[i] = 1
    #Recorremos los elementos de la fila y sumamos los valores de la fila anterior
        for j in range(i - 1, 0, -1):
            coeficientes[j] += coeficientes[j - 1]

    return coeficientes

#FunciOn para mostrar el polinomio generado
def mostrar_polinomio(coeficientes, n):
    print("\n+ Polinomio generado:")

    #Construimos el polinomio como una cadena de texto
    terminos = []

    for i in range(n + 1):
        exponente = n - i
        coeficiente = coeficientes[i]

        if exponente == 0:
            termino = str(coeficiente)
        elif exponente == 1:
            termino = "x" if coeficiente == 1 else f"{coeficiente}x"
        else:
            termino = f"x^{exponente}" if coeficiente == 1 else f"{coeficiente}x^{exponente}"

        terminos.append(termino)

    print(" + ".join(terminos))

#FunciOn para evaluar el polinomio en un valor dado de x
def evaluar_polinomio(coeficientes, n, x):
    print("\n+ Evaluacion paso a paso:")

    suma = 0

    for i in range(n + 1):
        exponente = n - i
        coeficiente = coeficientes[i]

        potencia = pow(x, exponente)
        termino = coeficiente * potencia

        print(
            f"- Termino {i + 1}: {coeficiente} * "
            f"{x:.2f}^{exponente} = {termino:.2f}"
        )

        suma += termino

    print(f"\n+ Resultado final: {suma:.2f}")

#Funcion para medir el tiempo de ejecucion de la generacion de coeficientes
def medir_tiempo_generacion(n, repeticiones):
    inicio = time.perf_counter()

    for _ in range(repeticiones):
        coeficientes = generarCoeficientes(n)

    fin = time.perf_counter()

    return (fin - inicio) / repeticiones

#++++++++FunciOn principal+++++++++++++
def main():
    
    print("\n\t\tCalculo del Polinomio (x+1)^n (Python)\n\n")


    n = int(input("+ Ingrese el valor de N (Valor del exponente): "))

    while n < 0:
        print("\nEl valor de n debe ser no negativo.\n")
        n = int(input("+ Ingrese el valor de N (Valor del exponente): "))
    x = float(input("+ Ingrese el valor de X (Valor de la variable): "))

    coeficientes = generarCoeficientes(n)

    mostrar_polinomio(coeficientes, n)
    evaluar_polinomio(coeficientes, n, x)

    print("\n--- Medicion de rendimiento ---")

    repeticiones = 1000

    tiempo = medir_tiempo_generacion(n, repeticiones)

    print(f"Valor de n: {n}")
    print(f"Repeticiones: {repeticiones}")
    print(f"Tiempo promedio: {tiempo:.10f} segundos")


if __name__ == "__main__":
    main()
