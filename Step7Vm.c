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

extern airlang_char current_airport[16];

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

                    if (strstr(current->operand.str_operand, "METAR") != NULL && val.type == VAL_STRING) {
                        update_airport_context_from_metar(vm, val.data.string);
                        process_metar_in_vm(vm, val.data.string);
                    }

                    if (strstr(current->operand.str_operand, "_RUNWAY") != NULL) {
                        update_current_airport(vm, current->operand.str_operand);
                    }
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
                printf("CALC_EXPRESSION \"%s\"\n", current->operand.str_operand);

                // The expression is in the instruction operand, NOT on the stack
                if (strlen(current->operand.str_operand) > 0) {
                    // Sync variables so expression evaluator can access them
                    sync_variables(vm);
                

                        airlang_doub result = evaluate_expression_with_distance(current->operand.str_operand);
                        push(vm, createNumberValue(result));

                        printf("Expression '%s' evaluated to %.2f\n", current->operand.str_operand, result);
                }
                else {
                        printf("Error: CALC_EXPRESSION requires string operand\n");
                        push(vm, createNumberValue(0.0));
                 }
               
                break;


            case OP_PRINT_INTERPOLATED:
                printf("PRINT_INTERPOLATED %s\n", current->operand.str_operand);
                if (strlen(current->operand.str_operand) > 0) {
                    sync_variables(vm);

                    printf("OUTPUT: ");

                    airlang_char formatted_expr[MAX_EXPR_LEN];
                    airlang_strg expr = current->operand.str_operand;

                    printf("DEBUG: Analyzing expression: '%s'\n", expr);

                    // Count quotes to understand the structure
                    airlang_intg quote_count = 0;
                    airlang_intg has_plus = 0;
                    for (airlang_intg i = 0; i < (airlang_intg)strlen(expr); i++) {
                        if (expr[i] == '"') quote_count++;
                        if (expr[i] == '+') has_plus = 1;
                    }

                    printf("DEBUG: Found %d quotes, has_plus=%d\n", quote_count, has_plus);

                    if (has_plus && quote_count > 2) {
                        // Complex expression with multiple parts - DON'T remove outer quotes ""something" + value + "something""
                        snprintf(formatted_expr, sizeof(formatted_expr), "PRINT {%s}", expr);
                        printf("DEBUG: Complex expression, keeping all quotes\n");
                    }
                    else if (expr[0] == '"' && expr[strlen(expr) - 1] == '"' && quote_count == 2) {
                        // Simple expression with just outer quotes - remove them
                        airlang_char temp[MAX_EXPR_LEN];
                        strncpy(temp, expr + 1, strlen(expr) - 2);
                        temp[strlen(expr) - 2] = '\0';
                        snprintf(formatted_expr, sizeof(formatted_expr), "PRINT {%s}", temp);
                        printf("DEBUG: Simple expression, removed outer quotes\n");
                    }
                    else {
                        // No modification needed
                        snprintf(formatted_expr, sizeof(formatted_expr), "PRINT {%s}", expr);
                        printf("DEBUG: No quote modification needed\n");
                    }

                    printf("DEBUG: Final formatted: '%s'\n", formatted_expr);

                    airlang_intg saved_phase = initial_phase;
                    initial_phase = 0;

                    handle_write(formatted_expr);

                    initial_phase = saved_phase;
                    printf("\n");
                    sync_variables_back(vm);
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
                //Airpath
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
                    // calcLastLegDistance function
                    airlang_doub distance = calcLastLegDistance_vm(vm);
                    push(vm, createNumberValue(distance));
                }
                break;

            case OP_CALC_HEADWIND:
                printf("CALC_HEADWIND\n");
                if (strlen(current_airport) > 0) {
                    // Look for wind variables with current airport prefix
                    airlang_char wind_dir_var[32], wind_speed_var[32], runway_var[32];
                    snprintf(wind_dir_var, sizeof(wind_dir_var), "%s_WIND_DIR", current_airport);
                    snprintf(wind_speed_var, sizeof(wind_speed_var), "%s_WIND_SPEED", current_airport);
                    snprintf(runway_var, sizeof(runway_var), "%s_RUNWAY", current_airport);

                    printf("DEBUG: Looking for variables: %s, %s, %s\n", wind_dir_var, wind_speed_var, runway_var);

                    Value* wind_dir_val = getVariable(vm, wind_dir_var);
                    Value* wind_speed_val = getVariable(vm, wind_speed_var);
                    Value* runway_val = getVariable(vm, runway_var);

                    printf("DEBUG: Found wind_dir=%p, wind_speed=%p, runway=%p\n",
                        wind_dir_val, wind_speed_val, runway_val);

                    if (wind_dir_val && wind_speed_val && runway_val &&
                        wind_dir_val->type == VAL_NUMBER && wind_speed_val->type == VAL_NUMBER &&
                        runway_val->type == VAL_NUMBER) {

                        printf("DEBUG: Using values - wind_dir=%.0f, wind_speed=%.0f, runway=%.0f\n",
                            wind_dir_val->data.number, wind_speed_val->data.number, runway_val->data.number);

                        airlang_doub hw = headwind(wind_dir_val->data.number, wind_speed_val->data.number,
                            runway_val->data.number);
                        push(vm, createNumberValue(hw));
                        printf("Calculated headwind: %.2f kt\n", hw);

                        //  storing the headwind result in a specific variable
                        airlang_char headwind_var[32];
                        snprintf(headwind_var, sizeof(headwind_var), "%s_HEADWIND", current_airport);
                        Value headwind_value = createNumberValue(hw);
                        storeVariable(vm, headwind_var, headwind_value);
                        printf("Stored headwind in variable: %s = %.2f\n", headwind_var, hw);
                    }
                    else {
                        printf("Error: Missing wind or runway data for headwind calculation\n");
                        printf("DEBUG: Variables found - wind_dir: %s, wind_speed: %s, runway: %s\n",
                            wind_dir_val ? "YES" : "NO",
                            wind_speed_val ? "YES" : "NO",
                            runway_val ? "YES" : "NO");
                        push(vm, createNumberValue(0.0));
                    }
                }
                else if (vm->stack_pointer >= 3) {
                    
                    Value runway = pop(vm);
                    Value wind_speed = pop(vm);
                    Value wind_dir = pop(vm);

                    if (wind_dir.type == VAL_NUMBER && wind_speed.type == VAL_NUMBER && runway.type == VAL_NUMBER) {
                        airlang_doub hw = headwind(wind_dir.data.number, wind_speed.data.number, runway.data.number);
                        push(vm, createNumberValue(hw));
                        printf("Calculated headwind (stack-based): %.2f kt\n", hw);
                    }
                    else {
                        printf("Error: CALC_HEADWIND requires numeric values\n");
                        push(vm, createNumberValue(0.0));
                    }
                }
                else {
                    printf("Error: Not enough operands for CALC_HEADWIND and no airport context\n");
                    push(vm, createNumberValue(0.0));
                }
                break;
               

            case OP_CALC_CROSSWIND:
                printf("CALC_CROSSWIND\n");

                // Try to find wind variables from current airport context first
                if (strlen(current_airport) > 0) {
                    // Look for wind variables with current airport prefix
                    airlang_char wind_dir_var[32], wind_speed_var[32], runway_var[32];
                    snprintf(wind_dir_var, sizeof(wind_dir_var), "%s_WIND_DIR", current_airport);
                    snprintf(wind_speed_var, sizeof(wind_speed_var), "%s_WIND_SPEED", current_airport);
                    snprintf(runway_var, sizeof(runway_var), "%s_RUNWAY", current_airport);

                    printf("DEBUG: Looking for variables: %s, %s, %s\n", wind_dir_var, wind_speed_var, runway_var);

                    Value* wind_dir_val = getVariable(vm, wind_dir_var);
                    Value* wind_speed_val = getVariable(vm, wind_speed_var);
                    Value* runway_val = getVariable(vm, runway_var);

                    printf("DEBUG: Found wind_dir=%p, wind_speed=%p, runway=%p\n",
                        wind_dir_val, wind_speed_val, runway_val);

                    if (wind_dir_val && wind_speed_val && runway_val &&
                        wind_dir_val->type == VAL_NUMBER && wind_speed_val->type == VAL_NUMBER &&
                        runway_val->type == VAL_NUMBER) {

                        printf("DEBUG: Using values - wind_dir=%.0f, wind_speed=%.0f, runway=%.0f\n",
                            wind_dir_val->data.number, wind_speed_val->data.number, runway_val->data.number);

                        airlang_doub cw = crosswind(wind_dir_val->data.number, wind_speed_val->data.number,
                            runway_val->data.number);
                        push(vm, createNumberValue(cw));
                        printf("Calculated crosswind: %.2f kt\n", cw);

                        
                        airlang_char crosswind_var[32];
                        snprintf(crosswind_var, sizeof(crosswind_var), "%s_CROSSWIND", current_airport);
                        Value crosswind_value = createNumberValue(cw);
                        storeVariable(vm, crosswind_var, crosswind_value);
                        printf("Stored crosswind in variable: %s = %.2f\n", crosswind_var, cw);
                    }
                    else {
                        printf("Error: Missing wind or runway data for crosswind calculation\n");
                        printf("DEBUG: Variables found - wind_dir: %s, wind_speed: %s, runway: %s\n",
                            wind_dir_val ? "YES" : "NO",
                            wind_speed_val ? "YES" : "NO",
                            runway_val ? "YES" : "NO");
                        push(vm, createNumberValue(0.0));
                    }
                }
                else if (vm->stack_pointer >= 3) {
                    
                    Value runway = pop(vm);
                    Value wind_speed = pop(vm);
                    Value wind_dir = pop(vm);

                    if (wind_dir.type == VAL_NUMBER && wind_speed.type == VAL_NUMBER && runway.type == VAL_NUMBER) {
                        airlang_doub cw = crosswind(wind_dir.data.number, wind_speed.data.number, runway.data.number);
                        push(vm, createNumberValue(cw));
                        printf("Calculated crosswind (stack-based): %.2f kt\n", cw);
                    }
                    else {
                        printf("Error: CALC_CROSSWIND requires numeric values\n");
                        push(vm, createNumberValue(0.0));
                    }
                }
                else {
                    printf("Error: Not enough operands for CALC_CROSSWIND and no airport context\n");
                    push(vm, createNumberValue(0.0));
                }
                break;

            case OP_ENTER_MAIN:
                printf("ENTER_MAIN\n");
                break;
            case OP_EXIT_MAIN:
                printf("EXIT_MAIN\n");
                break;
            case OP_ENTER_BRIEFING:
                printf("ENTER_BRIEFING\n");
                break;
            case OP_EXIT_BRIEFING:
                printf("EXIT_BRIEFING\n");
                break;
            case OP_ENTER_WEATHER:
                printf("ENTER_WEATHER\n");
                break;
            case OP_EXIT_WEATHER:
                printf("EXIT_WEATHER\n");
                break;
            case OP_ENTER_LOADSHEET:
                printf("ENTER_LOADSHEET\n");
                break;
            case OP_EXIT_LOADSHEET:
                printf("EXIT_LOADSHEET\n");
                break;
            case OP_ENTER_DISPATCH:
                printf("ENTER_DISPATCH\n");
                break;
            case OP_EXIT_DISPATCH:
                printf("EXIT_DISPATCH\n");
                break;

            case OP_ENTER_AIRCRAFT:
                printf("ENTER_AIRCRAFT\n");
                break;
            case OP_ENTER_FLIGHT:
                printf("ENTER_FLIGHT\n");
                break;
            case OP_ENTER_ROUTE:
                printf("ENTER_ROUTE\n");
                break;
            case OP_ENTER_RECEIVEDDATA:
                printf("ENTER_RECEIVEDDATA\n");
                break;
            case OP_EXIT_RECEIVEDDATA:
                printf("EXIT_RECEIVEDDATA\n");
                break;
            case OP_ENTER_RUNWAYDATA:
                printf("ENTER_RUNWAYDATA\n");
                break;
            case OP_EXIT_RUNWAYDATA:
                printf("EXIT_RUNWAYDATA\n");
                break;
            case OP_ENTER_WINDANALYSIS:
                printf("ENTER_WINDANALYSIS\n");
                break;
            case OP_EXIT_WINDANALYSIS:
                printf("EXIT_WINDANALYSIS\n");
                break;
            case OP_ENTER_SAFETYALERT:
                printf("ENTER_SAFETYALERT\n");
                break;
            case OP_EXIT_SAFETYALERT:
                printf("EXIT_SAFETYALERT\n");
                break;
            case OP_ENTER_REPORT:
                printf("ENTER_REPORT\n");
                break;
            case OP_EXIT_REPORT:
                printf("EXIT_REPORT\n");
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

            printf("Updated variable '%s' = ", name); 
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


