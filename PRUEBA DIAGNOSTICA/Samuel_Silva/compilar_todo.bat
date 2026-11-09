@echo off
chcp 65001 > nul
cd /d "%~dp0"

echo ========================================================
echo   COMPILACION DE EJERCICIOS - LENGUAJES Y COMPILADORES
echo   Alumno: Samuel Silva
echo ========================================================
echo.

where gcc >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo [ERROR] GCC no fue encontrado en el PATH.
    echo Asegurate de tener instalado MinGW o MSYS2.
    pause
    exit /b 1
)

echo [1/2] Compilando Ejercicio 2 (Triangulo de Pascal y Polinomio)...
gcc Ejercicio_2\ejercicio2.c -o Ejercicio_2\ejercicio2.exe -lm
if %ERRORLEVEL% equ 0 (
    echo       -^> Ejercicio_2\ejercicio2.exe generado exitosamente.
) else (
    echo       -^> [ERROR] Fallo al compilar Ejercicio 2.
)

echo.
echo [2/2] Compilando Ejercicio 3 (Traductor de Palabras Reservadas)...
gcc Ejercicio_3\ejercicio3.c -o Ejercicio_3\ejercicio3.exe
if %ERRORLEVEL% equ 0 (
    echo       -^> Ejercicio_3\ejercicio3.exe generado exitosamente.
) else (
    echo       -^> [ERROR] Fallo al compilar Ejercicio 3.
)

echo.
echo ========================================================
echo   Proceso finalizado. Ejecutables listos para usar.
echo ========================================================
echo.
pause
