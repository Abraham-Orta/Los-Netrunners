CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -std=c11

.PHONY: all clean run

all: Ejercicio3

Ejercicio3: Ejercicio3.c
	$(CC) $(CFLAGS) -o Ejercicio3 Ejercicio3.c

# Demo: analiza el programa de ejemplo para mostrar la traducción
run: Ejercicio3
	./Ejercicio3 programa_ejemplo.c

clean:
	rm -f Ejercicio3
