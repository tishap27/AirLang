@echo off
:: AirLang - Universal Command Handler
:: Usage: airlang install  OR  airlang run file.air

set COMMAND=%1

:: If no command, show help
if "%COMMAND%"=="" goto ShowHelp

:: Handle 'install' command
if /i "%COMMAND%"=="install" goto Install

:: Handle 'help' command
if /i "%COMMAND%"=="help" goto ShowHelp
if /i "%COMMAND%"=="--help" goto ShowHelp
if /i "%COMMAND%"=="-h" goto ShowHelp

:: Handle 'version' command
if /i "%COMMAND%"=="version" goto ShowVersion
if /i "%COMMAND%"=="--version" goto ShowVersion
if /i "%COMMAND%"=="-v" goto ShowVersion

:: Otherwise, try to run as a file
goto RunFile

::====================
:: INSTALL COMMAND
::====================
:Install
echo.
echo ======================================
echo    AirLang Installation Script
echo ======================================
echo.

:: Check if running as administrator
>nul 2>&1 "%SYSTEMROOT%\system32\cacls.exe" "%SYSTEMROOT%\system32\config\system"
if '%errorlevel%' NEQ '0' (
    echo Requesting administrator privileges...
    echo.
    echo Set UAC = CreateObject^("Shell.Application"^) > "%temp%\getadmin.vbs"
    echo UAC.ShellExecute "%~s0", "install", "", "runas", 1 >> "%temp%\getadmin.vbs"
    "%temp%\getadmin.vbs"
    del "%temp%\getadmin.vbs"
    exit /B
)

echo Installing AirLang...
echo.

:: Get the current directory
set INSTALL_DIR=%~dp0

:: Check if airlang.exe exists
if exist "%INSTALL_DIR%standalone\AirLang.exe" (
    set EXE_PATH=%INSTALL_DIR%standalone\AirLang.exe
    echo [OK] Found: standalone\AirLang.exe
)else if exist "%INSTALL_DIR%x64\Debug\airlang.exe" (
    set EXE_PATH=%INSTALL_DIR%x64\Debug\airlang.exe
    echo [OK] Found: x64\Debug\airlang.exe
) else if exist "%INSTALL_DIR%x64\Release\airlang.exe" (
    set EXE_PATH=%INSTALL_DIR%x64\Release\airlang.exe
    echo [OK] Found: x64\Release\airlang.exe
) else if exist "%INSTALL_DIR%Debug\airlang.exe" (
    set EXE_PATH=%INSTALL_DIR%Debug\airlang.exe
    echo [OK] Found: Debug\airlang.exe
) else if exist "%INSTALL_DIR%Release\airlang.exe" (
    set EXE_PATH=%INSTALL_DIR%Release\airlang.exe
    echo [OK] Found: Release\airlang.exe
) else if exist "%INSTALL_DIR%airlang.exe" (
    set EXE_PATH=%INSTALL_DIR%airlang.exe
    echo [OK] Found: airlang.exe
) else (
    echo [ERROR] Could not find airlang.exe
    echo Please build the project first in Visual Studio.
    echo.
    pause
    exit /b 1
)

echo.
echo Adding AirLang to your PATH...

:: Get the directory containing the exe
for %%i in ("%EXE_PATH%") do set EXE_DIR=%%~dpi
set EXE_DIR=%EXE_DIR:~0,-1%

:: Also add the current directory (for airlang.cmd)
set SCRIPT_DIR=%~dp0
set SCRIPT_DIR=%SCRIPT_DIR:~0,-1%

