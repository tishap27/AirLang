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
* File name: Step7Vm.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file implements the Virtual Machine
************************************************************
*/


#ifndef VM_H
#include "Step7VM.h"
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


airlang_void initVM(VirtualMachine* vm) {
    vm->instructions = NULL;
    vm->instruction_count = 0;
    vm->program_counter = 0;
    vm->stack_pointer = 0;
    vm->variable_count = 0;
    vm->running = 1;

    // Clear variables
    for (airlang_intg i = 0; i < MAX_VARIABLES; i++) {
        vm->variables[i].is_used = 0;
        memset(vm->variables[i].name, 0, sizeof(vm->variables[i].name));
    }

    printf("Virtual Machine initialized.\n");
}

airlang_intg loadBytecode(VirtualMachine* vm, const airlang_strg filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Cannot open bytecode file %s\n", filename);
        return 0;
    }
    airlang_intg magic;
    if (fread(&magic, sizeof(airlang_intg), 1, file) != 1) {
        printf("Error: Cannot read magic number\n");
        fclose(file);
        return 0;
    }

    if (magic != MAGIC_NUMBER) {
        printf("Error: Invalid bytecode file (wrong magic number: 0x%X)\n", magic);
        fclose(file);
        return 0;
    }

    // Read instruction count
    if (fread(&vm->instruction_count, sizeof(airlang_intg), 1, file) != 1) {
        printf("Error: Cannot read instruction count\n");
        fclose(file);
        return 0;
    }

    // Allocate memory for instructions
    vm->instructions = malloc(vm->instruction_count * sizeof(Instruction));
    if (!vm->instructions) {
        printf("Error: Cannot allocate memory for instructions\n");
        fclose(file);
        return 0;
    }

    // Read instructions
    if (fread(vm->instructions, sizeof(Instruction), vm->instruction_count, file) != vm->instruction_count) {
        printf("Error: Cannot read instructions\n");
        free(vm->instructions);
        fclose(file);
        return 0;
    }

    fclose(file);
    printf("Loaded %d instructions from %s\n", vm->instruction_count, filename);
    return 1;
}



airlang_void executeVM(VirtualMachine* vm) {
    printf("\n=== EXECUTING AIRLANG BYTECODE ===\n");

    while (vm->running && vm->program_counter < vm->instruction_count) {
        Instruction* current = &vm->instructions[vm->program_counter];

        printf("PC:%d Executing: OpCode=%d ", vm->program_counter, current->opCode);

        switch (current->opCode) {
        case OP_LOAD_NUM:
            printf("LOAD_NUM %.2f\n", current->operand.num_operand);
            push(vm, createNumberValue(current->operand.num_operand));
            break;

        case OP_LOAD_STR:
            printf("LOAD_STR \"%s\"\n", current->operand.str_operand);
            push(vm, createStringValue(current->operand.str_operand));
            break;
        
        case OP_STORE_VAR:
            printf("STORE_VAR %s\n", current->operand.str_operand);
            if (vm->stack_pointer > 0) {
                Value val = pop(vm);
                storeVariable(vm, current->operand.str_operand, val);
            }
            else {
                printf("Error: Stack underflow for STORE_VAR\n");
            }
            break;

        

        case OP_PRINT:
            printf("PRINT\n");
            if (vm->stack_pointer > 0) {
                Value val = pop(vm);
                printf("OUTPUT: ");
                printValue(&val);
                printf("\n");
            }
            break;
        
        case OP_STORE_AIRCRAFT_ID:
            if (vm->stack_pointer > 0) {
                Value val = pop(vm); 
                if (val.type == VAL_STRING) {
                    if (is_aircraft_identifier(val.data.string)) {
                        storeVariable(vm, current->operand.str_operand, val);
                        printf("Valid aircraft ID stored: %s\n", val.data.string);
                    }
                    else {
                        printf("Error: Invalid aircraft identifier format: %s\n", val.data.string);
                        printf("Expected formats: AL123 (IATA) or C-GNBL (ICAO)\n");
                    }
                }
                else {
                    printf("Error: STORE_AIRCRAFT_ID requires string operand\n");
                }

            }
            else {
                printf("Error: Stack underflow for STORE_AIRCRAFT_ID\n");

            }
            break;


        case OP_CALC_EXPRESSION:
            if (vm->stack_pointer > 0) {
                Value expr_val = pop(vm);
                if (expr_val.type == VAL_STRING) {
                    
                    airlang_doub result = evaluate_expression(expr_val.data.string);
                    push(vm, createNumberValue(result));
                }
                else {
                    printf("Error: CALC_EXPRESSION requires string operand\n");
                }
            }
            else {
                printf("Error: Stack underflow for CALC_EXPRESSION\n");
            }
            break;
      


        case OP_HALT:
            printf("HALT\n");
            vm->running = 0;
            break;

        default:
            printf("UNKNOWN_OP\n");
            printf("Warning: Unknown opcode %d\n", current->opCode);
            break;
        }

        vm->program_counter++;
    }

    printf("=== EXECUTION COMPLETED ===\n");
}


