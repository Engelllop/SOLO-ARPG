@echo off
chcp 65001 >nul
echo ============================================
echo   SOLO-ARPG - Setup para Windows
echo ============================================
echo.

REM Verificar que estamos en el directorio correcto
if not exist "SOLO.uproject" (
    echo [ERROR] No se encontro SOLO.uproject
    echo Asegurate de ejecutar este script desde la raiz del proyecto.
    pause
    exit /b 1
)

echo [1/6] Verificando estructura...
if not exist "Content\SOLO" (
    echo   Creando estructura de Content/...
    python Tools\setup_content.py
) else (
    echo   Content/ ya existe.
)

echo.
echo [2/6] Verificando UE5.7...
set "UE5_PATH=C:\Program Files\Epic Games\UE_5.7"
if not exist "%UE5_PATH%" (
    echo [ADVERTENCIA] UE5.7 no encontrado en %UE5_PATH%
    echo.
    echo Por favor instala Unreal Engine 5.7 desde Epic Games Launcher.
    echo.
    echo Una vez instalado, ejecuta este script nuevamente.
    pause
    exit /b 1
)
echo   UE5.7 encontrado.

echo.
echo [3/6] Generando archivos de Visual Studio...
set "UBT=%UE5_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe"
if exist "%UBT%" (
    "%UBT%" -projectfiles -project="%CD%\SOLO.uproject" -game -engine -progress
    if %ERRORLEVEL% neq 0 (
        echo [ERROR] Fallo al generar archivos de Visual Studio.
        pause
        exit /b 1
    )
) else (
    echo [ERROR] No se encontro UnrealBuildTool.exe
    echo Ruta esperada: %UBT%
    pause
    exit /b 1
)

echo.
echo [4/6] Compilando proyecto...
echo   Abriendo Visual Studio 2022...
echo   Por favor compila manualmente: Build -^> Build Solution (Ctrl+Shift+B)
echo   Asegurate de usar configuracion: Development Editor
echo.
start SOLO.sln

echo.
echo [5/6] Creando guias de Content...
python Tools\setup_content.py

echo.
echo ============================================
echo   Setup inicial completado!
echo ============================================
echo.
echo Siguientes pasos manuales:
echo   1. Compilar en Visual Studio (paso 4)
echo   2. Doble click en SOLO.uproject
echo   3. Seguir guias en Content/SOLO/*/
echo   4. Crear Input Actions, Blueprints y Mapa
echo.
echo Documentacion:
echo   - Docs/UE5_SETUP_GUIDE.md
echo   - Content/SOLO/Input/INPUT_SETUP.txt
echo   - Content/SOLO/Maps/MAP_SETUP.txt
echo.
pause
