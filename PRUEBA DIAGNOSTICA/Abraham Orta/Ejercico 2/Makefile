CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -std=c11 -D_POSIX_C_SOURCE=199309L
LDFLAGS = -lrt

.PHONY: all clean run run_py

all: Ejercicio2

Ejercicio2: Ejercicio2.c
	$(CC) $(CFLAGS) -o Ejercicio2 Ejercicio2.c $(LDFLAGS)

run: Ejercicio2
	./Ejercicio2

run_py:
	python3 benchmark.py

clean:
	rm -f Ejercicio2
