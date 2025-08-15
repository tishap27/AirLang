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


extern Variable variables[MAX_VARS];
extern airlang_intg var_count;
extern airlang_intg initial_phase;
extern airlang_char output_buffer[MAX_EXPR_LEN * 10];

// External functions from Step5Writer.c  
extern airlang_intg find_variable(const airlang_strg name);
extern airlang_void handle_write(airlang_strg expression);
extern airlang_doub calculateAirpath(airlang_doub lat1, airlang_doub lon1, airlang_doub lat2, airlang_doub lon2);
extern airlang_doub calcLastLegDistance();
extern airlang_doub headwind(airlang_doub wind_dir, airlang_doub wind_speed, airlang_doub runway);
extern airlang_doub crosswind(airlang_doub wind_dir, airlang_doub wind_speed, airlang_doub runway);
extern airlang_doub evaluate_expression_with_distance(const airlang_strg expr);


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

    vm->condition_result = 0;
    vm->in_if_block = 0;
    vm->skip_execution = 0;
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
    size_t bytes_to_read = vm->instruction_count * sizeof(Instruction);
    if (fread(vm->instructions,1, bytes_to_read , file) != bytes_to_read) {
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


        if (vm->skip_execution &&
            current->opCode != OP_ELSE &&
            current->opCode != OP_ENDIF &&
            current->opCode != OP_IF &&
            current->opCode != OP_CONDITION &&
            current->opCode != OP_PRINT_INTERPOLATED) {
            printf("SKIPPED\n");
            vm->program_counter++;
            continue;
        }
       
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

            case OP_STORE_COORDS:
                if (vm->stack_pointer > 0) {
                    Value val = pop(vm);
                    if (val.type == VAL_STRING) {
                        if (is_coordinate_format(val.data.string)) {
                            storeVariable(vm, current->operand.str_operand, val);
                            printf("Valid coordinates stored: %s\n", val.data.string);
                        }
                        else {
                            printf("Error: Invalid coordinate format: %s\n", val.data.string);

                        }
                    }
                    else {
                        printf("Error: STORE_COORDS requires string operand\n");
                    }
                }
                else {
                    printf("Error: Stack underflow for STORE_COORDS\n");
                }
                break;

            case OP_STORE_DATE:
                if (vm->stack_pointer > 0) {
                    Value val = pop(vm);
                    if (val.type == VAL_STRING) {

                        if (is_date_format(val.data.string)) {
                            storeVariable(vm, current->operand.str_operand, val);
                            printf("Valid date stored: %s\n", val.data.string);
                        }
                        else {
                            printf("Error: Invalid date format: %s\n", val.data.string);
                            printf("Expected format: 'YYYY-MM-DD' (e.g., '2025-05-15')\n");
                        }
                    }
                    else {
                        printf("Error: STORE_DATE requires string operand\n");
                    }
                }
                else {
                    printf("Error: Stack underflow for STORE_DATE\n");
                }
                break;


            case OP_CALC_EXPRESSION:
                if (vm->stack_pointer > 0) {
                    Value expr_val = pop(vm);
                    if (expr_val.type == VAL_STRING) {

                        airlang_doub result = evaluate_expression_with_distance(expr_val.data.string);
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


            case OP_PRINT_INTERPOLATED:
                printf("PRINT_INTERPOLATED %s\n", current->operand.str_operand);
                if (strlen(current->operand.str_operand) > 0) {
                    sync_variables(vm); 

                    printf("OUTPUT: ");

                    // Format for handle_write
                    airlang_char formatted_expr[MAX_EXPR_LEN];
                    airlang_strg expr = current->operand.str_operand;

                    // Removing outer quotes if present
                    if (expr[0] == '"' && expr[strlen(expr) - 1] == '"') {
                        airlang_char temp[MAX_EXPR_LEN];
                        strncpy(temp, expr + 1, strlen(expr) - 2);
                        temp[strlen(expr) - 2] = '\0';
                        snprintf(formatted_expr, sizeof(formatted_expr), "PRINT {%s}", temp);
                    }
                    else {
                        snprintf(formatted_expr, sizeof(formatted_expr), "PRINT {%s}", expr);
                    }

                    // Set initial_phase to 0 so handle_write prints directly
                    initial_phase = 0;
                    handle_write(formatted_expr);
                    initial_phase = 1; // Reset for safety
                }
                break;

            case OP_CONDITION:
                printf("CONDITION \"%s\"\n", current->operand.str_operand);

                // DEBUG print VM variable values
               /* printf("DEBUG: VM Variables - ");
                for (airlang_intg i = 0; i < vm->variable_count; i++) {
                    if (vm->variables[i].is_used) {
                        printf("%s=%.0f ", vm->variables[i].name, vm->variables[i].value.data.number);
                    }
                }
                printf("\n");*/

                //sync_variables(vm);
                vm->condition_result = evaluate_condition_vm(vm , current->operand.str_operand);
                printf("DEBUG: Condition result = %d\n", vm->condition_result); 
                break;

            case OP_IF:
                printf("IF (condition=%d)\n", vm->condition_result);
                vm->in_if_block = 1;
                vm->skip_execution = (vm->condition_result == 0 );
                break;

            case OP_ELSE:
                printf("ELSE\n");
                if (vm->in_if_block) {
                    vm->skip_execution = (vm->condition_result != 0);
                }
                break;

            case OP_ENDIF:
                printf("ENDIF\n");
                vm->skip_execution = 0;
                vm->condition_result = 0;
                vm->in_if_block = 0;
                break;

            case OP_CALC_DISTANCE:
                // Calculate great circle distance using your aviation functions
                if (vm->stack_pointer >= 4) {
                    Value lon2 = pop(vm);
                    Value lat2 = pop(vm);
                    Value lon1 = pop(vm);
                    Value lat1 = pop(vm);

                    if (lat1.type == VAL_NUMBER && lon1.type == VAL_NUMBER &&
                        lat2.type == VAL_NUMBER && lon2.type == VAL_NUMBER) {
                        airlang_doub distance = calculateAirpath(lat1.data.number, lon1.data.number,
                            lat2.data.number, lon2.data.number);
                        push(vm, createNumberValue(distance));
                    }
                    else {
                        printf("Error: CALC_DISTANCE requires numeric coordinates\n");
                    }
                }
                else {
                    // Use your existing calcLastLegDistance function
                    airlang_doub distance = calcLastLegDistance_vm(vm);
                    push(vm, createNumberValue(distance));
                }
                break;

            case OP_CALC_HEADWIND:
                // Calculate headwind component
                if (vm->stack_pointer >= 3) {
                    Value runway = pop(vm);
                    Value wind_speed = pop(vm);
                    Value wind_dir = pop(vm);

                    if (wind_dir.type == VAL_NUMBER && wind_speed.type == VAL_NUMBER && runway.type == VAL_NUMBER) {
                        airlang_doub hw = headwind(wind_dir.data.number, wind_speed.data.number, runway.data.number);
                        push(vm, createNumberValue(hw));
                    }
                    else {
                        printf("Error: CALC_HEADWIND requires numeric values\n");
                    }
                }
                else {
                    printf("Error: Not enough operands for CALC_HEADWIND\n");
                }
                break;

            case OP_CALC_CROSSWIND:
                // Calculate crosswind component
                if (vm->stack_pointer >= 3) {
                    Value runway = pop(vm);
                    Value wind_speed = pop(vm);
                    Value wind_dir = pop(vm);

                    if (wind_dir.type == VAL_NUMBER && wind_speed.type == VAL_NUMBER && runway.type == VAL_NUMBER) {
                        airlang_doub cw = crosswind(wind_dir.data.number, wind_speed.data.number, runway.data.number);
                        push(vm, createNumberValue(cw));
                    }
                    else {
                        printf("Error: CALC_CROSSWIND requires numeric values\n");
                    }
                }
                else {
                    printf("Error: Not enough operands for CALC_CROSSWIND\n");
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


airlang_intg evaluate_condition_vm(VirtualMachine* vm, const airlang_strg condition) {
    airlang_char tempCond[256];
    strcpy_s(tempCond, sizeof(tempCond), condition);

    // Remove spaces
    airlang_char cleanCond[256] = { 0 };
    airlang_intg i = 0, j = 0;
    while (tempCond[i]) {
        if (!isspace(tempCond[i])) {
            cleanCond[j++] = tempCond[i];
        }
        i++;
    }
    cleanCond[j] = '\0';

    // Find comparison operator
    airlang_char* operatorPos = NULL;
    if ((operatorPos = strstr(cleanCond, ">")) != NULL) {
        // Split left and right
        *operatorPos = '\0';
        airlang_char* leftOperand = cleanCond;
        airlang_char* rightOperand = operatorPos + 1;

        // Get left value from VM variables
        airlang_doub leftVal = 0.0;
        if (isdigit(leftOperand[0])) {
            leftVal = atof(leftOperand);
        }
        else {
            Value* var = getVariable(vm, leftOperand);
            if (var && var->type == VAL_NUMBER) {
                leftVal = var->data.number;
            }
        }

        // Get right value from VM variables  
        airlang_doub rightVal = 0.0;
        if (isdigit(rightOperand[0])) {
            rightVal = atof(rightOperand);
        }
        else {
            Value* var = getVariable(vm, rightOperand);
            if (var && var->type == VAL_NUMBER) {
                rightVal = var->data.number;
            }
        }

        printf("DEBUG: VM Comparing %.2f > %.2f = %s\n", leftVal, rightVal, (leftVal > rightVal) ? "TRUE" : "FALSE");
        return leftVal > rightVal;
    }
    // DEBUG print VM variable values
    printf("DEBUG: VM Variables - ");
    for (airlang_intg i = 0; i < vm->variable_count; i++) {
        if (vm->variables[i].is_used) {
            printf("%s=", vm->variables[i].name);
            if (vm->variables[i].value.type == VAL_NUMBER) {
                printf("%.0f ", vm->variables[i].value.data.number);
            }
            else if (vm->variables[i].value.type == VAL_STRING) {
                printf("\"%s\" ", vm->variables[i].value.data.string);
            }
            else {
                printf("UNKNOWN ");
            }
        }
    }
    printf("\n");
    return 0;
}




airlang_void handle_write_vm(VirtualMachine* vm, const airlang_strg expression) {
    sync_variables(vm);
    airlang_char formatted[512];
    sprintf_s(formatted, sizeof(formatted), "PRINT {%s}", expression);
    handle_write(formatted);
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
    VirtualMachine* vm =  malloc(sizeof(VirtualMachine));
    if (!vm) {
        printf("Error: Cannot allocate memory for VirtualMachine\n");
        return 0;
    }
    initVM(vm);

    if (!loadBytecode(vm, bytecode_file)) {
        printf("Failed to load bytecode file: %s\n", bytecode_file);
        return 0;
    }

    executeVM(vm);
    cleanupVM(vm);

    return 1;
}

// Copy VM variables to writer globals
airlang_void sync_variables(VirtualMachine* vm) {
    var_count = 0;

    // Copy VM variables to global variables array
    for (airlang_intg i = 0; i < vm->variable_count && var_count < MAX_VARS; i++) {
        if (vm->variables[i].is_used) {
            strncpy(variables[var_count].name, vm->variables[i].name, sizeof(variables[var_count].name) - 1);
            variables[var_count].name[sizeof(variables[var_count].name) - 1] = '\0';

            if (vm->variables[i].value.type == VAL_NUMBER) {
                variables[var_count].type = NUMERIC;
                variables[var_count].value.num_value = vm->variables[i].value.data.number;
            }
            else if (vm->variables[i].value.type == VAL_STRING) {
                variables[var_count].type = STRING;
                strncpy(variables[var_count].value.str_value, vm->variables[i].value.data.string, sizeof(variables[var_count].value.str_value) - 1);
                variables[var_count].value.str_value[sizeof(variables[var_count].value.str_value) - 1] = '\0';
            }
            var_count++;
        }
    }
}

airlang_doub calcLastLegDistance_vm(VirtualMachine* vm) {
    // Find coordinate variables in VM variable table
    airlang_intg coord_count = 0;
    airlang_intg coord_indices[10];

    for (airlang_intg i = 0; i < vm->variable_count; i++) {
        if (vm->variables[i].is_used && vm->variables[i].value.type == VAL_STRING) {
            if (is_coordinate_format(vm->variables[i].value.data.string)) {
                coord_indices[coord_count] = i;
                coord_count++;
                if (coord_count >= 10) break;
            }
        }
    }

    if (coord_count < 2) return 0.0;

    // Use last two coordinates
    airlang_intg idx1 = coord_indices[coord_count - 2];
    airlang_intg idx2 = coord_indices[coord_count - 1];

    airlang_doub lat1, lon1, lat2, lon2;

    if (parse_coordinates(vm->variables[idx1].value.data.string, &lat1, &lon1) &&
        parse_coordinates(vm->variables[idx2].value.data.string, &lat2, &lon2)) {
        return calculateAirpath(lat1, lon1, lat2, lon2);
    }

    return 0.0;
}