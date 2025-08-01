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
    if (IS_QUOTE(str[0])) {
        memmove(str, str + 1, strlen(str));
        if (IS_QUOTE(str[strlen(str) - 1])) {
            str[strlen(str) - 1] = '\0';
        }
    }
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

                // Extract first word to check for keywords
                airlang_char first_word[64];
                sscanf(line, "%63s", first_word);
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

        removeQuotes(print_content);
        // Remove quotes if present
      

        emitInstruction(cg, OP_LOAD_STR, 0, print_content);
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
        trimWhitespace(var_name);
        trimWhitespace(value_str);

       
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



airlang_intg isAssignment(const airlang_strg line) {
    return strchr(line, ':') != NULL && !strstr(line, "PRINT");
}

/*airlang_void generateCalculation(const airlang_strg line, Generator* cg) {
    // For now, just emit a comment instruction
    airlang_char var_name[64] = { 0 };
    airlang_char operand1_str[64] = { 0 };
    airlang_char operand2_str[64] = { 0 };

    // Find '=' character
    const char* eq_pos = strchr(line, '=');
    if (!eq_pos) return;

    // Extract variable name (left side)  Chcek 7 + 7 ; 
    airlang_intg var_len = (airlang_intg)(eq_pos - line);


    // Trim trailing whitespace from variable name
    while (var_len > 0 && (line[var_len - 1] == ' ' || line[var_len - 1] == '\t')) var_len--;
    strncpy(var_name, line, var_len);
    var_name[var_len] = '\0';



    // Pointer to right side (expression)
    const char* rhs = eq_pos + 1;
    // Skip leading whitespace
    while (*rhs == ' ' || *rhs == '\t') rhs++;



    // Parse first operand (number) until space or '+' PLUS_T
    airlang_intg pos = 0;
    while (rhs[pos] && rhs[pos] != ' ' && rhs[pos] != '\t' && rhs[pos] != '+') {
        operand1_str[pos] = rhs[pos];
        pos++;
        if (pos >= 63) break;  // safety check
    }
    operand1_str[pos] = '\0';

    // Skip spaces after first operand
    while (rhs[pos] == ' ' || rhs[pos] == '\t') pos++;



    // Next character must be '+'
    if (rhs[pos] != '+') return;
    pos++; // skip '+'

    // Skip spaces after '+'
    while (rhs[pos] == ' ' || rhs[pos] == '\t') pos++;

    // Parse second operand (number) until space or end
    airlang_intg pos2 = 0;
    while (rhs[pos] && rhs[pos] != ' ' && rhs[pos] != '\t' && rhs[pos] != ';') {
        operand2_str[pos2] = rhs[pos];
        pos2++;
        pos++;
        if (pos2 >= 63) break;  // safety check
    }
    operand2_str[pos2] = '\0';

    // Convert operands to double
    double op1 = atof(operand1_str);
    double op2 = atof(operand2_str);

    // Emit instructions: LOAD_NUM op1, LOAD_NUM op2, ADD, STORE_VAR var_name
    emitInstruction(cg, OP_LOAD_NUM, op1, "");
    emitInstruction(cg, OP_LOAD_NUM, op2, "");
    emitInstruction(cg, ARTHOP_ADD, 0, "");         // this fixed forr now just to check 7 + 7 
    emitInstruction(cg, OP_STORE_VAR, 0, var_name);

}*/


airlang_void generateCalculation(const airlang_strg line, Generator* cg) {
    airlang_char var_name[255] = { 0 };
    airlang_char expr[255] = { 0 };

    // Split at '='
    char* eq_pos = strchr(line, '=');
    if (!eq_pos) return;

    // Get variable name
    strncpy(var_name, line, eq_pos - line);
      trimWhitespace(var_name);

    // Get expression
    strcpy(expr, eq_pos + 1);
    trimWhitespace(expr);

    // Simple support for addition only (as per original)
    airlang_doub op1, op2;
    if (sscanf(expr, "%lf + %lf", &op1, &op2) == 2) {
        emitInstruction(cg, OP_LOAD_NUM, op1, "");
        emitInstruction(cg, OP_LOAD_NUM, op2, "");
        emitInstruction(cg, ARTHOP_ADD, 0, "");
        emitInstruction(cg, OP_STORE_VAR, 0, var_name);
    }
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
 * Purpose: Clean check for block end keywords
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
        token = strtok(NULL, " \t{}");
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
 * LEGACY FUNCTIONS - NOW MUCH SIMPLER
 */

 /*
  * Function: getBlockNameFromKeyword
  * Purpose: Get block name using lookup table
  */
  /*
   * Function: getKeywordCode
   * Purpose: Find keyword in your existing keywordTable[]
   * Returns: The enum index of the keyword, or NO_ATTR if not found
   */
airlang_intg getKeywordCode(const airlang_strg word) {
    for (airlang_intg i = 0; i < KWT_SIZE; i++) {
        if (strcmp(word, keywordTable[i]) == 0) {
            return i; // Returns the enum index (KW_MAIN=0, KW_BRIEFING=1, etc.)
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