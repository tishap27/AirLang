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
# ECHO "[VIRTUAL MACHINE SCRIPT ................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Step7Vm.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main header for Code Generator (.h)
************************************************************
*/

#define _CRT_SECURE_NO_WARNINGS



#include <stddef.h>
#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef GENERATOR_H_
#include "Step6Generator.h"
#endif

#ifndef VM_H
#define VM_H

#define MAX_STACK_SIZE 1000
#define MAX_VARIABLES 100
#define MAX_STRING_LENGTH 256
#define MAGIC_NUMBER 0x41495254  // AIRT


// Stack value type
typedef enum {
    VAL_NUMBER,
    VAL_STRING
} ValueType;

// Stack value
typedef struct {
    ValueType type;
    union {
        airlang_doub number;
        airlang_char string[MAX_STRING_LENGTH];
    } data;
} Value;

// Variable storage
typedef struct {
    airlang_char name[64];
    Value value;
    airlang_intg is_used;
} Var;

// Virtual Machine structure
typedef struct {
    Instruction* instructions;
    airlang_intg instruction_count;
    airlang_intg program_counter;  // Current instruction

    Value stack[MAX_STACK_SIZE];
    airlang_intg stack_pointer;

    Var variables[MAX_VARIABLES];
    airlang_intg variable_count;

    airlang_intg running;
} VirtualMachine;



airlang_void initVM(VirtualMachine* vm);

airlang_intg loadBytecode(VirtualMachine* vm, const airlang_strg filename);
airlang_void executeVM(VirtualMachine* vm);
airlang_void cleanupVM(VirtualMachine* vm);

// Stack operations
airlang_void push(VirtualMachine* vm, Value value);
Value pop(VirtualMachine* vm);


Value createNumberValue(airlang_doub num);

Value createStringValue(const airlang_strg str);

airlang_void storeVariable(VirtualMachine* vm, const airlang_strg name, Value value);
Value* getVariable(VirtualMachine* vm, const airlang_strg name);
airlang_void printValue(const Value* value);

airlang_intg runVirtualMachine(const airlang_strg bytecode_file);

#endif