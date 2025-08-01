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
* File name: Step6Generator.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main header for Code Generator (.h)
************************************************************
*/

#define _CRT_SECURE_NO_WARNINGS

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

/*#ifndef STEP4PARSER_H_
#include "Step4Parser.h"
#endif*/

/*#ifndef PARSER_H_
#include "Step4Parser.h"
#endif*/

#ifndef WRITER_H_
#include "Step5Writer.h"
#endif


#ifndef GENERATOR_H
#define GENERATOR_H

#define MAX_CODE_SIZE 10000
#define MAX_INSTRUCTIONS 1000

#include <stddef.h>  // For size_t

#define SAFE_COPY(dest, src) strncpy(dest, src, sizeof(dest)-1); dest[sizeof(dest)-1] = '\0'
#define IS_QUOTE(c) (c == '"' || c == '\'')

typedef enum {
	OP_LOAD_NUM,
	OP_LOAD_STR,
	OP_STORE_VAR,
	OP_PRINT,
	OP_HALT,
	ARTHOP_ADD,
	ARTHOP_SUB, 
	ARTHOP_MULTI,
	ARTHOP_DIV

}OpCode;

typedef struct {
	OpCode opCode; 
	union {
		airlang_doub num_operand; 
		airlang_char str_operand[256]; 
		airlang_intg int_operand; 
	} operand;
} Instruction;

typedef struct {
	Instruction instructions[MAX_INSTRUCTIONS];
	airlang_intg instruction_count; 
	airlang_char output_filename[256];

}Generator;

/*Function declarations now */

airlang_void processFileGeneration(const airlang_strg source, const airlang_strg content);
airlang_void displayGeneratedCode(const Generator* cg);

airlang_void initGenerator(Generator* cg, const airlang_strg output_file);

airlang_void generateCode(const airlang_strg source_content, Generator* cg);

airlang_void parseAndGenerate(const airlang_strg content, Generator* cg);

airlang_void emitInstruction(Generator* cg, OpCode op, airlang_doub num_val, const airlang_strg str_val);

airlang_void writeBinaryFile(const Generator* cg);

airlang_void generatePrint(const airlang_strg line, Generator* cg);

airlang_intg isPrintStatement(const airlang_strg line);

airlang_void generateAssignment(const airlang_strg line, Generator* cg);

airlang_intg isAssignment(const airlang_strg line);

airlang_void generateCalculation(const airlang_strg line, Generator* cg);

airlang_intg isCalculation(const airlang_strg line);

static airlang_void removeQuotes(airlang_strg str);


#endif