airlang_void process_metar_in_vm(VirtualMachine* vm, const airlang_strg metar_string) {
    // Extract airport code from METAR string
    airlang_char station_id[16] = { 0 };
    airlang_char* metar_pos = strstr(metar_string, "METAR");

    if (metar_pos != NULL) {
        // Skip "METAR" and whitespace  
        metar_pos += 5;
        while (*metar_pos && isspace(*metar_pos)) metar_pos++;

        // Extract airport code
        airlang_intg i = 0;
        while (*metar_pos && !isspace(*metar_pos) && i < sizeof(station_id) - 1) {
            station_id[i++] = *metar_pos++;
        }
        station_id[i] = '\0';

        if (strlen(station_id) > 0) {
            // Use Step5Writer functions to parse METAR
            sync_variables(vm);  // Sync VM vars to global vars
            parseMetar(metar_string, station_id);  
            sync_variables_back(vm);  // Sync global vars back to VM

            printf("METAR parsed for station: %s\n", station_id);
        }
    }
}

// function to sync global variables back to VM
airlang_void sync_variables_back(VirtualMachine* vm) {
    // Copy global variables back to VM variables
    for (airlang_intg i = 0; i < var_count; i++) {
        // Find if variable exists in VM
        airlang_intg vm_idx = -1;
        for (airlang_intg j = 0; j < vm->variable_count; j++) {
            if (vm->variables[j].is_used && strcmp(vm->variables[j].name, variables[i].name) == 0) {
                vm_idx = j;
                break;
            }
        }

        // If not found, create new VM variable
        if (vm_idx == -1) {
            if (vm->variable_count >= MAX_VARIABLES) continue;
            vm_idx = vm->variable_count++;
            vm->variables[vm_idx].is_used = 1;
            strncpy(vm->variables[vm_idx].name, variables[i].name, sizeof(vm->variables[vm_idx].name) - 1);
            vm->variables[vm_idx].name[sizeof(vm->variables[vm_idx].name) - 1] = '\0';
        }

        // Copy value based on type
        if (variables[i].type == NUMERIC) {
            vm->variables[vm_idx].value.type = VAL_NUMBER;
            vm->variables[vm_idx].value.data.number = variables[i].value.num_value;
        }
        else if (variables[i].type == STRING) {
            vm->variables[vm_idx].value.type = VAL_STRING;
            strncpy(vm->variables[vm_idx].value.data.string, variables[i].value.str_value,
                sizeof(vm->variables[vm_idx].value.data.string) - 1);
            vm->variables[vm_idx].value.data.string[sizeof(vm->variables[vm_idx].value.data.string) - 1] = '\0';
        }
    }
}




