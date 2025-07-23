/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2025
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (S25)  |"
# ECHO "=---------------------------------------="
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@           %&@@@@@@@@@@@     @@    ”
# ECHO "    @@       @%% (@@@@@@@@@  @     @@    ”
# ECHO "    @@      @& @   @ @       @     @@    ”
# ECHO "    @@     @ @ %  / /   @@@@@@     @@    ”
# ECHO "    @@      & @ @  @@              @@    ”
# ECHO "    @@       @/ @*@ @ @   @        @@    ”
# ECHO "    @@           @@@@  @@ @ @      @@    ”
# ECHO "    @@            /@@    @@@ @     @@    ”
# ECHO "    @@     @      / /     @@ @     @@    ”
# ECHO "    @@     @ @@   /@/   @@@ @      @@    ”
# ECHO "    @@     @@@@@@@@@@@@@@@         @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@         S O F I A           @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
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

/* Assign string variable */
airlang_void assign_string_variable(const airlang_strg name, const airlang_strg value) {
    airlang_intg idx = find_variable(name);
    if (idx == -1) {
        idx = var_count++;
        strcpy_s(variables[idx].name, sizeof(variables[idx].name), name);
    }
    variables[idx].type = STRING;
    strncpy_s(variables[idx].value.str_value, sizeof(variables[idx].value.str_value), value, sizeof(variables[idx].value.str_value) - 1);
    variables[idx].value.str_value[sizeof(variables[idx].value.str_value) - 1] = EOS;
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
airlang_void handle_write(airlang_strg expression) {
    airlang_char buffer[MAX_EXPR_LEN] = { 0 };
    airlang_strg start = strchr(expression, LPAR) + 1;
    airlang_strg end = strrchr(expression, RPAR);
    if (start != NULL && end != NULL && start < end) {
        *end = EOS;
        while (*start != EOS) {
            if (*start == QUOTES) {
                start++;
                while (*start != QUOTES && *start != EOS) {
                    strncat_s(buffer, sizeof(buffer), start, 1);
                    start++;
                }
                start++;
            }
            else if (isalpha(*start)) {
                airlang_char var_name[32] = { 0 };
                airlang_intg i = 0;
                while (isalnum(*start)) {
                    var_name[i++] = *start++;
                }
                airlang_intg var_idx = find_variable(var_name);
                if (var_idx != -1) {
                    if (variables[var_idx].type == STRING) {
                        strcat_s(buffer, sizeof(buffer), get_string_value(var_name));
                    }
                }
            }
            else if (isspace(*start)) {
                strncat_s(buffer, sizeof(buffer), start, 1);
                start++;
            }
            else {
                start++;
            }
        }
    }
    if (initial_phase) {
        strcat_s(output_buffer, sizeof(output_buffer), buffer);
        strcat_s(output_buffer, sizeof(output_buffer), "\n");
    }
    else {
        printf("%s\n", buffer);
    }
}

/* Calculate expression */
airlang_void calculate(airlang_strg expression) {
    
    if (!expression || strlen(expression) == 0 || expression[0] == '%') {
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
        airlang_intg name_len = colon_pos - expression;
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

        // Remove semicolon if present
        airlang_char value_clean[64] = { 0 };
        i = 0;
        while (value_str[i] && value_str[i] != ';') {
            if (!isspace(value_str[i])) {
                value_clean[strlen(value_clean)] = value_str[i];
            }
            i++;
        }

        // Convert to number and assign
        airlang_doub num_value = atof(value_clean);
        assign_numeric_variable(clean_name, num_value);
    }
      else if (strchr(expression, EQUALS)) {
        sscanf_s(expression, "%31s =", var_name, (unsigned)_countof(var_name));
        airlang_strg expr = strchr(expression, EQUALS) + 1;
        while (isspace(*expr)) expr++;
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
    }
    else if (strstr(expression, WRITE)) {
        handle_write(expression);
    }
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
            printf("%s = %.2lf\n", variables[i].name, variables[i].value.num_value);
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
    airlang_strg line = malloc(MAX_EXPR_LEN);
    if (!lines || !line) {
        return;
    }
    printf("Lines from content:\n");
    airlang_intg i = 0;
	for (i = 0; i < lineCount; i++) {
		line = lines[i];
		calculate(line);
	}
    initial_phase = 0; // End of initial phase
    printf("%s", output_buffer); // Print the buffered write output
    printf("\nVariable values:\n");
    for (i = 0; i < var_count; i++) {
        if (variables[i].type == STRING) {
            printf("%s = \"%s\"\n", variables[i].name, variables[i].value.str_value);
        }
        else if (variables[i].type == NUMERIC) {
            printf("%s = %.2lf\n", variables[i].name, variables[i].value.num_value);
        }
        else if (variables[i].type == BOOLEAN) {
            printf("%s = %s\n", variables[i].name, variables[i].value.bool_value ? "true" : "false");
        }
        else if (variables[i].type == CHAR) {
            printf("%s = '%c'\n", variables[i].name, variables[i].value.char_value);
        }
    }
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