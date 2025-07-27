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
# ECHO "[WRITER SCRIPT .........................]"
# ECHO "                                         "
*/

/*
***********************************************************
* File name: Writer.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */

#ifndef CODER_H_
#include "Step5Writer.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Global variables */
Variable variables[MAX_VARS];
airlang_intg var_count = 0;
airlang_intg initial_phase = 1; // Flag to track the initial phase
airlang_char output_buffer[MAX_EXPR_LEN * 10] = { 0 }; // Buffer to store write output

//for if else 
airlang_intg if_condition_result = 0;
airlang_intg in_if_block = 0;
airlang_intg in_else_block = 0;
airlang_intg skip_execution = 0;

//AIRPORT CODES
airlang_char current_airport[16] = { 0 };

/* Finds variables */
airlang_intg find_variable(const airlang_strg name) {
    airlang_intg i = 0;
    for (i = 0; i < var_count; ++i) {
        if (strcmp(variables[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}



airlang_void assign_string_variable(const airlang_strg name, const airlang_strg value) {
    airlang_intg idx = find_variable(name);
    if (idx == -1) {
        if (var_count >= MAX_VARS) return; // No space
        idx = var_count++;

        // Copy name safely
        size_t i;
        for (i = 0; name[i] && i < sizeof(variables[idx].name) - 1; i++) {
            variables[idx].name[i] = name[i];
        }
        variables[idx].name[i] = '\0';
    }

    variables[idx].type = STRING;

    // Copy value safely
    size_t i;
    for (i = 0; value[i] && i < sizeof(variables[idx].value.str_value) - 1; i++) {
        variables[idx].value.str_value[i] = value[i];
    }
    variables[idx].value.str_value[i] = '\0';
}
/* Gets string variable */
const airlang_strg get_string_value(const airlang_strg name) {
    airlang_intg idx = find_variable(name);
    if (idx != -1 && variables[idx].type == STRING) {
        return variables[idx].value.str_value;
    }
    return STREMPTY;
}

/* Write output */

void safe_concat(char* dest, size_t dest_size, const char* src) {
    // Find current end of destination string
    size_t dest_len = strlen(dest);

    // Calculate remaining space (-1 for null terminator)
    size_t remaining = dest_size - dest_len - 1;

    // Copy only what fits
    for (size_t i = 0; i < remaining && src[i] != '\0'; i++) {
        dest[dest_len + i] = src[i];
    }

    // Always null-terminate
    dest[dest_size - 1] = '\0';
}

airlang_void handle_write(airlang_strg expression) {
    airlang_char buffer[MAX_EXPR_LEN] = { 0 };
    size_t buf_pos = 0;

    // Trim input first
    trimWhitespace(expression);

    // Find the content between parentheses
    airlang_strg start = strchr(expression, '{');
    airlang_strg end = strrchr(expression, '}');

    if (!start || !end || start >= end) return;

    start++; // Move past '('

    // Check if it's an empty string between quotes
    while (*start && isspace(*start)) start++; // Skip whitespace
    if (*start == '"' && *(start + 1) == '"') {
        // Empty string - just add newline
        buffer[0] = '\0';
    }
    else {


        while (*start && *start != '}' && buf_pos < MAX_EXPR_LEN - 1) {
            if (*start == '"') {
                // Handle quoted strings
                start++;
                while (*start != '"' && *start && buf_pos < MAX_EXPR_LEN - 1) {
                    buffer[buf_pos++] = *start++;
                }
                if (*start == '"') start++;
            }
            else if (isalpha(*start)) {
                // Handle variables
                airlang_char var_name[32] = { 0 };
                size_t var_pos = 0;

                // Extract variable name
                while ((isalnum(*start) || *start == '_') && var_pos < sizeof(var_name) - 1) {
                    var_name[var_pos++] = *start++;
                }
                var_name[var_pos] = '\0';

                // Get variable value
                airlang_intg idx = find_variable(var_name);
                if (idx != -1) {
                    const char* value = NULL;
                    airlang_char num_str[32];

                    if (variables[idx].type == STRING) {
                        value = variables[idx].value.str_value;
                    }
                    else if (variables[idx].type == NUMERIC) {
                        if (variables[idx].value.num_value == (int)variables[idx].value.num_value) {
                            snprintf(num_str, sizeof(num_str), "%.0f", variables[idx].value.num_value);
                        }
                        else {
                            snprintf(num_str, sizeof(num_str), "%.2f", variables[idx].value.num_value);
                        }
                        value = num_str;
                    }

                    // Append value to buffer
                    if (value) {
                        while (*value && buf_pos < MAX_EXPR_LEN - 1) {
                            buffer[buf_pos++] = *value++;
                        }
                    }
                }
            }
            else if (isspace(*start)) {
                // Preserve whitespace
                if (buf_pos < MAX_EXPR_LEN - 1) {
                    buffer[buf_pos++] = *start++;
                }
            }
            else {
                if (*start == '+') {
                    start++; // Skip the + and continue processing
                    // Skip any whitespace after +
                    while (*start && isspace(*start)) start++;
                }
                else {
                    start++; // Skip other characters
                }
            }
        }
    }

    buffer[buf_pos] = '\0'; // Ensure null termination

    // Handle output
    if (initial_phase) {
        // Simple append to output_buffer with newline
        size_t out_len = strlen(output_buffer);
        size_t buf_len = strlen(buffer);

        if (out_len + buf_len + 2 < sizeof(output_buffer)) { // +2 for \n and \0
            memcpy(output_buffer + out_len, buffer, buf_len);
            output_buffer[out_len + buf_len] = '\n';
            output_buffer[out_len + buf_len + 1] = '\0';
        }
    }
    else {
        printf("%s\n", buffer);
    }
}





/* Calculate expression */
airlang_void calculate(airlang_strg expression) {

    trimWhitespace(expression);

    if (!expression || strlen(expression) == 0 || expression[0] == '%') {
        return;
    }

    //wind

   // block to handle^^ comments
        if (strstr(expression, "^^") != NULL) {
            return; // Skip comment lines
        }

        if (validateRequestStatement(expression)) {
            handleRequestStatement(expression);
            return;
        }

        if (strstr(expression, "METAR") != NULL && strchr(expression, ':') != NULL) {
            handle_metar_assignment(expression);
            return;
        }
    // First check if it's an IF-ELSE statement
    if (handle_if_else(expression)) {
        return; // IF-ELSE handled it
    }

    // Skip execution if we're in a block that shouldn't execute
    if (skip_execution) {
       // printf("DEBUG: Skipping execution of: %s\n", expression);
        return;
    }
    airlang_char var_name[32] = { 0 };

    // Handle PRINT statements
    if (strstr(expression, "PRINT")) {
        handle_write(expression);
        return;
    }

    // Handle colon syntax: VariableName: value;
    if (strchr(expression, ':')) {
        // Find the colon
        airlang_char* colon_pos = strchr(expression, ':');

        // Get variable name (everything before colon)
        airlang_intg name_len = (airlang_intg)(colon_pos - expression);
        strncpy_s(var_name, sizeof(var_name), expression, name_len);
        var_name[name_len] = EOS;

        // Remove spaces from variable name
        airlang_char clean_name[32] = { 0 };
        airlang_intg i = 0, j = 0;
        while (var_name[i]) {
            if (!isspace(var_name[i])) {
                clean_name[j++] = var_name[i];
            }
            i++;
        }
        clean_name[j] = EOS;

        // Get value (everything after colon)
        airlang_strg value_str = colon_pos + 1;

        // Remove spaces and semicolon
        while (isspace(*value_str)) value_str++;

        // Check if this is a quoted string
        if (*value_str == QUOTES) {
            // Handle quoted strings
            value_str++; // Skip opening quote
            airlang_char string_value[256] = { 0 };
            airlang_intg i = 0;
            while (*value_str != QUOTES && *value_str != EOS && *value_str != ';') {
                string_value[i++] = *value_str++;
            }
            string_value[i] = EOS;
            assign_string_variable(clean_name, string_value);
        }
        else if (strchr(value_str, ',')) {


            // Remove semicolon if present
            airlang_char coord_value[64] = { 0 };
            i = 0;
            while (value_str[i] && value_str[i] != ';') {
                if (!isspace(value_str[i])) {
                    coord_value[strlen(coord_value)] = value_str[i];
                }
                i++;
            }

            // Remove trailing spaces
            while (strlen(coord_value) > 0 && isspace(coord_value[strlen(coord_value) - 1])) {
                coord_value[strlen(coord_value) - 1] = '\0';
            }
            // Check if it's a valid coordinate format
            if (is_coordinate_format(coord_value)) {
                assign_string_variable(clean_name, coord_value);
            }
            else {
                // Handle as regular comma-separated string (for other uses)
                assign_string_variable(clean_name, coord_value);
            }
        }
        else {
                // Handle single numeric values
                airlang_char value_clean[64] = { 0 };
                i = 0;
                while (value_str[i] && value_str[i] != ';') {
                    if (!isspace(value_str[i])) {
                        value_clean[strlen(value_clean)] = value_str[i];
                    }
                    i++;
                }


                if (is_date_format(value_clean)) {
                    // Remove quotes and store as string
                    airlang_char date_without_quotes[32] = { 0 };
                    strncpy_s(date_without_quotes, sizeof(date_without_quotes), value_clean + 1, strlen(value_clean) - 2);
                    assign_string_variable(clean_name, date_without_quotes);
                }
                // Check if it's a flight number pattern IATA , OR AIRCRAFT REGISTRATION ICAO
                else if (is_aircraft_identifier(value_clean)) {
                    assign_string_variable(clean_name, value_clean);
                }
                else {
                    // Convert to number and assign
                    airlang_doub num_value = atof(value_clean);
                    assign_numeric_variable(clean_name, num_value);
                }
            }
        
    }
    else if (strchr(expression, EQUALS)) {

        extract_airport_from_assignment(expression, current_airport);
        airlang_char var_name[32] = { 0 };


        sscanf_s(expression, "%31s =", var_name, (unsigned)_countof(var_name));
        airlang_strg expr = strchr(expression, EQUALS) + 1;
        while (isspace(*expr)) expr++;

        // Remove semicolon if present
        airlang_char clean_expr[256] = { 0 };
        airlang_intg i = 0;
        while (expr[i] && expr[i] != ';') {
            clean_expr[i] = expr[i];
            i++;
        }
        clean_expr[i] = EOS;


        if (*expr == QUOTES) {
            expr++;
            airlang_char str_value[256] = { 0 };
            airlang_intg i = 0;
            while (*expr != QUOTES && *expr != EOS) {
                str_value[i++] = *expr++;
            }
            assign_string_variable(var_name, str_value);
            if (!initial_phase) {
                printf("%s = \"%s\"\n", var_name, str_value);
            }
        }
    
        else {
        // Handle arithmetic expression
            airlang_doub result = evaluate_expression_with_distance(clean_expr);
            assign_numeric_variable(var_name, result);
            if (!initial_phase) {
                printf("DEBUG: Assigned %s = %.2f\n", var_name, result);
            if (result == (airlang_intg)result) {
                printf("%s = %.0lf\n", var_name, result);
            }
            else {
                printf("%s = %.2lf\n", var_name, result);
                }
            }

            // Create proper variable name
            if (strstr(clean_expr, "HEADWIND")) {
            airlang_char full_name[32];
            snprintf(full_name, sizeof(full_name), "%s_HEADWIND", var_name);
            assign_numeric_variable(full_name, result);
            assign_numeric_variable(var_name, result);  // Also assign to base variable
            }
            else if (strstr(clean_expr, "CROSSWIND")) {
            airlang_char full_name[32];
            snprintf(full_name, sizeof(full_name), "%s_CROSSWIND", var_name);
            assign_numeric_variable(full_name, result);
           // assign_numeric_variable(var_name, result);  // Also assign to base variable
             }
       
            //assign_numeric_variable(var_name, result);
        
            //assign_numeric_variable(var_name, result);
       
        }
    }
    else if (strstr(expression, WRITE)) {
        handle_write(expression);
    }
}


void trimWhitespace(char* str) {
    if (!str || *str == '\0') return;

    char* start = str;
    char* end;
    size_t original_len = strlen(str);

    // Find first non-whitespace character
    while (isspace((unsigned char)*start)) {
        start++;
    }

    // If string is all whitespace
    if (*start == '\0') {
        *str = '\0';
        return;
    }

    // Find last non-whitespace character
    end = str + original_len - 1;
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }

    // Calculate length of trimmed string
    size_t trimmed_len = end - start + 1;

    // Safety check to prevent buffer overflow
    if (trimmed_len >= original_len + 1) {
        return; // Something went wrong, don't modify
    }

    // Move the trimmed content to the beginning of the buffer
    if (start != str) {
        memmove(str, start, trimmed_len);
    }

    // Null terminate
    str[trimmed_len] = '\0';
}

/* Process input file */
airlang_void process_file(const airlang_strg filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    airlang_char line[MAX_EXPR_LEN];
    while (fgets(line, sizeof(line), file)) {

        // Trim whitespace first
        trimWhitespace(line);

        if (line[0] == NEWLINE || line[0] == RETURN) {
            continue; // Skip empty lines
        }
        line[strcspn(line, "\n")] = 0; // Remove newline character
        calculate(line);
    }
    fclose(file);
    initial_phase = 0; // End of initial phase
    printf("%s", output_buffer); // Print the buffered write output
    printf("\nVariable values:\n");
    airlang_intg i = 0;
    for (i = 0; i < var_count; i++) {
        if (variables[i].type == STRING) {
            printf("%s = \"%s\"\n", variables[i].name, variables[i].value.str_value);
        }
        else if (variables[i].type == NUMERIC) {
            //printf("%s = %.2lf\n", variables[i].name, variables[i].value.num_value);
            if (variables[i].value.num_value == (airlang_intg)variables[i].value.num_value) {
                printf("%s = %.0lf\n", variables[i].name, variables[i].value.num_value);
            }
            else {
                printf("%s = %.2lf\n", variables[i].name, variables[i].value.num_value);
            }
        }
        else if (variables[i].type == BOOLEAN) {
            printf("%s = %s\n", variables[i].name, variables[i].value.bool_value ? "true" : "false");
        }
        else if (variables[i].type == CHAR) {
            printf("%s = '%c'\n", variables[i].name, variables[i].value.char_value);
        }
    }
}

/* Split lines into components */
airlang_strg* splitIntoLines(const airlang_strg content, airlang_intg* lineCount) {
    airlang_strg* lines = malloc(MAX_LINES * sizeof(char*));
    if (!lines) {
        perror("Error allocating memory");
        return NULL;
    }
    *lineCount = 0;
    const airlang_char* start = content;
    const airlang_char* end;
    while ((end = strchr(start, NEWLINE)) != NULL) {
        if (*lineCount >= MAX_LINES) {
            fprintf(stderr, "Exceeded maximum number of lines\n");
            break;
        }
        airlang_intg lineLength = (airlang_intg) (end - start);
        lines[*lineCount] = malloc(lineLength + 1);
        if (!lines[*lineCount]) {
            perror("Error allocating memory for line");
            break;
        }
        // Use strncpy_s for safer copying
        if (strncpy_s(lines[*lineCount], lineLength + 1, start, lineLength) != 0) {
            perror("Error copying string using strncpy_s");
            free(lines[*lineCount]);
            break;
        }
        lines[*lineCount][lineLength] = EOS; // Null-terminate the string
        (*lineCount)++;
        start = end + 1;
    }
    // Handle the last line if it doesn't end with a newline
    if (*start != EOS && *lineCount < MAX_LINES) {
        size_t lineLength = strlen(start);
        lines[*lineCount] = malloc(lineLength + 1);
        if (!lines[*lineCount]) {
            perror("Error allocating memory for line");
        }
        else {
            // Use _strdup to copy the remaining content
            lines[*lineCount] = _strdup(start);
            if (!lines[*lineCount]) {
                perror("Error copying string using _strdup");
            }
            else {
                (*lineCount)++;
            }
        }
    }
    return lines;
}

/* Free lines */
airlang_void freeLines(airlang_strg* lines, airlang_intg lineCount) {
    airlang_intg i = 0;
    for (i = 0; i < lineCount; i++) {
        free(lines[i]);
    }
    free(lines);
}

/* Process content */
airlang_void process_content(airlang_strg fileContent) {
    airlang_intg lineCount = 0;
    airlang_strg* lines = splitIntoLines(fileContent, &lineCount);
   // airlang_strg line = malloc(MAX_EXPR_LEN);
    if (!lines ) {
        return;
    }
    printf("Lines from content:\n");
    airlang_intg i = 0;
	for (i = 0; i < lineCount; i++) {
       // printf("DEBUG: Line %d length: %d\n", i, (int)strlen(lines[i]));
       // printf("DEBUG: Line %d content: '%s'\n", i, lines[i]);
       // printf("DEBUG: First 10 chars as hex: ");
        for (int j = 0; j < 10 && lines[i][j]; j++) {
           // printf("%02x ", (unsigned char)lines[i][j]);
        }
       // printf("\n");

		//line = lines[i];
       // printf("DEBUG: Processing line: '%s'\n", lines[i]);
         // TRIM WHITESPACE BEFORE PROCESSING
        trimWhitespace(lines[i]);

       // printf("DEBUG: After trim: '%s'\n", lines[i]);
		calculate(lines[i]);
	}
    initial_phase = 0; // End of initial phase
    printf("%s", output_buffer); // Print the buffered write output
    printf("\nVariable values:\n");
    for (i = 0; i < var_count; i++) {
        if (variables[i].type == STRING) {
            printf("%s = \"%s\"\n", variables[i].name, variables[i].value.str_value);
        }
        else if (variables[i].type == NUMERIC) {
            //printf("%s = %.2lf\n", variables[i].name, variables[i].value.num_value);
            if (variables[i].value.num_value == (airlang_intg)variables[i].value.num_value) {
                printf("%s = %.0lf\n", variables[i].name, variables[i].value.num_value);
            }
            else {
                printf("%s = %.2lf\n", variables[i].name, variables[i].value.num_value);
            }
        }
        else if (variables[i].type == BOOLEAN) {
            printf("%s = %s\n", variables[i].name, variables[i].value.bool_value ? "true" : "false");
        }
        else if (variables[i].type == CHAR) {
            printf("%s = '%c'\n", variables[i].name, variables[i].value.char_value);
        }
    }
    freeLines(lines, lineCount);
}


airlang_void assign_numeric_variable(const airlang_strg name, airlang_doub value) {
    airlang_intg idx = find_variable(name);
    if (idx == -1) {
        idx = var_count++;
        strcpy_s(variables[idx].name, sizeof(variables[idx].name), name);
    }
    variables[idx].type = NUMERIC;
    variables[idx].value.num_value = value;
}


airlang_doub evaluate_expression(const airlang_strg expr) {
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
    clean_expr[j] = EOS;

    airlang_char op_expr[256];
    strcpy_s(op_expr, sizeof(op_expr), clean_expr);

    // Extract operators first
    airlang_char operators[10] = { 0 };
    airlang_intg op_count = 0;
    for (i = 0; op_expr[i]; i++) {
        if (op_expr[i] == '+' || op_expr[i] == '-' ||
            op_expr[i] == '*' || op_expr[i] == '/') {
            operators[op_count++] = op_expr[i];
        }
    }

    // Simple parser for expressions like: var1 * 200 + var2

    airlang_char* context = NULL;
    airlang_char* token = strtok_s(clean_expr, "+-*/", &context);
    airlang_doub operands[10];
    airlang_intg operand_count = 0;

    // Extract operators
    while (token != NULL && operand_count < 10) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            // It's a number
            operands[operand_count++] = atof(token);
        }
        else {
            // It's a variable
            airlang_intg var_idx = find_variable(token);
            if (var_idx != -1 && variables[var_idx].type == NUMERIC) {
                operands[operand_count++] = variables[var_idx].value.num_value;
            }
            else {
                operands[operand_count++] = 0.0; // Default value if variable not found
            }
        }
        token = strtok_s(NULL, "+-*/", &context);
    }

    // Evaluate with proper operator precedence (* and / before + and -)
    airlang_doub result = 0.0;
    if (operand_count > 0) {
        // First pass: handle multiplication and division
        for (i = 0; i < op_count; i++) {
            if (operators[i] == '*') {
                operands[i + 1] = operands[i] * operands[i + 1];
                operands[i] = 0; // Mark as processed
                operators[i] = '+'; // Change to addition for next pass
            }
            else if (operators[i] == '/') {
                if (operands[i + 1] != 0) {
                    operands[i + 1] = operands[i] / operands[i + 1];
                }
                operands[i] = 0; // Mark as processed
                operators[i] = '+'; // Change to addition for next pass
            }
        }

        // Second pass: handle addition and subtraction
        result = operands[0];
        for (i = 0; i < op_count; i++) {
            if (operators[i] == '+') {
                result += operands[i + 1];
            }
            else if (operators[i] == '-') {
                result -= operands[i + 1];
            }
        }
    }

    return result;
}

airlang_intg evaluate_condition(const airlang_strg condition) {
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
    cleanCond[j] = EOS;

    // Find comparison operator
    airlang_char* operatorPos = NULL;
    if ((operatorPos = strstr(cleanCond, ">")) != NULL) {
        // Split left and right
        *operatorPos = '\0';
        airlang_char* leftOperand = cleanCond;
        airlang_char* rightOperand = operatorPos + 1;

        // Get left value
        airlang_doub leftVal = 0.0;
        if (isdigit(leftOperand[0])) {
            leftVal = atof(leftOperand);
        }
        else {
            airlang_intg var_idx = find_variable(leftOperand);
            if (var_idx != -1 && variables[var_idx].type == NUMERIC) {
                leftVal = variables[var_idx].value.num_value;
            }
        }

        // Get right value
        airlang_doub rightVal = 0.0;
        if (isdigit(rightOperand[0])) {
            rightVal = atof(rightOperand);
        }
        else {
            airlang_intg var_idx = find_variable(rightOperand);
            if (var_idx != -1 && variables[var_idx].type == NUMERIC) {
                rightVal = variables[var_idx].value.num_value;
            }
        }

       // printf("DEBUG: Comparing %.2f > %.2f = %s\n", leftVal, rightVal, (leftVal > rightVal) ? "TRUE" : "FALSE");
        return leftVal > rightVal;
    }

    return 0;
}

airlang_intg handle_if_else(airlang_strg expression) {
    // Trim leading whitespace
    while (*expression && isspace(*expression)) {
        expression++;
    }

    // Handle IF statements
    if (strncmp(expression, "IF ", 3) == 0) {
        airlang_char* then_pos = strstr(expression, " THEN");
        if (then_pos != NULL) {
            // Extract condition
            airlang_intg cond_len = (airlang_intg)(then_pos - expression - 3);
            airlang_char condition[256] = { 0 };
            strncpy_s(condition, sizeof(condition), expression + 3, cond_len);

            if_condition_result = evaluate_condition(condition);
            in_if_block = 1;
            skip_execution = !if_condition_result;

            //printf("DEBUG: IF condition evaluated to %s\n", if_condition_result ? "TRUE" : "FALSE");
        }
        return 1;
    }

    // Handle ELSE
    if (strncmp(expression, "ELSE", 4) == 0) {
        skip_execution = if_condition_result;
        //printf("DEBUG: ELSE block, skip = %s\n", skip_execution ? "TRUE" : "FALSE");
        return 1;
    }

    // Handle ENDIF
    if (strncmp(expression, "ENDIF", 5) == 0) {
        skip_execution = 0;
        if_condition_result = 0;
        in_if_block = 0;
       // printf("DEBUG: ENDIF reached\n");
        return 1;
    }

    return 0;
}


/*airlang_intg is_flight_number(const airlang_strg value) {
    if (strlen(value) != 5) {
        return 0;
    }
    if (!isalpha(value[0]) || !isalpha(value[1])) {
        return 0;
    }
    if (!isdigit(value[2]) || !isdigit(value[3]) || !isdigit(value[4])) {
        return 0;
    }

    return 1;
}
*/
airlang_intg is_aircraft_identifier(const airlang_strg value) {
    airlang_intg len = (airlang_intg)strlen(value);

    // Flight Id pattern: IATA STANDARD - 2 letters + 3 digits (e.g., AL123)
    if (len == 5) {
        if (isalpha(value[0]) && isalpha(value[1]) &&
            isdigit(value[2]) && isdigit(value[3]) && isdigit(value[4])) {
            return 1;
        }
    }

    // Aircraft ID pattern: ICAO STANDARD - 1 letter + dash + 4 alphanumeric (e.g., C-GNBL)
    if (len == 6) {
        if (isalpha(value[0]) && value[1] == '-' &&
            isalnum(value[2]) && isalnum(value[3]) &&
            isalnum(value[4]) && isalnum(value[5])) {
            return 1;
        }
    }

    return 0;
}

airlang_intg is_date_format(const airlang_strg value) {
    airlang_intg len = (airlang_intg)strlen(value);

    // Date with single quotes: 'YYYY-MM-DD' (e.g., '2025-05-15')
    if (len == 12) {
        if (value[0] == '\'' && value[11] == '\'' &&
            isdigit(value[1]) && isdigit(value[2]) && isdigit(value[3]) && isdigit(value[4]) &&
            value[5] == '-' &&
            isdigit(value[6]) && isdigit(value[7]) &&
            value[8] == '-' &&
            isdigit(value[9]) && isdigit(value[10])) {
            return 1;
        }
    }

    return 0;
}

airlang_intg is_coordinate_format(const airlang_strg value) {
    // Check if the string contains a comma
    if (strchr(value, ',') == NULL) {
        return 0;
    }

    // Make a copy to work with
    airlang_char temp[128];
    strcpy_s(temp, sizeof(temp), value);

    // Find the comma
    airlang_char* comma_pos = strchr(temp, ',');
    if (comma_pos == NULL) {
        return 0;
    }

    // Split at comma
    *comma_pos = '\0';
    airlang_char* lat_str = temp;
    airlang_char* lon_str = comma_pos + 1;

    // Remove leading/trailing spaces
    while (isspace(*lat_str)) lat_str++;
    while (isspace(*lon_str)) lon_str++;

    // Check if both parts are numeric (can be negative)
    // Check latitude part
    airlang_char* lat_ptr = lat_str;
    if (*lat_ptr == '-') lat_ptr++; // Skip negative sign
    if (!isdigit(*lat_ptr)) return 0;
    while (*lat_ptr && (isdigit(*lat_ptr) || *lat_ptr == '.')) lat_ptr++;
    if (*lat_ptr != '\0' && !isspace(*lat_ptr)) return 0;

    // Check longitude part  
    airlang_char* lon_ptr = lon_str;
    if (*lon_ptr == '-') lon_ptr++; // Skip negative sign
    if (!isdigit(*lon_ptr)) return 0;
    while (*lon_ptr && (isdigit(*lon_ptr) || *lon_ptr == '.')) lon_ptr++;
    if (*lon_ptr != '\0' && !isspace(*lon_ptr)) return 0;

    return 1; // Valid coordinate format
}

// Function to parse coordinates and extract latitude and longitude
airlang_intg parse_coordinates(const airlang_strg coord_str, airlang_doub* lat, airlang_doub* lon) {
    if (!is_coordinate_format(coord_str)) {
        return 0;
    }

    airlang_char temp_str[128];
    strcpy_s(temp_str, sizeof(temp_str), coord_str);

    // Find the comma
    airlang_char* comma_pos = strchr(temp_str, ',');
    if (comma_pos == NULL) {
        return 0;
    }

    // Split at comma
    *comma_pos = '\0';
    airlang_char* lat_str = temp_str;
    airlang_char* lon_str = comma_pos + 1;

    // Remove leading/trailing spaces
    while (isspace(*lat_str)) lat_str++;
    while (isspace(*lon_str)) lon_str++;

    // Convert to double
    *lat = atof(lat_str);
    *lon = atof(lon_str);

    return 1; // Success
}



//DISTANCEEE



airlang_doub my_sin(airlang_doub x) {
    airlang_doub x3 = x * x * x;
    airlang_doub x5 = x3 * x * x;
    airlang_doub x7 = x5 * x * x;
    return x - x3 / 6.0 + x5 / 120.0 - x7 / 5040.0;
}

airlang_doub my_cos(airlang_doub x) {
    airlang_doub x2 = x * x;
    airlang_doub x4 = x2 * x2;
    airlang_doub x6 = x4 * x2;
    return 1 - x2 / 2.0 + x4 / 24.0 - x6 / 720.0;
}

airlang_doub my_sqrt(airlang_doub n) {
    if (n < 0) return -1;
    if (n == 0) return 0;
    airlang_doub x = n;
    airlang_intg i = 0;
    for (i = 0; i < 10; i++) {
        x = 0.5 * (x + n / x);
    }
    return x;
}

airlang_doub my_atan2(airlang_doub y, airlang_doub x) {
    if (x > 0) return y / x;
    else if (x < 0 && y >= 0) return PI + y / x;
    else if (x < 0 && y < 0) return -PI + y / x;
    else if (x == 0 && y > 0) return PI / 2;
    else if (x == 0 && y < 0) return -PI / 2;
    else return 0;
}

airlang_doub toRadians(airlang_doub degree) {
    return degree * (PI / 180.0);
}

// Haversine formula - exactly like your C code
    airlang_doub calculateAirpath(airlang_doub lat1, airlang_doub lon1, airlang_doub lat2, airlang_doub lon2) {
        airlang_doub dlat = toRadians(lat2 - lat1);
        airlang_doub dlon = toRadians(lon2 - lon1);
        airlang_doub rlat1 = toRadians(lat1);
        airlang_doub rlat2 = toRadians(lat2);

        airlang_doub sin_dlat2 = my_sin(dlat / 2.0);
        airlang_doub sin_dlon2 = my_sin(dlon / 2.0);
        airlang_doub a = sin_dlat2 * sin_dlat2 +
            my_cos(rlat1) * my_cos(rlat2) *
            sin_dlon2 * sin_dlon2;
        airlang_doub c = 2 * my_atan2(my_sqrt(a), my_sqrt(1 - a));

        airlang_doub distance_km = EARTH_RADIUS_KM * c;
        airlang_doub distance_nm = distance_km * km_to_nm;

        return distance_nm;
}





//DISTNCE BETWEEN LAST TWO CORRDINATES 
airlang_doub calcLastLegDistance() {
    // Find the last two coordinate variables automatically
    airlang_intg coord_count = 0;
    airlang_intg coord_indices[10]; // Store up to 10 coordinate variables

    // Scan through all variables to find coordinate strings
    airlang_intg i = 0;
    for (i = 0; i < var_count; i++) {
        if (variables[i].type == STRING && is_coordinate_format(variables[i].value.str_value)) {
            coord_indices[coord_count] = i;
            coord_count++;
            if (coord_count >= 10) break; // Safety limit
        }
    }

   // printf("DEBUG: Found %d coordinate variables\n", coord_count);

    // Need at least 2 coordinate variables
    if (coord_count < 2) {
       // printf("DEBUG: Not enough coordinate variables found\n");
        return 0.0;
    }

    // Use the last two coordinate variables
    airlang_intg idx1 = coord_indices[coord_count - 2]; // Second to last
    airlang_intg idx2 = coord_indices[coord_count - 1]; // Last

    //printf("DEBUG: Using variables '%s' and '%s'\n",variables[idx1].name, variables[idx2].name);
   

    // Parse coordinates
    airlang_doub lat1, lon1, lat2, lon2;

    if (!parse_coordinates(variables[idx1].value.str_value, &lat1, &lon1)) {
       // printf("DEBUG: Failed to parse first coordinate\n");
        return 0.0;
    }

    if (!parse_coordinates(variables[idx2].value.str_value, &lat2, &lon2)) {
        return 0.0;
    }

   // printf("DEBUG: Calculating distance between (%.6f,%.6f) and (%.6f,%.6f)\n",lat1, lon1, lat2, lon2);

    // Calculate distance
    airlang_doub distance = calculateAirpath(lat1, lon1, lat2, lon2);
   // printf("DEBUG: Calculated distance = %.2f km\n", distance);

    return distance;
}


airlang_doub evaluate_expression_with_distance(const airlang_strg expr) {
   // printf("DEBUG: evaluate_expression_with_distance called with: '%s'\n", expr);

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
    clean_expr[j] = EOS;

    //printf("DEBUG: cleaned expression: '%s'\n", clean_expr);
    
   
    //check for HEADWIND KEYWORD
    if (strcmp(clean_expr, "HEADWIND()") == 0) {
        if (strlen(current_airport) == 0) return 0.0; // No airport context
        
        airlang_char wind_dir[32], wind_speed[32], rwy_hdg[32];
        snprintf(wind_dir, sizeof(wind_dir), "%s_WIND_DIR", current_airport);
        snprintf(wind_speed, sizeof(wind_speed), "%s_WIND_SPEED", current_airport);
        snprintf(rwy_hdg, sizeof(rwy_hdg), "%s_RUNWAY", current_airport);


        airlang_intg wind_dir_idx = find_variable(wind_dir);
        airlang_intg wind_speed_idx = find_variable(wind_speed);
        airlang_intg runway_idx = find_variable(rwy_hdg);

        if (wind_dir_idx != -1 && wind_speed_idx != -1 && runway_idx != -1) {
            airlang_doub wind_dir = variables[wind_dir_idx].value.num_value;
            airlang_doub wind_speed = variables[wind_speed_idx].value.num_value;
            airlang_doub runway = variables[runway_idx].value.num_value;

            return headwind(wind_dir, wind_speed, runway);
        }
        return 0.0;
    }

    if (strcmp(clean_expr, "CROSSWIND()") == 0) {
        if (strlen(current_airport) == 0) return 0.0; // No airport context

        airlang_char wind_dir[32], wind_speed[32], rwy_hdg[32];
        snprintf(wind_dir, sizeof(wind_dir), "%s_WIND_DIR", current_airport);
        snprintf(wind_speed, sizeof(wind_speed), "%s_WIND_SPEED", current_airport);
        snprintf(rwy_hdg, sizeof(rwy_hdg), "%s_RUNWAY", current_airport);

        airlang_intg wind_dir_idx = find_variable(wind_dir);
        airlang_intg wind_speed_idx = find_variable(wind_speed);
        airlang_intg runway_idx = find_variable(rwy_hdg);

        if (wind_dir_idx != -1 && wind_speed_idx != -1 && runway_idx != -1) {
            airlang_doub wind_dir = variables[wind_dir_idx].value.num_value;
            airlang_doub wind_speed = variables[wind_speed_idx].value.num_value;
            airlang_doub runway = variables[runway_idx].value.num_value;

            return crosswind(wind_dir, wind_speed, runway);
        }
        return 0.0;
    }
    
    // Check for AIRPATH keyword
    if (strcmp(clean_expr, "AIRPATH") == 0) {
        printf("DEBUG: Found AIRPATH keyword\n");
        airlang_doub result = calcLastLegDistance();
        printf("DEBUG: AIRPATH returning: %.2f\n", result);
        return result;
    }

    printf("DEBUG: Not AIRPATH, calling original evaluate_expression\n");
    // If not AIRPATH, use original evaluation
    return evaluate_expression(clean_expr);
}


//WEATHERRRRR

airlang_intg validateRequestStatement(const airlang_strg expression) {

    if (strncmp(expression, "REQUEST", 7) == 0) {
        return 1;
    }
    return 0; 
}

airlang_void handleRequestStatement(airlang_strg expression) {

    trimWhitespace(expression);

    //REQUEST METAR FROM "blabla"
    if (strstr(expression, "REQUEST METAR FROM") != NULL) {
        //GO FOR QUOTES

        airlang_char* quoteStart = strchr(expression, QUOTES);
        airlang_char* quoteEnd = NULL; 

        if (quoteStart != NULL) {
            quoteEnd = strchr(quoteStart + 1, QUOTES);
            if (quoteEnd != NULL) {
                airlang_intg urlLen = (airlang_intg)(quoteEnd - quoteStart - 1);
                airlang_char url[256] = { 0 };

                // Safely copy URL
                airlang_intg i;
                for (i = 0; i < urlLen && i < sizeof(url) - 1; i++) {
                    url[i] = quoteStart[1 + i];
                }
                url[i] = '\0';
                assign_string_variable("METAR_REQUEST_URL", url);

                if (initial_phase) {
                    size_t outLen = strlen(output_buffer);
                    const char* msg = "METAR request received from service\n";
                    size_t msgLen = strlen(msg);

                    if (outLen + msgLen + 1 < sizeof(output_buffer)) {
                        memcpy(output_buffer + outLen, msg, msgLen);
                        output_buffer[outLen + msgLen] = '\0';
                    }
                }
                else {
                    printf("METAR request received from service\n");
                }
                return;
            }
        }
    }

    //NOTAM FOR NOW 
    if (strstr(expression, "REQUEST NOTAM FROM") != NULL) {
        //GO FOR QUOTES

        airlang_char* quoteStart = strchr(expression, QUOTES);
        airlang_char* quoteEnd = NULL;

        if (quoteStart != NULL) {
            quoteEnd = strchr(quoteStart + 1, QUOTES);
            if (quoteEnd != NULL) {
                airlang_intg urlLen = (airlang_intg)(quoteEnd - quoteStart - 1);
                airlang_char url[256] = { 0 };

                // Safely copy URL
                airlang_intg i;
                for (i = 0; i < urlLen && i < sizeof(url) - 1; i++) {
                    url[i] = quoteStart[1 + i];
                }
                url[i] = '\0';
                assign_string_variable("NOTAM_REQUEST_URL", url);

                if (initial_phase) {
                    size_t outLen = strlen(output_buffer);
                    const char* msg = "NOTAM request received from service\n";
                    size_t msgLen = strlen(msg);

                    if (outLen + msgLen + 1 < sizeof(output_buffer)) {
                        memcpy(output_buffer + outLen, msg, msgLen);
                        output_buffer[outLen + msgLen] = '\0';
                    }
                }
                else {
                    printf("NOTAM request received from service\n");
                }
                return;
            }
        }
    }

}

airlang_void windInfo(const airlang_strg metar, const airlang_strg station_id) {
    // Look for wind pattern: 3 digits + 2 digits + optional G + digits + KT
    // Example: 27015KT or 27015G25KT

    airlang_char* kt_pos = strstr(metar, "KT");
    if (kt_pos == NULL) return;

    // Go backwards from KT to find start of wind group
    airlang_char* wind_start = kt_pos;
    while (wind_start > metar && !isspace(*(wind_start - 1))) {
        wind_start--;
    }

    // Check if we have enough characters for wind data
    airlang_intg wind_len = (airlang_intg)(kt_pos - wind_start);
    if (wind_len < 5) return; // Need at least 5 chars (DDDSS)

    // Extract wind direction (first 3 digits)
    if (isdigit(wind_start[0]) && isdigit(wind_start[1]) && isdigit(wind_start[2])) {
        airlang_intg wind_dir = (wind_start[0] - '0') * 100 +
            (wind_start[1] - '0') * 10 +
            (wind_start[2] - '0');

        airlang_char var_name[64];
        snprintf(var_name, sizeof(var_name), "%s_WIND_DIR", station_id);
        assign_numeric_variable(var_name, (airlang_doub)wind_dir);
    }

    // Extract wind speed (next 2 digits)
    if (wind_len >= 5 && isdigit(wind_start[3]) && isdigit(wind_start[4])) {
        airlang_intg wind_speed = (wind_start[3] - '0') * 10 + (wind_start[4] - '0');

        airlang_char var_name[64];
        snprintf(var_name, sizeof(var_name), "%s_WIND_SPEED", station_id);
        assign_numeric_variable(var_name, (airlang_doub)wind_speed);
    }

    // Check for gusts (G followed by 2 digits)
    airlang_char* gust_pos = wind_start + 5; // Start looking after speed
    while (gust_pos < kt_pos) {
        if (*gust_pos == 'G' && (gust_pos + 2) < kt_pos &&
            isdigit(gust_pos[1]) && isdigit(gust_pos[2])) {
            airlang_intg gust_speed = (gust_pos[1] - '0') * 10 + (gust_pos[2] - '0');

            airlang_char var_name[64];
            snprintf(var_name, sizeof(var_name), "%s_WIND_GUST", station_id);
            assign_numeric_variable(var_name, (airlang_doub)gust_speed);
            break;
        }
        gust_pos++;
    }
}


//statue Miles eg 10SM
airlang_void visibilityInfo(const airlang_strg metar, const airlang_strg station_id) {
    airlang_doub visibility = 0.0;

    airlang_char* sm_pos = strstr(metar, "SM");
    if (sm_pos != NULL) {
        //10SM SO MOVE BACK GET NUMBER
        airlang_char* vis_end = sm_pos; 
        airlang_char* vis_start = vis_end - 1;

        //ok now find number start position so go back 
        while (vis_start > metar && (isdigit(*vis_start) || *vis_start == '/')) {
            vis_start--;
        }
        if (!isdigit(*vis_start) && *vis_start != '/') vis_start++;

        //ok now extract the string ending SM 
        airlang_intg visLen = (airlang_intg)(vis_end - vis_start);
        if (visLen > 0 && visLen < 10) {
            airlang_char vis_str[16] = { 0 };
            airlang_intg i;
            for (i = 0; i < visLen && i < sizeof(vis_str) - 1; i++) {
                vis_str[i] = vis_start[i];
            }
            vis_str[i] = '\0';

            // Convert to meters (1 statute mile = 1609 meters)
            //visibility = atof(vis_str) * 1609.0;     
            visibility = atof(vis_str);
        }
        //BUT WHAT IF METAR SHOWS IN METERS NOT SM JUST NUMBERS ; International
    }
    else {
        const airlang_char* pos = metar;
        while (*pos) {
            if (isdigit(*pos)) {
                airlang_intg digitCount = 0; 
                const airlang_char* digitStart = pos; 

                while (isdigit(*pos)) {
                    digitCount++;
                    pos++;
                }
                //5000 : 4 DIGIT NUMBER 
                if (digitCount == 4) {
                    airlang_char vis_str[8] = { 0 };
                    airlang_intg i; 
                    for (i = 0; i < 4;i++) {
                        vis_str[i] = digitStart[i];
                    }
                    vis_str[4] = '\0';
                    visibility = (airlang_doub)atoi(vis_str);
                    break;
                }
            }
            else
            {
                pos++;
            }
        }
    }
    airlang_char var_name[64];
    snprintf(var_name, sizeof(var_name), "%s_VISIBILITY", station_id);
    assign_numeric_variable(var_name, visibility);
}




// A5432 ; A FOLLOWED BY 4 DIGITS 
airlang_void altimeterInfo(const airlang_strg metar, const airlang_strg station_id) {
    const airlang_char* pos = metar;

    while (*pos) {
        if (*pos == 'A' && isdigit(pos[1]) && isdigit(pos[2]) && isdigit(pos[3]) && isdigit(pos[4])) {

            //Extract all 4 digits    sscanf??

            //5432 = 5000 + 400 + 30 + 2; 
            airlang_intg alt_temp = (pos[1] - '0') * 1000 + (pos[2] - '0') * 100 + (pos[3] - '0') * 10 + (pos[4] - '0');

            //converting to decimal as we say decimalll
            airlang_doub altimeter = (airlang_doub)alt_temp / 100.0;

            airlang_char var_name[64];
            snprintf(var_name, sizeof(var_name), "%s_ALTIMETER", station_id);
            assign_numeric_variable(var_name, altimeter);

            break; 

        }
        pos++;
    }

}



//08/06 temperature/dewpoint
airlang_void temperatureInfo(const airlang_strg metar, const airlang_strg station_id) {
    const airlang_char* pos = metar; 

    while (*pos) {
        if (*pos == '/') {
            //temp/dewpoint 
            const airlang_char* slash_pos = pos;
            const airlang_char* start_temp = slash_pos - 1;
            const airlang_char* start_dew = slash_pos + 1;

            //again go baack to find temp start 
            while (start_temp > metar && (isdigit(*start_temp) || *start_temp == 'M')) {
                start_temp--;
            }
            if (!isdigit(*start_temp) && *start_temp != 'M') start_temp++;


            airlang_intg tempLen = (airlang_intg)(slash_pos - start_temp);
            if (tempLen >= 2 && tempLen <= 3) {
                // Extract temperature
                airlang_intg temperature = 0;
                airlang_intg temp_negative = 0;

                if (*start_temp == 'M') {
                    temp_negative = 1;
                    start_temp++;
                }
                if (isdigit(start_temp[0])) {
                    temperature = start_temp[0] - '0';
                    if (isdigit(start_temp[1])) {
                        temperature = temperature * 10 + (start_temp[1] - '0');
                    }
                    if (temp_negative) temperature = -temperature;

                    airlang_char var_name[64];
                    snprintf(var_name, sizeof(var_name), "%s_TEMP", station_id);
                    assign_numeric_variable(var_name, (airlang_doub)temperature);
                }
                // Extract dewpoint
                airlang_intg dewpoint = 0;
                airlang_intg dewp_negative = 0;

                if (*start_dew == 'M') {
                    dewp_negative = 1;
                    start_dew++;
                }
                if (isdigit(start_dew[0])) {
                    dewpoint = start_dew[0] - '0';
                    if (isdigit(start_dew[1])) {
                        dewpoint = dewpoint * 10 + (start_dew[1] - '0');
                    }
                    if (dewp_negative) dewpoint = -dewpoint;

                    airlang_char var_name[64];
                    snprintf(var_name, sizeof(var_name), "%s_DEWPOINT", station_id);
                    assign_numeric_variable(var_name, (airlang_doub)dewpoint);
                }
                break;
            }
        }
        pos++;
    }

}



airlang_void parseMetar(const airlang_strg metar_string, const airlang_strg station_id) {
    airlang_char temp_metar[512] = { 0 };

    airlang_intg i; 
    for (i = 0; metar_string[i] && i < sizeof(temp_metar) - 1; i++) {
        temp_metar[i] = metar_string[i];
    }
    temp_metar[i] = '\0';

    //Removing quotes
    if (temp_metar[0] == QUOTES && temp_metar[strlen(temp_metar) - 1] == QUOTES) {

        //Shift 
        for (i = 0; temp_metar[i + 1]; i++) {
            temp_metar[i] = temp_metar[i + 1];
        }
        temp_metar[i - 1] = '\0'; // Remove the last quote
    }

    //extracting metar components   METAR CYOW 251630Z 27015G25KT 10SM 5000 -RA BKN008 OVC015 08/06 A2995 RMK
    windInfo(temp_metar, station_id);
    visibilityInfo(temp_metar, station_id);
    temperatureInfo(temp_metar, station_id);
    altimeterInfo(temp_metar, station_id);
}



airlang_void handle_metar_assignment(airlang_strg expression) {
   // if (strstr(expression, "METAR_") == NULL) return;

    airlang_char* colon_pos = strchr(expression, ':');
    if (colon_pos == NULL) return;

    // Get variable name (should be "METAR")
    airlang_char var_name[32] = { 0 };
    airlang_intg name_len = (airlang_intg)(colon_pos - expression);

    // Copy and clean variable name
    airlang_intg i, j = 0;
    for (i = 0; i < name_len && i < sizeof(var_name) - 1; i++) {
        if (!isspace(expression[i])) {
            var_name[j++] = expression[i];
        }
    }
    var_name[j] = '\0';

    if (strcmp(var_name, "METAR") != 0) return;

    // Get variable name (everything before colon)
    //airlang_intg name_len = (airlang_intg)(colon_pos - expression);
   // airlang_char var_name[64] = { 0 };

    // Copy variable name safely
    /*airlang_intg i;
    for (i = 0; i < name_len && i < sizeof(var_name) - 1; i++) {
        if (!isspace(expression[i])) {
            var_name[strlen(var_name)] = expression[i];
        }
    }
    */
    // Get METAR string (everything after colon)
    airlang_strg metar_start = colon_pos + 1;
    while (isspace(*metar_start)) metar_start++;

    // Copy METAR string, removing semicolon
    airlang_char metar_string[512] = { 0 };
    for (i = 0;
        metar_start[i] && metar_start[i] != ';' && i < sizeof(metar_string) - 1; i++) {
        metar_string[i] = metar_start[i];
        
    }
    metar_string[i] = '\0';

    // Remove quotes if present
    if (metar_string[0] == QUOTES && metar_string[strlen(metar_string) - 1] == QUOTES) {
        size_t len = strlen(metar_string);
        if (len >= 2) {
            for (i = 0; metar_string[i + 1]; i++) {
                metar_string[i] = metar_string[i + 1];
            }
            metar_string[len - 2] = '\0';
        }
    }

    // Extract airport code (second word after "METAR")
    airlang_char station_id[16] = { 0 };
    airlang_char* metar_pos = strstr(metar_string, "METAR");
    if (metar_pos != NULL) {
        // Skip "METAR" and whitespace
        metar_pos += 5;
        while (*metar_pos && isspace(*metar_pos)) metar_pos++;

        // Extract airport code (next 4 characters typically)
        i = 0;
        while (*metar_pos && !isspace(*metar_pos) && i < sizeof(station_id) - 1) {
            station_id[i++] = *metar_pos++;
        }
        station_id[i] = '\0';
    }

    // Store the raw METAR string
   // assign_string_variable(var_name, metar_string);

    // Extract station ID (METAR_CYOW -> CYOW)
    if (strlen(station_id) >  0) {
        airlang_char full_var_name[64];
        snprintf(full_var_name, sizeof(full_var_name), "METAR_%s", station_id);
        assign_string_variable(full_var_name, metar_string);


        // Parse METAR and create weather variables
        parseMetar(metar_string, station_id);
    }
}

//CALCULATE WIND
airlang_doub headwind(airlang_doub windDirection, airlang_doub windSpeed, airlang_doub runwayHdg) {
    airlang_doub angle_diff = windDirection - runwayHdg ;

    //within -180 to +180
    while (angle_diff > 180) angle_diff -= 360;
    while (angle_diff <= -180) angle_diff += 360;

    airlang_doub headwind = windSpeed * my_cos(angle_diff * (PI / 180.0));
    return headwind;
}

airlang_doub crosswind(airlang_doub windDirection, airlang_doub windSpeed, airlang_doub runwayHdg) {
    airlang_doub angle_diff = windDirection - runwayHdg;

    //within -180 to +180
    while (angle_diff > 180) angle_diff -= 360;
    while (angle_diff <= -180) angle_diff += 360;

    airlang_doub crosswind = windSpeed * my_sin(angle_diff * (PI / 180.0));
    return (crosswind<0) ? -crosswind: crosswind ;
}



// to work with any airport code CYOW, CYYZ, KORD ,
airlang_void extract_airport_from_assignment(const airlang_strg expression, airlang_char* airport_out) {
    airlang_char* equals_pos = strchr(expression, '=');
    if (!equals_pos) return;

    // Get everything before the '='
    airlang_intg name_len = (airlang_intg)(equals_pos - expression);
    airlang_intg i, j = 0;

    for (i = 0; i < name_len && j < 15; i++) {
        if (!isspace(expression[i])) {
            airport_out[j++] = expression[i];
        }
    }
    airport_out[j] = '\0';
}