//function to extract airport from variable assignments
airlang_void update_current_airport(VirtualMachine* vm, const airlang_strg var_name) {
    // Look for pattern like "CYOW_RUNWAY" to extract "CYOW"
    if (strstr(var_name, "_RUNWAY") != NULL) {
        airlang_intg len = (airlang_intg)(strstr(var_name, "_RUNWAY") - var_name);
        if (len > 0 && len < sizeof(current_airport) - 1) {
            strncpy(current_airport, var_name, len);
            current_airport[len] = '\0';
            printf("Updated current airport context: %s\n", current_airport);
        }
    }
}


airlang_void update_airport_context_from_metar(VirtualMachine* vm, const airlang_strg metar_string) {
    airlang_char* metar_pos = strstr(metar_string, "METAR");
    if (metar_pos != NULL) {
        // Skip "METAR" and whitespace  
        metar_pos += 5;
        while (*metar_pos && isspace(*metar_pos)) metar_pos++;

        // Extracting airport code
        airlang_intg i = 0;
        while (*metar_pos && !isspace(*metar_pos) && i < sizeof(current_airport) - 1) {
            current_airport[i++] = *metar_pos++;
        }
        current_airport[i] = '\0';
        printf("DEBUG: Set current airport from METAR: %s\n", current_airport);
    }
}
