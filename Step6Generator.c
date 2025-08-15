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

static double generator_dep_lat = 0.0;
static double generator_dep_lon = 0.0;
static double generator_arr_lat = 0.0;
static double generator_arr_lon = 0.0;
static int coords_initialized = 0;



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
            printf("LOAD_STR \"%s\"\n", inst->operand.str_operand);
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
        case ARTHOP_ADD:
            printf("ADD\n");
            break;
        case ARTHOP_SUB:
            printf("SUB\n");
            break;
        case ARTHOP_MULTI:
            printf("MUL\n");
            break;
        case ARTHOP_DIV:
            printf("DIV\n");
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
       /* case OP_ENTER_BLOCK:
            printf("ENTER_BLOCK\n");
            break;*/
        case OP_PRINT_INTERPOLATED:
            printf("PRINT_INTERPOLATED \"%s\"\n", inst->operand.str_operand);
            break;
        case OP_STORE_AIRCRAFT_ID:
            printf("STORE_AIRCRAFT_ID %s\n", inst->operand.str_operand);
            break;
        case OP_STORE_COORDS:
            printf("STORE_COORDS %s\n", inst->operand.str_operand);
            break;
        case OP_STORE_DATE:
            printf("STORE_DATE %s\n", inst->operand.str_operand);
            break;
        case OP_CALC_DISTANCE:
            printf("CALC_DISTANCE\n");
            break;
        case OP_CALC_HEADWIND:
            printf("CALC_HEADWIND\n");
            break;
        case OP_CALC_CROSSWIND:
            printf("CALC_CROSSWIND\n");
            break;
        case OP_CALC_EXPRESSION:
            printf("CALC_EXPRESSION \"%s\"\n", inst->operand.str_operand);
            break;
        case OP_IF:
            printf("IF\n");
            break;
        case OP_ELSE:
            printf("ELSE\n");
            break;
        case OP_ENDIF:
            printf("ENDIF\n");
            break;
        case OP_CONDITION:
            printf("CONDITION \"%s\"\n", inst->operand.str_operand);
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

/* Helper to remove quotes from strings */
static void removeQuotes(airlang_strg str) {
    if (str == NULL || strlen == 0) {
        return;
    }
    int len = (airlang_intg)strlen(str);
    if (str[0] == '"') {
        memmove(str, str + 1, len);
        len--;
    }
    // Remove trailing quote
    if (len > 0 && str[len - 1] == '"') {
        str[len - 1] = '\0';
    }

    /*if (IS_QUOTE(str[0])) {
        memmove(str, str + 1, strlen(str));
        if (IS_QUOTE(str[strlen(str) - 1])) {
            str[strlen(str) - 1] = '\0';
        }
    }*/
}

