/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2025
* Author: Tisha Patel
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (S25)  |"
# ECHO "=---------------------------------------="
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    "
# ECHO "    @@                             @@    "
# ECHO "    @@            __|__            @@    "
# ECHO "    @@-------@--o--(_)--o--@-------@@    "
# ECHO "    @@                             @@    "
# ECHO "    @@        A I R L A N G        @@    "
# ECHO "    @@                             @@    "
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    "
# ECHO "                                         "
# ECHO "[GENERATOR SCRIPT ......................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Main6Generator.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12, A22, A32.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: Main program for Code Generator (Step 6)
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

#ifndef GENERATOR_H_
#include "Step6Generator.h"
#endif


/*Function declaration*/
airlang_void displayGeneratorContent(const char* content);

/*
************************************************************
* Main function for Code Generator
* Parameters:
*   argc / argv = Parameters from command prompt
* Return value:
*   Success operation.
************************************************************
*/

airlang_intg main6Generator(airlang_intg argc, airlang_strg* argv) {

    if (argc >= 2) {
        airlang_strg source = argv[2];
        printf("Reading source file: %s\n", source);

        // Load and decrypt source file
        airlang_intg size = getSizeOfFile(source);
        airlang_strg content = vigenereMem(source, STR_LANGNAME, DECYPHER);

        if (size <= 0) {
            printf("ERROR: The input file %s is not completely loaded.\n", source);
            return EXIT_FAILURE;
        }
        else {
            displayGeneratorContent(content);
        }

        processFileGeneration(source, content);
    }
    else {
        printf("Usage: %s <option> <input_file>\n", argv[0]);
        printf("Example: %s 6 simple_flight.txt\n", argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}



airlang_void displayGeneratorContent(const char* content) {
    printf("\nPrinting input buffer parameters:\n\n");
    printf("%s\n", content);
    printf("\n");
    printf("Running the source file...\n");
    printf("\n");
}