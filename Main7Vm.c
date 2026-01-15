/*
************************************************************
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
# ECHO "[VIRTUAL MACHINE SCRIPT ................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Main7Vm.c
* Compiler: MS Visual Studio 2022
* Date: August 2025
* Purpose: Main program for Virtual Machine (Step 7)
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

#ifndef VM_H
#include "Step7VM.h"
#endif

/*Function declaration*/
airlang_void displayVMContent(const airlang_strg bytecode_file);

/*
************************************************************
* Main function for Virtual Machine
* Parameters:
*   argc / argv = Parameters from command prompt
* Return value:
*   Success operation.
************************************************************
*/
airlang_intg main7Vm(airlang_intg argc, airlang_strg* argv) {
    if (argc >= 2) {
        airlang_strg bytecode_file = argv[2];
        printf("Reading bytecode file: %s\n", bytecode_file);


        displayVMContent(bytecode_file);

        // Run the Virtual Machine
        airlang_intg result = runVirtualMachine(bytecode_file);

        if (result) {
            printf("\nAIRLANG Virtual Machine executed successfully!\n");
            return EXIT_SUCCESS;
        }
        else {
            printf("\nAIRLANG Virtual Machine execution failed!\n");
            return EXIT_FAILURE;
        }
    }
    else {
        printf("Usage: %s <option> <bytecode_file>\n", argv[0]);
        printf("Example: %s 7 simple_flight.txt.air\n", argv[0]);
        return EXIT_FAILURE;
    }
}
airlang_void displayVMContent(const airlang_strg bytecode_file) {
    printf("\nPrinting bytecode file parameters:\n\n");
    printf("Bytecode file: %s\n", bytecode_file);
    printf("\n");
    printf("Starting Virtual Machine execution...\n");
    printf("\n");
}