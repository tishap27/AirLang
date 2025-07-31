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
airlang_void displayGeneratedCode(const Generator* cg);

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

        // Initialize code generator
        Generator cg;
        airlang_char output_filename[256];
        snprintf(output_filename, sizeof(output_filename), "%s.air", source);

        initGenerator(&cg, output_filename);

        // Generate code
        generateCode(content, &cg);

        // Display generated instructions
        displayGeneratedCode(&cg);

        // Write binary file
        writeBinaryFile(&cg);

        printf("\nCode generation completed successfully!\n");
        printf("Output file: %s\n", output_filename);

    }
    else {
        printf("Usage: %s <option> <input_file>\n", argv[0]);
        printf("Example: %s 6 simple_flight.txt\n", argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

airlang_void displayGeneratedCode(const Generator* cg) {
    printf("\n=== GENERATED INSTRUCTIONS ===\n");

    for (airlang_intg i = 0; i < cg->instruction_count; i++) {
        const Instruction* inst = &cg->instructions[i];
        printf("%3d: ", i);

        switch (inst->opCode) {
        case OP_LOAD_NUM:
            printf("LOAD_NUM %.2f\n", inst->operand.num_operand);
            break;
        case OP_LOAD_STR:
            printf("LOAD_STR \"%s\n", inst->operand.str_operand);
            break;
        case OP_STORE_VAR:
            printf("STORE_VAR %s\n", inst->operand.str_operand);
            break;
        case OP_PRINT:
            printf("PRINT\n");
            break;
        case OP_HALT:
            printf("HALT\n");
            break;
        default:
            printf("UNKNOWN\n");
            break;
        }
    }
    printf("===============================\n");
}

airlang_void displayGeneratorContent(const char* content) {
    printf("\nPrinting input buffer parameters:\n\n");
    printf("%s\n", content);
    printf("\n");
    printf("Running the source file...\n");
    printf("\n");
}