#Ejercicio Nro.1: Tablero de Ajedrez  (Movimientos del Caballo)
#Estudiante: Alejandro Gonzalez
#Lenguaje: Python

#++++++++++Librerías++++++++++
import time 

#+++++++++++Funciones++++++++++

#Parte 1: Encargadas de validar los movimientos del caballo

#Función para verificar si la posición es válida dentro del tablero 
def posicion_valida(posicion):

    # Verificar que la posición tenga el formato correcto (columna + fila).
    if len(posicion) != 2:
        return False

    # Verificar que la columna y la fila estén dentro de los límites del tablero.
    columna = posicion[0]
    fila = posicion[1]

    return columna in "abcdefgh" and fila in "12345678"

#Función para validar los movimientos del caballo
def validar_movimientos(posicion_inicial, movimientos):

    posicion_actual = posicion_inicial

    # Recorrer cada movimiento y verificar su validez
    for numero, movimiento in enumerate(movimientos, start=1):

        # Comprobar que el movimiento tenga el formato K + columna + fila.
        if len(movimiento) != 3:
            return False, numero, "formato de movimiento incorrecto"

        # Comprobar que la letra inicial sea 'K'
        if movimiento[0].upper() != "K":
            return False, numero, f"la letra {movimiento[0]} no pertenece a un caballo"
        
        destino = movimiento[1:3].lower()

        # Comprobar que el destino este dentro del tablero.
        if not posicion_valida(destino):
            return False, numero, "la posicion de destino esta fuera del tablero"

    #Movimiento correcto, ahora verificamos si es un movimiento válido del caballo

        # Convertir las columnas y filas a valores numericos.
        columna_actual = ord(posicion_actual[0]) - ord("a")
        fila_actual = int(posicion_actual[1])

        columna_destino = ord(destino[0]) - ord("a")
        fila_destino = int(destino[1])

        # Calcular cuanto se desplaza el caballo.
        diferencia_columna = abs(columna_destino - columna_actual)
        diferencia_fila = abs(fila_destino - fila_actual)

        # Comprobar el movimiento en forma de L.
        movimiento_correcto = (
            (diferencia_columna == 2 and diferencia_fila == 1) or
            (diferencia_columna == 1 and diferencia_fila == 2)
        )

        if not movimiento_correcto:
            return False, numero, "Movimiento de caballo no valido"

        # Actualizar la posicion despues de un movimiento correcto.
        posicion_actual = destino

    return True, 0, "Movimiento CORRECTO"


#Parte 2: Encargada de medir el tiempo de ejecucion  

def medir_tiempo(posicion_inicial, movimientos):

    inicio = time.perf_counter()

    for _ in range(10):
        validar_movimientos(posicion_inicial, movimientos)

    fin = time.perf_counter()

    tiempo_total = fin - inicio

    return tiempo_total


#+++++++++++Main++++++++++++

def main():

    print("\n\t\t===== VALIDACION DE MOVIMIENTOS DEL CABALLO =====\n")

    posicion = input("+ Ingrese la posicion inicial del caballo: ").strip().lower()

    while(posicion_valida(posicion) == False):
        print("\n- Error: posicion inicial no valida.\n")
        posicion = input("+ Ingrese la posicion inicial del caballo: ").strip().lower()

    cantidad = int(input("\n+ Ingrese la cantidad de movimientos: "))

    while cantidad < 1:
        print("\n- Error: la cantidad no puede ser negativa ni 0.\n")
        cantidad = int(input("\n+ Ingrese la cantidad de movimientos: "))

    entrada = input("\n+ Ingrese los movimientos separados por espacios (ej: Ke4 Kd2 Kc3): ")

    movimientos = entrada.split()

    if len(movimientos) != cantidad:
        print("\n- Error: La cantidad de movimientos no coincide.")
        print("\n- Cantidad indicada:", cantidad)
        print("\n- Cantidad recibida:", len(movimientos))
        return

    # Validar la secuencia.
    correcto, numero, mensaje = validar_movimientos(posicion, movimientos)

    if correcto:
        print("\nResultado: movimiento CORRECTO.")
    else:
        print("\nResultado: INCORRECTO.")
        print("\nFalla en el movimiento:", numero)
        print("Motivo:", mensaje)

    # Medir el tiempo de las 10 verificaciones.
    tiempo = medir_tiempo(posicion, movimientos)

    print("\nTiempo de 10 verificaciones:", tiempo, "segundos")

    # Guardar los resultados en un archivo.
    with open("Resultado_Ejercicio1.txt", "a", encoding="utf-8") as archivo:

        archivo.write("\n\nRESULTADOS DEL EJERCICIO 1\n")
        archivo.write("==========================\n")
        archivo.write(f"Posicion inicial: {posicion}\n")
        archivo.write(f"Cantidad de movimientos: {cantidad}\n")

        if correcto:
            archivo.write("Resultado: movimiento CORRECTO\n")
        else:
            archivo.write("Resultado: INCORRECTO\n")
            archivo.write(f"Falla en el movimiento: {numero}\n")
            archivo.write(f"Motivo: {mensaje}\n")

        archivo.write(f"Tiempo de 10 verificaciones: {tiempo:.10f} segundos\n")

    print("\nResultados guardados en Resultado_Ejercicio1.txt")


if __name__ == "__main__":
    main()
