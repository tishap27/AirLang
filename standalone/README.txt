AirLang Compiler & Virtual Machine - Standalone Distribution
===========================================================

QUICK START:
1. Write your AirLang program and save it as [filename].txt
2. Double-click run_airlang.bat and follow the prompts
   OR
3. Open Command Prompt in this folder and run:
   AirLang.exe [filename].txt

EXAMPLES:
Try these demo programs first:
- example_program.txt     (Basic AirLang syntax)
- flight_plan_demo.txt    (Flight planning example)

USAGE:
Method 1 - Easy (Double-click):
  • Double-click run_airlang.bat
  • Type your filename when prompted (include .txt extension)

Method 2 - Command Line:
  • Open Command Prompt in this folder
  • Run: AirLang.exe your_program.txt

Method 3 - Drag & Drop:
  • Drag your .txt file onto run_airlang.bat

HOW IT WORKS:
1. AirLang.exe compiles your .txt source code to .air bytecode
2. The built-in Virtual Machine executes the bytecode
3. You'll see both the compiled .air file and the program output

CREATING YOUR OWN PROGRAMS:
1. Create a new text file
2. Save it with .txt extension (e.g., my_program.txt)
3. Write AirLang code using the syntax from examples
4. Run using one of the methods above
5. Check the generated .air file to see the bytecode

OUTPUT FILES:
- your_program.txt → Source code (what you write)
- your_program.air → Compiled bytecode (generated automatically)
- Console output → Program results

TROUBLESHOOTING:
- Make sure your .txt file is in the same folder as AirLang.exe
- Check that your program follows AirLang syntax
- Compilation errors will show which line has the problem
- The .air file is created only if compilation succeeds

For more examples and documentation:
https://github.com/tishap27/AirLang

Contact: tishaapatel08@gmail.com