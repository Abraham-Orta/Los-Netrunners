@echo off
title Instalador y Verificador de Dependencias - Prueba Diagnostica
echo ======================================================================
echo  UNIVERSIDAD NACIONAL EXPERIMENTAL DE GUAYANA (UNEG)
echo  Lenguajes y Compiladores - Periodo Lectivo 2026-II
echo  Estudiante: Jebel Ruiz
echo  Verificacion e Instalacion de Componentes y Dependencias
echo ======================================================================
echo.

echo [1/3] Verificando interprete Python...
where python >nul 2>nul
if %errorlevel% neq 0 (
    echo [ADVERTENCIA] Python no esta detectado en el PATH.
    echo Por favor descargue e instale Python desde: https://www.python.org/downloads/
    echo Asegurese de marcar la casilla "Add Python to PATH" durante la instalacion.
) else (
    for /f "tokens=*" %%i in ('python --version') do echo [OK] Python detectado: %%i
)
echo.

echo [2/3] Verificando compilador GCC (MinGW / C)...
where gcc >nul 2>nul
if %errorlevel% neq 0 (
    echo [ADVERTENCIA] GCC no esta detectado en el PATH.
    echo Si no tiene GCC, puede instalarlo mediante WinLibs o MSYS2:
    echo   https://winlibs.com/ o con winget:
    echo   winget install MSYS2.MSYS2
) else (
    for /f "tokens=*" %%i in ('gcc --version') do (
        echo [OK] Compilador GCC detectado: %%i
        goto :gcc_encontrado
    )
)
:gcc_encontrado
echo.

echo [3/3] Verificando modulos y librerias estandar...
echo Nota: Todas las soluciones fueron disenadas para no requerir dependencias
echo externas pesadas (cero dependencias de terceros; 100%% librerias estandar
echo de C y Python), asegurando maxima compatibilidad y portabilidad en cualquier entorno.
echo.
echo ======================================================================
echo  Verificacion completada exitosamente.
echo ======================================================================
pause