//specially for date "2025-08-03" not "'2025-08-03'"
static airlang_void removeDateQuotes(airlang_strg str) {
    if (str == NULL) return;

    airlang_char* src = str;
    airlang_char* dst = str;

    while (*src) {
        if (*src != '"' && *src != '\'') {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
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
         
            trimWhitespace(line);

            if (strlen(line) > 0 && line[0] != '^' && line[0] != '%') {

                //Checking block 
                if (strstr(line, "REQUEST") && strstr(line, "FROM")) {
                    generateRequestStatement(line, cg);
                }
                else {

                    // Extract first word to check for keywords
                    airlang_char first_word[64];
                    if (sscanf(line, "%63s", first_word) == 1) {
                        airlang_intg keyword = getKeywordCode(first_word);

                        // CHECK FOR BLOCK STRUCTURE USING KEYWORDS
                        if (isBlockKeyword(keyword)) {
                            if (isBlockStart(line)) {
                                generateBlockStart(line, cg);
                            }
                            else if (isBlockEnd(line)) {
                                generateBlockEnd(line, cg);
                            }
                        }
                        else if (isIfStatement(line)) {
                            generateIfStatement(line, cg);
                        }
                        else if (isElseStatement(line)) {
                            generateElseStatement(cg);
                        }
                        else if (isEndIfStatement(line)) {
                            generateEndIfStatement(cg);
                        }

                        // Generate code based on line type
                        else if (isAssignment(line)) {
                            generateAssignment(line, cg);
                        }
                        else if (isPrintStatement(line)) {
                            generatePrint(line, cg);
                        }
                        else if (isCalculation(line)) {
                            generateCalculation(line, cg);
                        }
                    }
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
        trimWhitespace(print_content);

      //  removeQuotes(print_content);
        // Remove quotes if present
      
        if (strchr(print_content, '+')) {
            emitInstruction(cg, OP_PRINT_INTERPOLATED, 0, print_content);
        }
        else {
            removeQuotes(print_content);
            emitInstruction(cg, OP_LOAD_STR, 0, print_content);
            emitInstruction(cg, OP_PRINT, 0, "");
        }
    }
}



airlang_intg isPrintStatement(const airlang_strg line) {
    return strstr(line, "PRINT") != NULL && strchr(line, '{') != NULL ;
}

airlang_void generateAssignment(const airlang_strg line, Generator* cg) {
    airlang_char var_name[64];
    airlang_char value_str[256];


    if (strchr(line, ':')) {
        // Simple parsing: VarName: Value;
        if (sscanf(line, "%63[^:]: %255[^;]", var_name, value_str) == 2) {
            // Trim spaces
            trimWhitespace(var_name);
            trimWhitespace(value_str);

            if (is_aircraft_identifier(value_str)) {
                emitInstruction(cg, OP_LOAD_STR, 0, value_str);
                emitInstruction(cg, OP_STORE_AIRCRAFT_ID, 0, var_name);
            }
            else if (is_coordinate_format(value_str)) {
                emitInstruction(cg, OP_LOAD_STR, 0, value_str);
                emitInstruction(cg, OP_STORE_COORDS, 0, var_name);

                printf("DEBUG: Parsing coordinates: %s for variable: %s\n", value_str, var_name);


                // Parse and cache coordinates for later distance calculation:
                airlang_doub lat, lon;
                if (parse_coordinates(value_str, &lat, &lon)) {
                    printf("DEBUG: Parsed lat=%.4f, lon=%.4f\n", lat, lon);
                    if (strcmp(var_name, "DepartureCoords") == 0) {
                        generator_dep_lat = lat;
                        generator_dep_lon = lon;
                        coords_initialized |= 1;
                        printf("DEBUG: Set departure coordinates\n");
                    }
                    else if (strcmp(var_name, "ArrivalCoords") == 0) {
                        generator_arr_lat = lat;
                        generator_arr_lon = lon;
                        coords_initialized |= 2;
                    }
                }
            }
            else if (is_date_format(value_str)) {
                //removeDateQuotes(value_str);
                emitInstruction(cg, OP_LOAD_STR, 0, value_str);
                emitInstruction(cg, OP_STORE_DATE, 0, var_name);
            }
            else {
                // Check if value is numeric
                airlang_doub num_val;
                if (sscanf(value_str, "%lf", &num_val) == 1) {
                    // Numeric assignment
                    emitInstruction(cg, OP_LOAD_NUM, num_val, "");
                    //emitInstruction(cg, OP_STORE_VAR, 0, value_str);
                }
                else {

                    removeQuotes(value_str);
                    emitInstruction(cg, OP_LOAD_STR, 0, value_str);
                    // String assignment
                    // Remove quotes if present
                    //if (value_str[0] == '"') {
                      //  value_str++;
                       // airlang_intg len = (airlang_intg)strlen(trimmed_val);
                       // if (len > 0 && trimmed_val[len - 1] == '"') {
                            //trimmed_val[len - 1] = '\0';
                       // }
                }
                // emitInstruction(cg, OP_LOAD_STR, 0, trimmed_val);
                 //emitInstruction(cg, OP_STORE_VAR, 0, trimmed_var);
                emitInstruction(cg, OP_STORE_VAR, 0, var_name);
            }
        }
    }
    else if (strchr(line, '=')) {
        // Equals syntax : VarName = Expression;
        airlang_char* eq_pos = strchr(line, '=');
        airlang_intg name_len = (airlang_intg)(eq_pos - line);

        strncpy(var_name, line, name_len);
        var_name[name_len] = '\0';
        trimWhitespace(var_name);

        airlang_strg expr = eq_pos + 1;
        while (isspace(*expr)) expr++;

        // Remove semicolon
        airlang_char clean_expr[256];
        airlang_intg i = 0;
        while (expr[i] && expr[i] != ';' && i < sizeof(clean_expr) - 1) {
            clean_expr[i] = expr[i];
            i++;
        }
        clean_expr[i] = '\0';

        if (strstr(clean_expr, "AIRPATH")) {
            emitInstruction(cg, OP_CALC_DISTANCE, 0, "");
            emitInstruction(cg, OP_STORE_VAR, 0, var_name);
        }
        else {
            // Use Writer's expression evaluator
            airlang_doub result = evaluate_expression_with_distance(clean_expr);
            emitInstruction(cg, OP_LOAD_NUM, result, "");
            emitInstruction(cg, OP_STORE_VAR, 0, var_name);
        }

    }
}


airlang_intg isAssignment(const airlang_strg line) {
    return strchr(line, ':') != NULL && !strstr(line, "PRINT");
}



airlang_void generateCalculation(const airlang_strg line, Generator* cg) {
    airlang_char var_name[255] = { 0 };
    airlang_char expr[255] = { 0 };

    // Split at '='
    char* eq_pos = strchr(line, '=');
    if (eq_pos == NULL) return;

    // Get variable name
    strncpy(var_name, line, eq_pos - line);
    var_name[eq_pos - line] = '\0';
      trimWhitespace(var_name);

    // Get expression
    strcpy(expr, eq_pos + 1);
    trimWhitespace(expr);

    // Remove semicolon
    airlang_char* semicolon = strchr(expr, ';');
    if (semicolon) *semicolon = '\0';

    // Handle different expression types
    if (strstr(expr, "AIRPATH")) {

        if ((coords_initialized & 3) == 3) { // Both departure and arrival set
            // Use Writer's distance calculation
            airlang_doub distance = calcLastLegDistance();
           // printf("DEBUG: AIRPATH calculated distance: %.2f\n", distance);
            // Use Writer's distance calculation
            //airlang_doub distance = calcLastLegDistance();
           // printf("DEBUG: %.2f,\n", distance);
            emitInstruction(cg, OP_CALC_DISTANCE, distance, "");
        }
        else {
            printf("ERROR: Coordinates not properly initialized for AIRPATH calculation\n");
            printf("coords_initialized = %d (need 3 for both coords)\n", coords_initialized);
            emitInstruction(cg, OP_LOAD_NUM, 0.0, ""); // Default to 0
        }
    }
    else if (strstr(expr, "HEADWIND()")) {
        emitInstruction(cg, OP_CALC_HEADWIND, 0, "");
    }
    else if (strstr(expr, "CROSSWIND()")) {
        emitInstruction(cg, OP_CALC_CROSSWIND, 0, "");
    }
    else if (contains_variables(expr) && (strstr(expr, "+") || strstr(expr, "-") || strstr(expr, "*") || strstr(expr, "/"))) {
      
        emitInstruction(cg, OP_CALC_EXPRESSION, 0, expr);
    }

    else if (strstr(expr, "+") || strstr(expr, "-") || strstr(expr, "*") || strstr(expr, "/")) {
        // Use Writer's expression evaluator
        airlang_doub result = evaluate_expression_with_distance(expr);
        emitInstruction(cg, OP_LOAD_NUM, result, "");
    }
    else {
        // Simple value assignment
        airlang_doub value = atof(expr);
        emitInstruction(cg, OP_LOAD_NUM, value, "");
    }
        emitInstruction(cg, OP_STORE_VAR, 0, var_name);
    
}


airlang_intg isCalculation(const airlang_strg line) {
    return strchr(line, '=') != NULL && !strchr(line, ':');
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

/*
 * Function: isInKeywordArray
 * Purpose: Generic function to check if keyword exists in an array
 */
static airlang_intg isInKeywordArray(airlang_intg keyword, const airlang_intg* array, airlang_intg array_size) {
    for (airlang_intg i = 0; i < array_size; i++) {
        if (array[i] == keyword) {
            return 1; // Found
        }
    }
    return 0; // Not found
}

/*
 * Function: getBlockMapping
 * Purpose: Find block mapping for a keyword
 */
static const BlockMapping* getBlockMapping(airlang_intg keyword) {
    for (airlang_intg i = 0; i < BLOCK_MAPPING_COUNT; i++) {
        if (BLOCK_MAPPINGS[i].keyword == keyword) {
            return &BLOCK_MAPPINGS[i];
        }
    }
    return NULL; // Not found
}

/*
 * Function: getEndKeywordFromStart
 * Purpose: Get the corresponding END keyword for a start keyword
 */
static airlang_intg getEndKeywordFromStart(airlang_intg start_keyword) {
    switch (start_keyword) {
    case KW_MAIN: return KW_ENDMAIN;
    case KW_BRIEFING: return KW_ENDBRIEFING;
    case KW_WEATHER: return KW_ENDWEATHER;
    case KW_LOADSHEET: return KW_ENDLOADSHEET;
    case KW_DISPATCH: return KW_ENDDISPATCH;
    case KW_RECEIVEDDATA: return KW_ENDRECEIVEDDATA;
    case KW_RUNWAYDATA: return KW_ENDRUNWAYDATA;
    case KW_WINDANALYSIS: return KW_ENDWINDANALYSIS;
    case KW_SAFETYALERT: return KW_ENDSAFETYALERT;
    case KW_REPORT: return KW_ENDREPORT;
    default: return NO_ATTR;
    }
}

/*
 * REFACTORED MAIN FUNCTIONS
 */

 /*
  * Function: isBlockKeyword
  * Purpose: Clean check if keyword is block-related (MUCH CLEANER!)
  */
airlang_intg isBlockKeyword(airlang_intg keyword) {
    return (isInKeywordArray(keyword, BLOCK_START_KEYWORDS, BLOCK_START_COUNT) ||
        isInKeywordArray(keyword, BLOCK_END_KEYWORDS, BLOCK_END_COUNT));
}

/*
 * Function: isBlockStart
 * Purpose: Clean check for block start keywords
 */
airlang_intg isBlockStart(const airlang_strg line) {
    airlang_char first_word[64];
    if (sscanf(line, "%63s", first_word) != 1) {
        return 0;
    }

    airlang_intg keyword = getKeywordCode(first_word);
    return isInKeywordArray(keyword, BLOCK_START_KEYWORDS, BLOCK_START_COUNT);
}

/*
 * Function: isBlockEnd
 * Purpose:  check for block end keywords
 */
airlang_intg isBlockEnd(const airlang_strg line) {
    // Create a copy for tokenization
    airlang_char line_copy[1024];
    strncpy(line_copy, line, sizeof(line_copy) - 1);
    line_copy[sizeof(line_copy) - 1] = '\0';

    // Check each word in the line
    airlang_char* token = strtok(line_copy, " \t{}");
    while (token != NULL) {
        airlang_intg keyword = getKeywordCode(token);
        if (isInKeywordArray(keyword, BLOCK_END_KEYWORDS, BLOCK_END_COUNT)) {
            return 1;
        }
        token = strtok(NULL, " \t{}");    // strtok might remove later 
    }
    return 0;
}

/*
 * Function: generateBlockStart
 * Purpose: Generate block start using lookup table
 */
airlang_void generateBlockStart(const airlang_strg line, Generator* cg) {
    airlang_char first_word[64];
    if (sscanf(line, "%63s", first_word) != 1) {
        return;
    }

    airlang_intg keyword = getKeywordCode(first_word);
    const BlockMapping* mapping = getBlockMapping(keyword);

    if (mapping != NULL) {
        emitInstruction(cg, mapping->enter_op, 0, mapping->block_name);
        printf("Generated ENTER_%s block (keyword: %d)\n", mapping->block_name, keyword);
    }
    else {
        printf("Warning: Unknown block start keyword: %s\n", first_word);
    }
}

/*
 * Function: generateBlockEnd
 * Purpose: Generate block end using lookup table
 */
airlang_void generateBlockEnd(const airlang_strg line, Generator* cg) {
    // Create a copy for tokenization
    airlang_char line_copy[1024];
    strncpy(line_copy, line, sizeof(line_copy) - 1);
    line_copy[sizeof(line_copy) - 1] = '\0';

    // Find the END keyword
    airlang_char* token = strtok(line_copy, " \t{}");
    while (token != NULL) {
        airlang_intg keyword = getKeywordCode(token);

        if (isInKeywordArray(keyword, BLOCK_END_KEYWORDS, BLOCK_END_COUNT)) {
            const BlockMapping* mapping = getBlockMapping(keyword);

            if (mapping != NULL) {
                emitInstruction(cg, mapping->exit_op, 0, mapping->block_name);
                printf("Generated EXIT_%s block (keyword: %d)\n", mapping->block_name, keyword);
            }
            else {
                printf("Warning: Unknown block end keyword: %s\n", token);
            }
            break;
        }
        token = strtok(NULL, " \t{}");
    }
}



 
  /*
   * Function: getKeywordCode
   * Purpose: Find keyword in your existing keywordTable[]
   * Returns: The enum index of the keyword, or NO_ATTR if not found
   */
airlang_intg getKeywordCode(const airlang_strg word) {
    for (airlang_intg i = 0; i < KWT_SIZE; i++) {
        if (strcmp(word, keywordTable[i]) == 0) {
            return i; // Returns the enum index now  (KW_MAIN=0, KW_BRIEFING=1, etc.)
        }
    }
    return NO_ATTR; // Not found
}



const airlang_strg getBlockNameFromKeyword(airlang_intg keyword) {
    const BlockMapping* mapping = getBlockMapping(keyword);
    return (mapping != NULL) ? mapping->block_name : "UNKNOWN";
}

/*
 * Function: getBlockOpFromKeyword
 * Purpose: Get opcode using lookup table
 */
OpCode getBlockOpFromKeyword(airlang_intg keyword) {
    const BlockMapping* mapping = getBlockMapping(keyword);
    if (mapping != NULL) {
        // For END keywords, we need to find the corresponding start keyword
        if (isInKeywordArray(keyword, BLOCK_END_KEYWORDS, BLOCK_END_COUNT)) {
            return mapping->exit_op;
        }
        else {
            return mapping->enter_op;
        }
    }
    return OP_ENTER_BLOCK; // Generic fallback
}

airlang_intg contains_variables(const airlang_strg expr) {
    airlang_char temp_expr[256];
    strcpy_s(temp_expr, sizeof(temp_expr), expr);

    // Remove spaces
    airlang_char clean_expr[256] = { 0 };
    airlang_intg i = 0, j = 0;
    while (temp_expr[i]) {
        if (!isspace(temp_expr[i])) {
            clean_expr[j++] = temp_expr[i];
        }
        i++;
    }
    clean_expr[j] = '\0';

    // Split by operators and check each token
    airlang_char* context = NULL;
    airlang_char* token = strtok_s(clean_expr, "+-*/()", &context);

    while (token != NULL) {
        // Skip if it's just a number or negative number
        if (!(isdigit(token[0]) || (token[0] == '-' && isdigit(token[1])))) {
            // Check if it's a variable name (starts with letter)
            if (isalpha(token[0])) {
                return 1; // Found a variable
            }
        }
        token = strtok_s(NULL, "+-*/()", &context);
    }

    return 0; // No variables found
}


airlang_intg isIfStatement(const airlang_strg line) {
    return strstr(line, "IF ") != NULL && strstr(line, " THEN") != NULL;
}

airlang_intg isElseStatement(const airlang_strg line) {
    airlang_char trimmed[1024];
    strcpy(trimmed, line);
    trimWhitespace(trimmed);
    return strcmp(trimmed, "ELSE") == 0;
}

airlang_intg isEndIfStatement(const airlang_strg line) {
    return strstr(line, "ENDIF") != NULL;
}

airlang_void generateIfStatement(const airlang_strg line, Generator* cg) {
    // Extract condition between IF and THEN
    airlang_char* if_pos = strstr(line, "IF ");
    airlang_char* then_pos = strstr(line, " THEN");

    if (if_pos && then_pos && if_pos < then_pos) {
        if_pos += 3; // Skip "IF "
        airlang_intg condition_len = (airlang_intg)(then_pos - if_pos);
        airlang_char condition[256];
        strncpy(condition, if_pos, condition_len);
        condition[condition_len] = '\0';
        trimWhitespace(condition);

        // Just emit the condition - VM will handle evaluation
        emitInstruction(cg, OP_CONDITION, 0, condition);
        emitInstruction(cg, OP_IF, 0, "");
    }
}

airlang_void generateElseStatement(Generator* cg) {
    emitInstruction(cg, OP_ELSE, 0, "");
}

airlang_void generateEndIfStatement(Generator* cg) {
    emitInstruction(cg, OP_ENDIF, 0, "");
}

/*REQUEST statements properly */
airlang_void generateRequestStatement(const airlang_strg line, Generator* cg) {
    airlang_char service_type[64];
    airlang_char url[256];

    // Parse: REQUEST METAR FROM "https://airlangMetar.fly"
    if (sscanf(line, "REQUEST %63s FROM %255s", service_type, url) == 2) {
        // Remove quotes and semicolon from URL
        removeQuotes(url);

        // Remove semicolon if present
        int len = (airlang_intg)strlen(url);
        if (len > 0 && url[len - 1] == ';') {
            url[len - 1] = '\0';
        }

        // Generate proper instructions
        emitInstruction(cg, OP_LOAD_STR, 0, url);

        // Create proper variable name
        airlang_char var_name[128];
        snprintf(var_name, sizeof(var_name), "REQUEST_%s_FROM", service_type);
        emitInstruction(cg, OP_STORE_VAR, 0, var_name);
    }
}