// Clean up VM resources
airlang_void cleanupVM(VirtualMachine * vm) {
    if (vm->instructions) {
        free(vm->instructions);
        vm->instructions = NULL;
    }
    printf("Virtual Machine cleaned up.\n");
}

// Stack operations
airlang_void push(VirtualMachine* vm, Value value) {
    if (vm->stack_pointer >= MAX_STACK_SIZE) {
        printf("Error: Stack overflow\n");
        return;
    }
    vm->stack[vm->stack_pointer++] = value;
}

Value pop(VirtualMachine* vm) {
    if (vm->stack_pointer <= 0) {
        printf("Error: Stack underflow\n");
        Value empty = { VAL_NUMBER, {0} };
        return empty;
    }
    return vm->stack[--vm->stack_pointer];
}

Value createNumberValue(airlang_doub num) {
    Value val; 
    val.type = VAL_NUMBER;
    val.data.number = num; 
    return val;
}



Value createStringValue(const airlang_strg str) {
    Value val;
    val.type = VAL_STRING;
    strncpy(val.data.string, str, MAX_STRING_LENGTH - 1);
    val.data.string[MAX_STRING_LENGTH - 1] = '\0';
    return val;
}

airlang_void storeVariable(VirtualMachine* vm , const airlang_strg name , Value value ) {
   
    //if variable exists

    for (airlang_intg i = 0; i < vm->variable_count; i++) {
        if (vm->variables[i].is_used && strcmp(vm->variables[i].name, name) == 0) {
            //changing current variable to new 
            vm->variables[i].value = value; 

            printf("Updated variable is: %s", name); 
            printValue(&value); 
            printf("\n"); 
            return; 

        }
    
    }


    if (vm->variable_count >= MAX_VARIABLES) {
        printf("Error: Maximum variables exceeded\n");
        return;
    }

    //now creating new variable

    vm->variables[vm->variable_count].is_used = 1; 
    strncpy(vm->variables[vm->variable_count].name, name, sizeof(vm->variables[vm->variable_count].name) - 1);
    vm->variables[vm->variable_count].name[sizeof(vm->variables[vm->variable_count].name) - 1] = '\0';
    vm->variables[vm->variable_count].value = value;

    printf("Created variable '%s' = ", name);
    printValue(&value);
    printf("\n");

    vm->variable_count++;

}

Value* getVariable(VirtualMachine* vm, const airlang_strg name) {
    for (airlang_intg i = 0; i < vm->variable_count; i++) {
        if (vm->variables[i].is_used && strcmp(vm->variables[i].name, name) == 0) {
            return &vm->variables[i].value;
        }
    }
    return NULL; // Variable not found
}

airlang_void printValue(const Value* value) {
    switch (value->type) {
    case VAL_NUMBER:
        printf("%.2f", value->data.number);
        break;
    case VAL_STRING:
        printf("%s", value->data.string);
        break;

    default:
        printf("UNKNOWN_VALUE_TYPE\n");
        break;
    }

}


airlang_intg runVirtualMachine(const airlang_strg bytecode_file) {
    VirtualMachine vm;

    initVM(&vm);

    if (!loadBytecode(&vm, bytecode_file)) {
        printf("Failed to load bytecode file: %s\n", bytecode_file);
        return 0;
    }

    executeVM(&vm);
    cleanupVM(&vm);

    return 1;
}