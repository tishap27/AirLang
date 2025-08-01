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

#ifndef SCANNER_H_
#include "Step3Scanner.h"
#endif

#ifndef PARSER_H_
#include "Step4Parser.h"
#endif

#ifndef WRITER_H_
#include "Step5Writer.h"
#endif


#ifndef GENERATOR_H
#define GENERATOR_H

#define MAX_CODE_SIZE 10000
#define MAX_INSTRUCTIONS 1000

#include <stddef.h>  // For size_t

//Global 
extern airlang_strg keywordTable[KWT_SIZE];

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
	ARTHOP_DIV,

	OP_ENTER_MAIN,
	OP_EXIT_MAIN,
	OP_ENTER_BRIEFING,
	OP_EXIT_BRIEFING,
	OP_ENTER_WEATHER,
	OP_EXIT_WEATHER,
	OP_ENTER_LOADSHEET,
	OP_EXIT_LOADSHEET,
	OP_ENTER_DISPATCH,
	OP_EXIT_DISPATCH,

	OP_ENTER_AIRCRAFT,

	OP_ENTER_FLIGHT,
	
	OP_ENTER_ROUTE,


	OP_ENTER_RECEIVEDDATA,
	OP_EXIT_RECEIVEDDATA,
	OP_ENTER_RUNWAYDATA,
	OP_EXIT_RUNWAYDATA,
	OP_ENTER_WINDANALYSIS,
	OP_EXIT_WINDANALYSIS,
	OP_ENTER_SAFETYALERT,
	OP_EXIT_SAFETYALERT,
	OP_ENTER_REPORT,
	OP_EXIT_REPORT,
	
	OP_ENTER_BLOCK  // fallback generic opcode

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


airlang_intg getKeywordCode(const airlang_strg word);

const airlang_strg getBlockNameFromKeyword(airlang_intg keyword);
OpCode getBlockOpFromKeyword(airlang_intg keyword);
airlang_intg isBlockKeyword(airlang_intg keyword);

airlang_intg isBlockStart(const airlang_strg line);
airlang_intg isBlockEnd(const airlang_strg line);

airlang_void generateBlockStart(const airlang_strg line, Generator* cg);

airlang_void generateBlockEnd(const airlang_strg line, Generator* cg);


#endif