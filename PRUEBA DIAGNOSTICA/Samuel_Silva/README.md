# Universidad Nacional Experimental de Guayana (UNEG)
## Coordinación de Ingeniería en Informática
### Asignatura: Lenguajes y Compiladores (Periodo 2026-II)
**Profesor:** Msc. Félix Márquez  
**Estudiante:** Samuel Silva  

---

## 📹 Enlace al Video de Defensa (5% de la evaluación)
* **URL de Google Drive:** https://drive.google.com/file/d/1l7lXrhrYh-Vj8AkSrgm4Dv5ZS5qxdRma/view?usp=sharing
* **Duración máxima:** 5 minutos.
* **Presentación formal incluida:** Grupo, Eslogan, Nombre y Sección.

---

## 📁 Estructura del Repositorio

```text
PRUEBA DIAGNOSTICA/
└── Samuel_Silva/
    ├── README.md                      <-- Documentación y enlace al video
    ├── compilar_todo.bat              <-- Script para compilar programas en C
    ├── Ejercicio_1/                   <-- Validador de caballo en ajedrez (Python)
    │   ├── ejercicio1.py
    │   └── tiempos.txt
    ├── Ejercicio_2/                   <-- Polinomio (x+1)^n y Pascal (C y Python)
    │   ├── ejercicio2.c
    │   ├── ejercicio2.exe
    │   ├── ejercicio2.py
    │   └── tiempos.txt
    └── Ejercicio_3/                   <-- Traductor léxico de C a Español
        ├── ejercicio3.c
        ├── ejercicio3.exe
        ├── ejercicio3.py
        └── programa_prueba.c
```

---

## ⚙️ Requisitos y Compilación

### Requisitos previos:
* **GCC** (MinGW / MSYS2) agregado al PATH del sistema.
* **Python 3.10+** instalado.

### Compilación automática (Windows):
Ejecutar el script desde la raíz de la carpeta `Samuel_Silva`:
```cmd
compilar_todo.bat
```

---

## 🚀 Guía de Ejecución

### Ejercicio 1 — Validador de movimientos de caballo en ajedrez
Valida movimientos legales de caballo en un tablero $8 \times 8$. Se detiene en el primer movimiento incorrecto o al detectar piezas inválidas (ej. letra diferente a `K`). Incluye la medición de tiempo al verificar 10 veces la secuencia oficial de 500 movimientos partiendo de `a2`.

* **Ejecución en Python:**
  ```cmd
  cd Ejercicio_1
  python ejercicio1.py
  ```
* **Archivo generado:** `tiempos.txt` guarda la medición de tiempo para 10 repeticiones de los 500 movimientos.

---

### Ejercicio 2 — Polinomio $(x+1)^n$ mediante Triángulo de Pascal
Genera coeficientes usando memoria dinámica (`malloc`/`free` en C, listas dinámicas en Python), formatea el polinomio omitiendo coeficientes unitarios innecesarios (ej. $x^3$ en vez de $1x^3$), realiza el cálculo paso a paso para un valor $x$ dado, y compara el tiempo exacto para $n=100$ en ambos lenguajes.

* **Ejecución en C:**
  ```cmd
  cd Ejercicio_2
  .\ejercicio2.exe
  ```
* **Ejecución en Python:**
  ```cmd
  cd Ejercicio_2
  python ejercicio2.py
  ```
* **Archivo generado:** `tiempos.txt` guarda la comparativa de ejecución para $n=100$ entre C y Python.

---

### Ejercicio 3 — Traductor de palabras reservadas de C a Español
Carga un programa en lenguaje C en memoria dinámica (`malloc`) y traduce las palabras clave del estándar a su equivalente en español, respetando strings, caracteres y comentarios de bloque/línea.

* **Ejecución en C:**
  ```cmd
  cd Ejercicio_3
  .\ejercicio3.exe programa_prueba.c
  ```
* **Ejecución en Python:**
  ```cmd
  cd Ejercicio_3
  python ejercicio3.py programa_prueba.c
  ```

---

## 🧠 Aspectos Técnicos Destacados
1. **Memoria Dinámica:** En C se asigna y libera memoria explícitamente en el Heap (`malloc` y `free`), evitando memory leaks. En Python, las listas manejan dynamic allocation de fondo.
2. **Precisión de Tiempos:** En Windows se implementó `QueryPerformanceCounter` para medir en microsegundos reales sin errores de baja resolución.
3. **Análisis Léxico:** Reconocimiento de tokens y preservación íntegra de cadenas y comentarios.
