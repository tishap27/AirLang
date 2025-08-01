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
    snprintf(output_filename, sizeof(output_filename), "%s.air", source);  // so will become file.txt.air; weird filename- will change later

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

   // if (isPrintStatement(source_content)) {
     //   generatePrint(source_content, cg);
    //}
    parseAndGenerate(source_content, cg);

	emitInstruction(cg, OP_HALT, 0, "");

	printf("Code Generation complete. Generated %d instructions.\n", cg->instruction_count);
}
airlang_void parseAndGenerate(const airlang_strg content, Generator* cg) {
    airlang_char line[1024];
    airlang_intg i = 0, line_start = 0;

    // Simple line-by-line parsing
    while (content[i] != '\0') {
        if (content[i] == '\n' || content[i] == ';') {
            // Extract line
            airlang_intg line_len = i - line_start;
            strncpy(line, &content[line_start], line_len);
            line[line_len] = '\0';

            // Trim whitespace
            airlang_char* trimmed = line;
            while (*trimmed == ' ' || *trimmed == '\t') trimmed++;

            if (strlen(trimmed) > 0 && trimmed[0] != '^' && trimmed[0] != '%') {
                // Generate code based on line type
                if (isAssignment(trimmed)) {
                    generateAssignment(trimmed, cg);
                }
                else if (isPrintStatement(trimmed)) {
                    generatePrint(trimmed, cg);
                }
            }

            line_start = i + 1;
        }
        i++;
    }
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

airlang_void generateAssignment(const airlang_strg line, Generator* cg) {
    airlang_char var_name[64];
    airlang_char value_str[256];

    // Simple parsing: VarName: Value;
    if (sscanf(line, "%63[^:]: %255[^;]", var_name, value_str) == 2) {
        // Trim spaces
        airlang_char* trimmed_var = var_name;
        while (*trimmed_var == ' ') trimmed_var++;

        airlang_char* trimmed_val = value_str;
        while (*trimmed_val == ' ') trimmed_val++;

        // Check if value is numeric
        airlang_doub num_val;
        if (sscanf(trimmed_val, "%lf", &num_val) == 1) {
            // Numeric assignment
            emitInstruction(cg, OP_LOAD_NUM, num_val, "");
            emitInstruction(cg, OP_STORE_VAR, 0, trimmed_var);
        }
        else {
            // String assignment
            // Remove quotes if present
            if (trimmed_val[0] == '"') {
                trimmed_val++;
                airlang_intg len = (airlang_intg)strlen(trimmed_val);
                if (len > 0 && trimmed_val[len - 1] == '"') {
                    trimmed_val[len - 1] = '\0';
                }
            }
            emitInstruction(cg, OP_LOAD_STR, 0, trimmed_val);
            emitInstruction(cg, OP_STORE_VAR, 0, trimmed_var);
        }
    }
}


airlang_intg isAssignment(const airlang_strg line) {
    return strchr(line, ':') != NULL && !strstr(line, "PRINT");
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