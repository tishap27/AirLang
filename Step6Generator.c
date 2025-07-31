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
* File name: Step6Generator.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file implements the Code Generator
************************************************************
*/

#ifndef CODER_H_
#include "Step6Generator.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>




airlang_void processFileGeneration(const airlang_strg source, const airlang_strg content) {
    // Initialize code generator
    Generator* cg = malloc(sizeof(Generator));
    airlang_char output_filename[256];
    snprintf(output_filename, sizeof(output_filename), "%s.air", source);

    initGenerator(cg, output_filename);

    // Generate code
    generateCode(content, cg);

    // Display generated instructions
    displayGeneratedCode(cg);

    // Write binary file
    writeBinaryFile(cg);

    printf("\nCode generation completed successfully!\n");
    printf("Output file: %s\n", output_filename);


    free(cg);
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



airlang_void initGenerator(Generator* cg, const airlang_strg output_file) {

	cg->instruction_count = 0;
	strcpy(cg->output_filename, output_file);

	printf("Generator initialized forr  output %s\n", STR_LANGNAME);
}

airlang_void generateCode(const airlang_strg source_content, Generator* cg) {

	printf("Starting Code Generation...\n");

    if (isPrintStatement(source_content)) {
        generatePrint(source_content, cg);
    }

	emitInstruction(cg, OP_HALT, 0, "");

	printf("Code Generation complete. Generated %d instructions.\n", cg->instruction_count);
}

airlang_void emitInstruction(Generator* cg, OpCode op, airlang_doub num_val, const airlang_strg str_val) {

	if (cg->instruction_count >= MAX_INSTRUCTIONS) {
		printf("Error: More than Maximum instructions\n");
		return;
	}

	Instruction* inst = &cg->instructions[cg->instruction_count];
	inst->opCode = op; 

	if (str_val && strlen(str_val) > 0) {
		strcpy(inst->operand.str_operand, str_val);

	}
	else {
		inst->operand.num_operand = num_val;

	}

	cg->instruction_count++; 

}


airlang_void generatePrint(const airlang_strg line, Generator* cg) {


    airlang_char* start = strchr(line, '{');
    airlang_char* end = strrchr(line, '}');

    if (start && end && start < end) {
        start++; // Skip {
        airlang_intg len = (airlang_intg)(end - start);
        airlang_char print_content[512];
        strncpy(print_content, start, len);
        print_content[len] = '\0';

        // Trim spaces
        airlang_char* trimmed = print_content;
        while (*trimmed == ' ') trimmed++;

        // Remove quotes if present
        if (trimmed[0] == '"') {
            trimmed++;
            airlang_intg str_len = (airlang_intg)strlen(trimmed);
            if (str_len > 0 && trimmed[str_len - 1] == '"') {
                trimmed[str_len - 1] = '\0';
            }
        }

        emitInstruction(cg, OP_LOAD_STR, 0, trimmed);
        emitInstruction(cg, OP_PRINT, 0, "");
    }
}



airlang_intg isPrintStatement(const airlang_strg line) {
    return strstr(line, "PRINT") != NULL && strchr(line, '{') != NULL ;
}


airlang_void writeBinaryFile(const Generator* cg) {

    FILE* file = fopen(cg->output_filename, "wb");
    if (file == NULL) {
        printf("Error: At Open file , %s\n ", cg->output_filename);
        return;
    }

    airlang_intg magic = 0x41495254; // FOR VM to know bytecode file have AIRT -> TRIA ; HEADER HAS magic , instruction count and actual data
    fwrite(&magic, sizeof(airlang_intg), 1, file);
    fwrite(&cg->instruction_count, sizeof(airlang_intg), 1, file);

    for (airlang_intg i = 0; i < cg->instruction_count; i++) {
        fwrite(&cg->instructions[i], sizeof(Instruction), 1, file);
    }
    fclose(file);
    printf("Binary file written: %s (%d instructions) \n", cg->output_filename, cg->instruction_count);

    printf("\n BYTECODE DEBUG \n");
    for (int i = 0; i < cg->instruction_count; i++) {
        printf("Instruction %d: OpCode=%d, String='%s'\n",
            i, cg->instructions[i].opCode, cg->instructions[i].operand.str_operand);
    }

}