@echo off
REM Setup the Visual Studio environment (update the path as needed)
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

echo --------------------------------------------
echo -      ALGONQUIN COLLEGE - COM - 25S       -
echo --------------------------------------------
echo -                                          -
echo -    ====                                  -
echo -   =                                      -
echo -  =       ==   = =  ==  = =   =   ==  ==  -
echo -   =     =  = = = = = = = =   ==  =  ==   -
echo -    ====  ==  = = = =   = === === =   ==  -
echo -                                          -
echo --------------------------------------------
echo -  [A4: Parser - By: Tisha Patel]          -
echo --------------------------------------------

set "arg=%1"
if "%arg%"=="" set "arg=CODED.txt"

REM Compile the code with cl.exe (edit source files list as needed)
cl /Fe:x64\Debug\airlang.exe compilers.c main1coder.c step1coder.c main2reader.c step2reader.c main3scanner.c step3scanner.c main4parser.c step4parser.c

cd /d "%~dp0x64\Debug"
timeout /t 1 /nobreak >nul
airlang.exe 1 1 FlightPlan.air %arg%
timeout /t 1 /nobreak >nul
airlang.exe 4 %arg% > codedFlightPlan.air 2> err.air
timeout /t 1 /nobreak >nul
dir codedFlightPlan.txt err.air
type codedFlightPlan.txt
