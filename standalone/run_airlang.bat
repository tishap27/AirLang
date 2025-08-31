@echo off
echo =======================================
echo   AirLang Compiler and Virtual Machine
echo =======================================
echo.

if "%1"=="" (
    echo Usage: run_airlang.bat [program.txt]
    echo.
    echo Examples:
    echo   run_airlang.bat example_program.txt
    echo   run_airlang.bat my_flight_plan.txt
    echo.
    echo Available demo programs:
    echo   - example_program.txt
    echo   - flight_plan_demo.txt
    echo.
    pause
    exit /b 1
)

if not exist "%1" (
    echo Error: File '%1' not found!
    echo.
    echo Make sure your .txt file is in the same folder as this batch file.
    echo Available files:
    dir *.txt /b 2>nul
    echo.
    pause
    exit /b 1
)

echo Compiling and running: %1
echo =======================================
echo.

echo [STEP 1] Compiling AirLang source code...
AirLang.exe "%1"

if errorlevel 1 (
    echo.
    echo *** COMPILATION FAILED ***
    echo Check the error messages above and fix your AirLang code.
) else (
    echo.
    echo [STEP 2] Compilation successful! Bytecode saved as:
    echo %~n1.air
    echo.
    echo [STEP 3] Program execution completed.
    echo =======================================
    echo Check the output above for your program results.
)

echo.
pause