CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -std=c11 -D_POSIX_C_SOURCE=199309L
LDFLAGS = -lrt

.PHONY: all clean run

all: Ejercicio1

Ejercicio1: Ejercicio1.c
	$(CC) $(CFLAGS) -o Ejercicio1 Ejercicio1.c $(LDFLAGS)

run: Ejercicio1
	./Ejercicio1

clean:
	rm -f Ejercicio1