:: Check if already in PATH
echo %PATH% | find /i "%EXE_DIR%" >nul
if %errorLevel% equ 0 (
    echo [OK] AirLang executable already in PATH
) else (
    :: Add to PATH for current user
    for /f "skip=2 tokens=3*" %%a in ('reg query HKCU\Environment /v PATH 2^>nul') do set "CURRENT_PATH=%%b"
    if not defined CURRENT_PATH set "CURRENT_PATH="
    
    setx PATH "%CURRENT_PATH%;%EXE_DIR%" >nul 2>&1
    
    if %errorLevel% equ 0 (
        echo [OK] Successfully added executable to PATH!
    ) else (
        echo [ERROR] Failed to add to PATH
        pause
        exit /b 1
    )
)

:: Also add script directory if different
if /i not "%EXE_DIR%"=="%SCRIPT_DIR%" (
    echo %PATH% | find /i "%SCRIPT_DIR%" >nul
    if %errorLevel% neq 0 (
        for /f "skip=2 tokens=3*" %%a in ('reg query HKCU\Environment /v PATH 2^>nul') do set "CURRENT_PATH=%%b"
        setx PATH "%CURRENT_PATH%;%SCRIPT_DIR%" >nul 2>&1
        if %errorLevel% equ 0 (
            echo [OK] Successfully added script directory to PATH!
        )
    )
)

echo.
echo ======================================
echo  Installation Complete! Success!
echo ======================================
echo.
echo AirLang is now installed!
echo.
echo Next steps:
echo   1. Close this window
echo   2. Open a NEW Command Prompt
echo   3. Type: airlang help
echo.
echo Example: airlang run myfile.air
echo.
timeout /t 5
exit /b 0

::====================
:: SHOW HELP
::====================
:ShowHelp
echo.
echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
echo @@                             @@
echo @@            __^|__            @@
echo @@-------@--o--^(_^)--o--@-------@@
echo @@                             @@
echo @@      A I R L A N G          @@
echo @@                             @@
echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
echo.
echo AirLang - Aviation Domain-Specific Language
echo.
echo Usage:
echo   airlang install              - Install AirLang to your system
echo   airlang run ^<file.air^>       - Run an AirLang program
echo   airlang ^<file.air^>           - Run an AirLang program
echo   airlang help                 - Show this help message
echo   airlang version              - Show version information
echo.
echo Examples:
echo   airlang install
echo   airlang run flight_plan.air
echo   airlang dispatch.air
echo.
exit /b 0

::====================
:: SHOW VERSION
::====================
:ShowVersion
echo AirLang v1.0.0
echo Author: Tisha Patel
echo Contact: tishaapatel08@gmail.com
exit /b 0

::====================
:: RUN FILE
::====================
:RunFile
set SCRIPT_DIR=%~dp0

:: Try to find airlang.exe
if exist "%SCRIPT_DIR%standalone\AirLang.exe" (
    "%SCRIPT_DIR%standalone\AirLang.exe" %*
    exit /b %errorlevel%
) else if exist "%INSTALL_DIR%standalone\airLang.exe" (
    set EXE_PATH=%INSTALL_DIR%standalone\airlang.exe
    echo [OK] Found: standalone\airlang.exe
)else if exist "%SCRIPT_DIR%x64\Debug\airlang.exe" (
    "%SCRIPT_DIR%x64\Debug\airlang.exe" %*
    exit /b %errorlevel%
) else if exist "%SCRIPT_DIR%x64\Release\airlang.exe" (
    "%SCRIPT_DIR%x64\Release\airlang.exe" %*
    exit /b %errorlevel%
) else if exist "%SCRIPT_DIR%Debug\airlang.exe" (
    "%SCRIPT_DIR%Debug\airlang.exe" %*
    exit /b %errorlevel%
) else if exist "%SCRIPT_DIR%Release\airlang.exe" (
    "%SCRIPT_DIR%Release\airlang.exe" %*
    exit /b %errorlevel%
) else if exist "%SCRIPT_DIR%airlang.exe" (
    "%SCRIPT_DIR%airlang.exe" %*
    exit /b %errorlevel%
) else (
    echo ERROR: AirLang is not installed or executable not found.
    echo Run: airlang install
    exit /b 1
)