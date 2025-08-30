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


    /*
    ************************************************************
    * Find Variable
    *		Searches through the global variables array to locate a variable
    *		by its name. Uses string comparison to match variable names and
    *		returns the index position for efficient access. Essential for
    *		variable lookup operations in assignment and retrieval processes.
    * Parameters:
    *		name: The name of the variable to search for (const airlang_strg)
    * Return:
    *		Index of the variable if found, -1 if not found (airlang_intg)
    * Purpose:
    *		Provides efficient variable lookup for assignment and retrieval operations
    ************************************************************
    */
    airlang_intg find_variable(const airlang_strg name) {
        airlang_intg i = 0;
        for (i = 0; i < var_count; ++i) {
            if (strcmp(variables[i].name, name) == 0) {
                return i;
            }
        }
        return -1;
    }


    /*
     ************************************************************
     * Assign String Variable
     *		Creates or updates a string variable with the specified value.
     *		If the variable doesn't exist, it allocates a new entry in the
     *		variables array. Performs safe string copying with bounds checking
     *		to prevent buffer overflow. Handles variable name sanitization.
     * Parameters:
     *		name: The name of the variable to assign (const airlang_strg)
     *		value: The string value to assign (const airlang_strg)
     * Return:
     *		None (airlang_void)
     * Purpose:
     *		Handles string variable assignment with bounds checking and memory safety
     ************************************************************
     */
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

    /*
     ************************************************************
     * Get String Value
     *		Retrieves the string value of a named variable with type validation.
     *		Ensures the variable exists and is of STRING type before returning
     *		its value. Returns empty string constant if variable not found or
     *		type mismatch occurs, providing safe access to string variables.
     ************************************************************
     */
    const airlang_strg get_string_value(const airlang_strg name) {
        airlang_intg idx = find_variable(name);
        if (idx != -1 && variables[idx].type == STRING) {
            return variables[idx].value.str_value;
        }
        return STREMPTY;
    }



    /*
    ************************************************************
    * Safe Concatenate
    *		Performs bounds-checked string concatenation to prevent buffer
    *		overflow vulnerabilities. Calculates remaining buffer space and
    *		copies only what fits safely. Always ensures null termination
    *		of the destination string for memory safety.
    ************************************************************
    */
    airlang_void safe_concat(char* dest, size_t dest_size, const char* src) {
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


    /*
    ************************************************************
    * Handle Write
    *		Processes PRINT statements in AIRLANG syntax with variable
    *		interpolation and string concatenation. Parses expressions within
    *		braces, handles quoted strings, resolves variable references,
    *		and manages output buffering for two-phase execution model.
    * Parameters:
    *		expression: The PRINT expression to process (airlang_strg)
    * Return:
    *		None (airlang_void)
    * Purpose:
    *		Processes and outputs formatted text with variable interpolation
    ************************************************************
    */
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




    /*
     ************************************************************
     * Calculate
     *		Main expression processor and statement dispatcher for AIRLANG.
     *		Handles all language constructs including variable assignments,
     *		control flow statements, aviation calculations, weather data
     *		processing, and output operations. Manages execution flow
     *		control and comment filtering for complete program interpretation.
     * Parameters:
     *		expression: The AIRLANG statement or expression to process (airlang_strg)
     * Return:
     *		None (airlang_void)
     * Purpose:
     *		Central dispatcher for all AIRLANG language constructs and statements
     ************************************************************
     */
    airlang_void calculate(airlang_strg expression) {

        trimWhitespace(expression);

        if (!expression || strlen(expression) == 0 || expression[0] == '%') {
            return;
        }

        //wind

        static airlang_intg aircraft_initialized = 0;
        if (!aircraft_initialized && find_variable("AircraftType") != -1) {
            initializeAircraftContext();
            aircraft_initialized = 1;
        }

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
                airlang_doub result = evaluate_expression_with_wb(clean_expr);
                assign_numeric_variable(var_name, result);
                if (!initial_phase) {
                    //printf("DEBUG: Assigned %s = %.2f\n", var_name, result);
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


    /*
     ************************************************************
     * Trim Whitespace
     *		Removes leading and trailing whitespace characters from strings
     *		in-place. Performs safe memory operations to prevent buffer
     *		overflow while maintaining string integrity. Essential for
     *		consistent parsing of AIRLANG statements and expressions.
     ************************************************************
     */
    airlang_void trimWhitespace(char* str) {
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


    /*
     ************************************************************
     * Process File
     *		Main file interpreter that reads and processes AIRLANG source
     *		files line by line. Implements two-phase execution: collection
     *		phase for gathering output, then display phase. Manages file
     *		operations and displays final variable states after execution.
     ************************************************************
     */
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


    /*
     ************************************************************
     * Split Into Lines
     *		Parses complete file content into individual line segments for
     *		sequential processing. Dynamically allocates memory for line
     *		storage and handles newline character detection. Returns array
     *		of string pointers for efficient line-by-line interpretation.
     ************************************************************
     */
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


    /*
     ************************************************************
     * Free Lines
     *		Deallocates memory previously allocated by splitIntoLines function.
     *		Prevents memory leaks by properly freeing both individual line
     *		storage and the main line pointer array. Essential for proper
     *		memory management in dynamic line processing operations.
     ************************************************************
     */
    airlang_void freeLines(airlang_strg* lines, airlang_intg lineCount) {
        airlang_intg i = 0;
        for (i = 0; i < lineCount; i++) {
            free(lines[i]);
        }
        free(lines);
    }


    /*
     ************************************************************
     * Process Content
     *		Alternative interpreter for processing AIRLANG programs from
     *		memory rather than files. Splits content into lines and executes
     *		using the same two-phase model as file processing. Provides
     *		flexibility for runtime program generation and execution.
     * Parameters:
     *		fileContent: Complete AIRLANG program as string (airlang_strg)
     * Return:
     *		None (airlang_void)
     * Purpose:
     *		In-memory AIRLANG program execution alternative to file processing
     ************************************************************
     */
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

        if (!initializeAircraftContext()) {
            printf("AIRCRAFT WARNING: Weight and balance calculations may not be accurate\n");
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

    /*
     ************************************************************
     * Assign Numeric Variable
     *		Creates or updates numeric variables with type management.
     *		Handles variable creation if it doesn't exist and ensures
     *		proper type assignment. Provides safe numeric value storage
     *		with automatic type conversion and variable table management.
     ************************************************************
     */
    airlang_void assign_numeric_variable(const airlang_strg name, airlang_doub value) {
        airlang_intg idx = find_variable(name);
        if (idx == -1) {
            idx = var_count++;
            strcpy_s(variables[idx].name, sizeof(variables[idx].name), name);
        }
        variables[idx].type = NUMERIC;
        variables[idx].value.num_value = value;
    }


    /*
     ************************************************************
     * Evaluate Expression
     *		Arithmetic expression evaluator with operator precedence handling.
     *		Parses mathematical expressions containing variables and constants,
     *		resolves variable references, and performs calculations following
     *		standard mathematical operator precedence rules ( *+-order).
     * Parameters:
     *expr : Mathematical expression to evaluate(const airlang_strg)
         * Return :
         *Result of the mathematical expression(airlang_doub)
         * Purpose :
         *Evaluates arithmetic expressions with proper operator precedence
         ************************************************************
     */
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


    /*
     ************************************************************
     * Evaluate Condition
     *		Boolean condition evaluator for IF-ELSE control structures.
     *		Parses comparison operators and evaluates logical conditions
     *		between variables and constants. Supports greater-than comparisons
     *		with automatic variable resolution and type conversion.
     * Parameters:
     *		condition: Logical condition to evaluate (const airlang_strg)
     * Return:
     *		1 if condition is true, 0 if false (airlang_intg)
     * Purpose:
     *		Evaluates boolean conditions for IF-ELSE control flow statements
     ************************************************************
     */
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

    /*
     ************************************************************
     * Handle IF-ELSE
     *		Control flow processor for conditional statements in AIRLANG.
     *		Manages IF-THEN-ELSE-ENDIF block execution with proper nesting
     *		and execution flow control. Maintains state flags for conditional
     *		execution and skip logic based on condition evaluation results.
     * Parameters:
     *		expression: IF-ELSE statement to process (airlang_strg)
     * Return:
     *		1 if statement was handled, 0 if not an IF-ELSE statement (airlang_intg)
     * Purpose:
     *		Manages conditional execution flow for IF-ELSE control structures
     ************************************************************
     */
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

    /*
     ************************************************************
     * Is Aircraft Identifier
     *		Validates aircraft identification strings according to aviation
     *		standards. Recognizes IATA flight numbers (2 letters + 3 digits)
     *		and ICAO aircraft registrations (letter-dash-4 alphanumeric).
     *		Essential for proper aviation data type recognition and validation.
     * Parameters:
     *		value: String to validate as aircraft identifier (const airlang_strg)
     * Return:
     *		1 if valid aircraft identifier, 0 if invalid (airlang_intg)
     * Purpose:
     *		Validates aircraft identification strings according to aviation standards
     ************************************************************
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

    /*
     ************************************************************
     * Is Date Format
     *		Validates date strings in aviation format with single quotes.
     *		Checks for proper YYYY-MM-DD format enclosed in single quotes
     *		as commonly used in aviation documentation and flight planning.
     *		Ensures date string integrity for temporal calculations.
     ************************************************************
     */
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

    /*
     ************************************************************
     * Is Coordinate Format
     *		Validates geographic coordinate strings in latitude,longitude format.
     *		Checks for proper decimal degree format with comma separation
     *		and supports negative values for southern latitudes and western
     *		longitudes. Essential for aviation navigation calculations.
     * Parameters:
     *		value: String to validate as coordinate format (const airlang_strg)
     * Return:
     *		1 if valid coordinate format, 0 if invalid (airlang_intg)
     * Purpose:
     *		Validates geographic coordinate strings for navigation calculations
     ************************************************************
     */
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


    /*
    ************************************************************
    * Parse Coordinates
    *		Extracts latitude and longitude values from coordinate strings.
    *		Validates format and converts string representations to numeric
    *		values for mathematical calculations. Handles negative coordinates
    *		and provides error checking for malformed coordinate data.
    * Parameters:
    *		coord_str: Coordinate string to parse (const airlang_strg)
    *		lat: Pointer to store latitude value (airlang_doub*)
    *		lon: Pointer to store longitude value (airlang_doub*)
    * Return:
    *		1 if parsing successful, 0 if failed (airlang_intg)
    * Purpose:
    *		Converts coordinate strings to numeric values for calculations
    ************************************************************
    */
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



    /*
    ************************************************************
    * AVIATION-SPECIFIC FUNCTIONS SECTION
    *		This section contains specialized functions for aviation calculations
    *		including mathematical utilities, navigation computations, weather
    *		data processing, and flight planning operations. These functions
    *		implement aviation standards and formulas required for:
    *		- Great circle distance calculations (Haversine formula)
    *		- Wind component analysis (headwind/crosswind)
    *		- METAR weather report parsing and interpretation
    *		- Geographic coordinate processing and validation
    *		- Trigonometric calculations for navigation
    *		All functions follow aviation industry standards and practices
    *		for accurate flight planning and operational calculations.
    ************************************************************
    */


    /* Sine function using Taylor series (x - x^3/6 + ^5/120 - x^7/5040) */
    airlang_doub my_sin(airlang_doub x) {
        airlang_doub x3 = x * x * x;
        airlang_doub x5 = x3 * x * x;
        airlang_doub x7 = x5 * x * x;
        return x - x3 / 6.0 + x5 / 120.0 - x7 / 5040.0;
    }

    /* Cosine function using Taylor series (1 - x^3/2 + x/24 - x^6/720) */
    airlang_doub my_cos(airlang_doub x) {
        airlang_doub x2 = x * x;
        airlang_doub x4 = x2 * x2;
        airlang_doub x6 = x4 * x2;
        return 1 - x2 / 2.0 + x4 / 24.0 - x6 / 720.0;
    }

    /*Arctangent function using Taylor series*/
        airlang_doub my_atan(airlang_doub x) {
        // For better convergence, use identities for |x| > 1
        if (x > 1.0) {
            return PI / 2.0 - my_atan(1.0 / x);
        }
        else if (x < -1.0) {
            return -PI / 2.0 - my_atan(1.0 / x);
        }

        // Taylor series: x - x^3/3 + x^5/5 - x^7/7 + x^9/9 - x^11/11
        airlang_doub result = x;
        airlang_doub x_squared = x * x;
        airlang_doub x_power = x;

        for (int i = 1; i < 6; i++) {
            x_power *= x_squared;
            int denominator = 2 * i + 1;
            if (i % 2 == 1) {
                result -= x_power / denominator;
            }
            else {
                result += x_power / denominator;
            }
        }

        return result;
    }

    /* Square root using Newton's method iteration */
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

    /* Arctangent2 function for quadrant-aware angle calculation */
    airlang_doub my_atan2(airlang_doub y, airlang_doub x) {
        if (x > 0) return my_atan(y / x);
        else if (x < 0 && y >= 0) return PI + my_atan(y / x);
        else if (x < 0 && y < 0) return -PI + my_atan(y / x);
        else if (x == 0 && y > 0) return PI / 2;
        else if (x == 0 && y < 0) return -PI / 2;
        else return 0;
    }

    /* Convert degrees to radians (degrees × PI/180) */
    airlang_doub toRadians(airlang_doub degree) {
        return degree * (PI / 180.0);
    }

    /*
    ************************************************************
    * Calculate Airpath
    *		Computes great circle distance between two geographic points
    *		using the Haversine formula. Implements custom trigonometric
    *		functions for accurate aviation distance calculations in nautical
    *		miles. Essential for flight planning and navigation computations.
    * Parameters:
    *		lat1: Starting latitude in degrees (airlang_doub)
    *		lon1: Starting longitude in degrees (airlang_doub)
    *		lat2: Ending latitude in degrees (airlang_doub)
    *		lon2: Ending longitude in degrees (airlang_doub)
    * Return:
    *		Great circle distance in nautical miles (airlang_doub)
    * Purpose:
    *		Computes aviation distance between geographic coordinates
    ************************************************************
    */
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


    /*
     ************************************************************
     * Calculate Last Leg Distance
     *		Automatically determines distance between the last two coordinate
     *		variables in the program. Scans variable table for coordinate
     *		pairs and computes great circle distance for the final leg of
     *		a flight path. Supports dynamic flight planning calculations.
     * Parameters:
     *		None (void)
     * Return:
     *		Distance between last two coordinates in nautical miles (airlang_doub)
     * Purpose:
     *		Computes distance between the most recent coordinate pair
     ************************************************************
     */
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

    /*
     ************************************************************
     * Evaluate Expression With Distance
     *		Enhanced expression evaluator supporting aviation-specific functions.
     *		Handles AIRPATH calculations, wind component computations (HEADWIND,
     *		CROSSWIND), and standard arithmetic expressions. Integrates airport
     *		context for weather-related calculations and flight operations.
     * Parameters:
     *		expr: Expression to evaluate with aviation functions (const airlang_strg)
     * Return:
     *		Result of expression evaluation (airlang_doub)
     * Purpose:
     *		Evaluates expressions with aviation-specific function support
     ************************************************************
     */
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
            //printf("DEBUG: Found AIRPATH keyword\n");
            airlang_doub result = calcLastLegDistance();
            //printf("DEBUG: AIRPATH returning: %.2f\n", result);
            return result;
        }

        //printf("DEBUG: Not AIRPATH, calling original evaluate_expression\n");
        // If not AIRPATH, use original evaluation
        return evaluate_expression(clean_expr);
    }


    /*
     ************************************************************
     * Validate Request Statement
     *		Identifies REQUEST statements for aviation data services.
     *		Recognizes METAR weather requests and NOTAM information requests
     *		for integration with external aviation data sources. Validates
     *		proper REQUEST syntax for service integration capabilities.
     * Parameters:
     *		expression: Statement to validate as REQUEST (const airlang_strg)
     * Return:
     *		1 if valid REQUEST statement, 0 if not (airlang_intg)
     * Purpose:
     *		Validates REQUEST statements for aviation data service integration
     ************************************************************
     */
    airlang_intg validateRequestStatement(const airlang_strg expression) {

        if (strncmp(expression, "REQUEST", 7) == 0) {
            return 1;
        }
        return 0; 
    }


    /*
     ************************************************************
     * Handle Request Statement
     *		Processes aviation data requests for METAR and NOTAM services.
     *		Extracts service URLs from quoted strings and creates appropriate
     *		variables for external data integration. Manages service request
     *		acknowledgment and status reporting for aviation data operations.
     * Parameters:
     *		expression: REQUEST statement to process (airlang_strg)
     * Return:
     *		None (airlang_void)
     * Purpose:
     *		Processes aviation data service requests and manages integration
     ************************************************************
     */
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

    /*
     ************************************************************
     * Wind Information
     *		Extracts wind direction, speed, and gust information from METAR
     *		reports. Parses standard aviation wind format (direction + speed + KT)
     *		with optional gust values. Creates station-specific wind variables
     *		for aviation weather analysis and flight planning calculations.
     ************************************************************
     */
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


    /*
     ************************************************************
     * Visibility Information
     *		Parses visibility data from METAR reports in statute miles or meters.
     *		Handles both international format (meters) and US format (statute
     *		miles with SM suffix). Creates visibility variables for flight
     *		planning and aviation safety assessments.
     ************************************************************
     */
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



    /*
     ************************************************************
     * Altimeter Information
     *		Extracts altimeter setting from METAR reports in inches of mercury.
     *		Parses standard aviation altimeter format (A followed by 4 digits)
     *		and converts to decimal representation. Essential for aircraft
     *		altitude calculations and pressure altitude determinations.
     ************************************************************
     */
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


    /*
     ************************************************************
     * Temperature Information
     *		Parses temperature and dewpoint data from METAR weather reports.
     *		Handles both positive and negative temperatures with M prefix for
     *		negative values. Extracts temperature/dewpoint pairs in standard
     *		aviation format for weather analysis and aircraft performance.
     *      08/06 temperature/dewpoint
     ************************************************************
     */
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


    /*
     ************************************************************
     * Parse METAR
     *		Comprehensive METAR weather report parser for aviation operations.
     *		Extracts wind, visibility, temperature, and altimeter information
     *		from standard aviation weather reports. Creates individual variables
     *		for each weather component with proper station identification.
     * Parameters:
     *		metar_string: METAR weather report string (const airlang_strg)
     *		station_id: Airport station identifier (const airlang_strg)
     * Return:
     *		None (airlang_void)
     * Purpose:
     *		Parses METAR weather reports into individual weather components
     ************************************************************
     */
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


    /*
     ************************************************************
     * Handle METAR Assignment
     *		Processes METAR variable assignments and triggers weather parsing.
     *		Extracts station identifiers from METAR strings and initiates
     *		comprehensive weather data extraction. Manages METAR string storage
     *		and coordinates weather component variable creation.
     * Parameters:
     *		expression: METAR assignment statement (airlang_strg)
     * Return:
     *		None (airlang_void)
     * Purpose:
     *		Manages METAR data assignment and triggers comprehensive parsing
     ************************************************************
     */
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

    /*
     ************************************************************
     * Headwind Calculation
     *		Computes headwind component from wind direction, speed, and runway
     *		heading using trigonometric calculations. Handles angle normalization
     *		and provides positive values for headwinds, negative for tailwinds.
     *		Critical for aircraft performance and landing calculations.
     ************************************************************
     */
    airlang_doub headwind(airlang_doub windDirection, airlang_doub windSpeed, airlang_doub runwayHdg) {
        airlang_doub angle_diff = windDirection - runwayHdg ;

        //within -180 to +180
        while (angle_diff > 180) angle_diff -= 360;
        while (angle_diff <= -180) angle_diff += 360;

        airlang_doub headwind = windSpeed * my_cos(angle_diff * (PI / 180.0));
        return headwind;
    }

    /*
     ************************************************************
     * Crosswind Calculation
     *		Computes crosswind component perpendicular to runway heading from
     *		wind direction and speed. Uses trigonometric functions with angle
     *		normalization and returns absolute crosswind values for aviation
     *		safety assessments and aircraft operational limitations.
     ************************************************************
     */
    airlang_doub crosswind(airlang_doub windDirection, airlang_doub windSpeed, airlang_doub runwayHdg) {
        airlang_doub angle_diff = windDirection - runwayHdg;

        //within -180 to +180
        while (angle_diff > 180) angle_diff -= 360;
        while (angle_diff <= -180) angle_diff += 360;

        airlang_doub crosswind = windSpeed * my_sin(angle_diff * (PI / 180.0));
        return (crosswind<0) ? -crosswind: crosswind ;
    }


    /*
     ************************************************************
     * Extract Airport From Assignment
     *		Parses variable assignment statements to extract airport codes
     *		for contextual operations. Identifies airport identifiers from
     *		variable names for use in weather calculations and aviation
     *		operations requiring airport-specific context information.
     * Parameters:
     *		expression: Assignment statement containing airport code (const airlang_strg)
     *		airport_out: Buffer to store extracted airport code (airlang_char*)
     * Return:
     *		None (airlang_void)
     * Purpose:
     *		Extracts airport context from variable assignments for operations
     *      //to work with any airport code CYOW, CYYZ, KORD ,
     ************************************************************
     */
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



    // Static aircraft database - certified performance data
        static AircraftConfig aircraft_database[] = {
        // Piper Cherokee PA-28 (General Aviation)
        {
            "PA28", 
            "Piper Cherokee PA-28-140",
            1410.0,     // Empty weight
            2150.0,     // max_takeoff_weight  
        2150.0,     // max_landing_weight
        300.0,      // fuel_capacity (50 gal × 6 lbs/gal)
        85.0,       // empty_cg_arm
        82.0,       // forward_cg_limit
        94.0,       // aft_cg_limit
        85.0,       // pilot_arm
        118.0,      // passenger_arm  
        142.0,      // baggage_arm
        95.0,       // fuel_arm
        108.0,      // cruise_speed
        8.0         // fuel_burn_rate
    },

            // Cessna 172 (General Aviation)
            {
                "C172",                      // aircraft_id
                "Cessna 172 Skyhawk",        // aircraft_name
                1663.0,     // empty_weight
                2450.0,     // max_takeoff_weight
                2450.0,     // max_landing_weight  
                318.0,      // fuel_capacity (53 gal × 6 lbs/gal)
                39.0,       // empty_cg_arm
                35.0,       // forward_cg_limit
                47.0,       // aft_cg_limit
                37.0,       // pilot_arm
                37.0,       // passenger_arm
                95.0,       // baggage_arm
                48.0,       // fuel_arm
                122.0,      // cruise_speed
                9.0         // fuel_burn_rate
            },

            // Boeing 747-400 (Commercial Aviation)
            {
                "B747",                      // aircraft_id  
                "Boeing 747-400",            // aircraft_name
                393263.0,   // empty_weight
                833000.0,   // max_takeoff_weight
                652000.0,   // max_landing_weight
                383000.0,   // fuel_capacity
                958.0,      // empty_cg_arm
                756.0,      // forward_cg_limit
                1157.0,     // aft_cg_limit
                350.0,      // pilot_arm (cockpit)
                1020.0,     // passenger_arm (average cabin)
                1400.0,     // baggage_arm (cargo hold)
                1020.0,     // fuel_arm (wing tanks)
                493.0,      // cruise_speed
                2400.0      // fuel_burn_rate
            }
        };
    static const airlang_intg AIRCRAFT_DB_SIZE = sizeof(aircraft_database) / sizeof(AircraftConfig);
    static AircraftConfig* current_aircraft = NULL;

    /*
 ************************************************************
 * Find Aircraft Configuration
 *		Searches aircraft database for specified aircraft type and
 *		sets current aircraft context for weight and balance calculations.
 *		Validates aircraft ID against certified database entries.
 * Parameters:
 *		aircraft_id: Aircraft identifier to search for (const airlang_strg)
 * Return:
 *		Pointer to aircraft configuration or NULL if not found (AircraftConfig*)
 * Purpose:
 *		Establishes aircraft context for performance calculations
 ************************************************************
 */
    AircraftConfig* findAircraftConfig(const airlang_strg aircraft_id) {
        airlang_intg i = 0;
        for (i = 0; i < AIRCRAFT_DB_SIZE; i++) {
            if (strcmp(aircraft_database[i].aircraft_id, aircraft_id) == 0) {
                current_aircraft = &aircraft_database[i];
                printf("AIRCRAFT DATABASE: Loaded configuration for %s (%s)\n",
                    aircraft_database[i].aircraft_id, aircraft_database[i].aircraft_name);
                return &aircraft_database[i];
            }
        }
        printf("AVIATION ERROR: Aircraft type '%s' not found in certified database\n", aircraft_id);
        return NULL;
    }

    /*
     ************************************************************
     * Calculate Total Weight
     *		Computes total aircraft weight including empty weight, payload,
     *		fuel, and all variable loads. Implements aviation weight
     *		calculation standards for flight safety and performance planning.
     * Parameters:
     *		None (uses current flight variables)
     * Return:
     *		Total aircraft weight in pounds (airlang_doub)
     * Purpose:
     *		Calculates total weight for MTOW compliance checking
     ************************************************************
     */
    airlang_doub calculateTotalWeight() {
        if (!current_aircraft) {
            printf("WEIGHT ERROR: No aircraft configuration loaded\n");
            return 0.0;
        }

        // Get variable values from flight plan
        airlang_doub passenger_weight = 0.0;
        airlang_doub baggage_weight = 0.0;
        airlang_doub fuel_weight = 0.0;

        // Find passenger count and calculate weight (assume 170 lbs per person)
        airlang_intg passenger_idx = find_variable("PassengerCount");
        if (passenger_idx != -1) {
            passenger_weight = variables[passenger_idx].value.num_value * 170.0;
        }

        // Find baggage weight
        airlang_intg baggage_idx = find_variable("BaggageWeight");
        if (baggage_idx != -1) {
            baggage_weight = variables[baggage_idx].value.num_value;
        }

        // Find fuel weight
        airlang_intg fuel_idx = find_variable("EstimatedFuelRequired");
        if (fuel_idx != -1) {
            fuel_weight = variables[fuel_idx].value.num_value;
        }

        airlang_doub total_weight = current_aircraft->empty_weight +
            passenger_weight +
            baggage_weight +
            fuel_weight;

        //printf("WEIGHT CALCULATION:\n");
        //printf("  Empty Weight: %.0f lbs\n", current_aircraft->empty_weight);
        //printf("  Passenger Weight: %.0f lbs\n", passenger_weight);
        //printf("  Baggage Weight: %.0f lbs\n", baggage_weight);
        //printf("  Fuel Weight: %.0f lbs\n", fuel_weight);
        //printf("  TOTAL WEIGHT: %.0f lbs\n", total_weight);

        return total_weight;
    }

    /*
     ************************************************************
     * Calculate Weight Balance Moment
     *		Computes total aircraft moment for center of gravity calculations.
     *		Uses aircraft-specific arm distances and load distributions to
     *		determine moment about the datum. Essential for flight safety
     *		and aircraft controllability validation.
     * Parameters:
     *		None (uses current aircraft configuration and flight variables)
     * Return:
     *		Total aircraft moment in pound-inches (airlang_doub)
     * Purpose:
     *		Calculates moment for center of gravity position determination
     ************************************************************
     */
    airlang_doub calculateWeightBalanceMoment() {
        if (!current_aircraft) {
            printf("MOMENT ERROR: No aircraft configuration loaded\n");
            return 0.0;
        }

        // Calculate individual moments
        airlang_doub empty_moment = current_aircraft->empty_weight * current_aircraft->empty_cg_arm;

        // Passenger moment
        airlang_doub passenger_weight = 0.0;
        airlang_intg passenger_idx = find_variable("PassengerCount");
        if (passenger_idx != -1) {
            passenger_weight = variables[passenger_idx].value.num_value * 170.0;
        }
        airlang_doub passenger_moment = passenger_weight * current_aircraft->passenger_arm;

        // Baggage moment  
        airlang_doub baggage_weight = 0.0;
        airlang_intg baggage_idx = find_variable("BaggageWeight");
        if (baggage_idx != -1) {
            baggage_weight = variables[baggage_idx].value.num_value;
        }
        airlang_doub baggage_moment = baggage_weight * current_aircraft->baggage_arm;

        // Fuel moment
        airlang_doub fuel_weight = 0.0;
        airlang_intg fuel_idx = find_variable("EstimatedFuelRequired");
        if (fuel_idx != -1) {
            fuel_weight = variables[fuel_idx].value.num_value;
        }
        airlang_doub fuel_moment = fuel_weight * current_aircraft->fuel_arm;

        airlang_doub total_moment = empty_moment + passenger_moment + baggage_moment + fuel_moment;

       // printf("MOMENT CALCULATION:\n");
       // printf("  Empty Moment: %.0f lb-in\n", empty_moment);
       // printf("  Passenger Moment: %.0f lb-in\n", passenger_moment);
       // printf("  Baggage Moment: %.0f lb-in\n", baggage_moment);
       // printf("  Fuel Moment: %.0f lb-in\n", fuel_moment);
       // printf("  TOTAL MOMENT: %.0f lb-in\n", total_moment);

        return total_moment;
    }

    /*
     ************************************************************
     * Calculate Center of Gravity
     *		Determines aircraft center of gravity position using total weight
     *		and moment calculations. Computes CG location relative to aircraft
     *		datum for weight and balance envelope validation. Critical for
     *		aircraft stability and control authority verification.
     * Parameters:
     *		None (uses calculated weight and moment values)
     * Return:
     *		Center of gravity position in inches from datum (airlang_doub)
     * Purpose:
     *		Calculates CG position for envelope compliance checking
     ************************************************************
     */
    airlang_doub calculateCenterOfGravity() {
        airlang_doub total_weight = calculateTotalWeight();
        airlang_doub total_moment = calculateWeightBalanceMoment();

        if (total_weight <= 0.0) {
            printf("CG ERROR: Invalid total weight for CG calculation\n");
            return 0.0;
        }

        airlang_doub cg_position = total_moment / total_weight;

        //printf("CENTER OF GRAVITY CALCULATION:\n");
        //printf("  CG Position: %.2f inches from datum\n", cg_position);
        //maybe want to add ASCII art to show CG envelope
        return cg_position;
    }

    /*
     ************************************************************
     * Validate Weight and Balance
     *		Performs comprehensive weight and balance validation against
     *		aircraft certified limits. Checks total weight against MTOW,
     *		validates center of gravity within approved envelope, and
     *		provides flight safety authorization status. Implements FAA
     *		weight and balance compliance requirements.
     * Parameters:
     *		None (uses current aircraft configuration and calculations)
     * Return:
     *		Validation status string: "WITHIN_LIMITS", "OVERWEIGHT",
     *		"CG_OUT_OF_LIMITS", or "CRITICAL_ERROR" (const airlang_strg)
     * Purpose:
     *		Validates aircraft loading for flight safety authorization
     ************************************************************
     */
    const airlang_strg validateWeightAndBalance() {
        if (!current_aircraft) {
            printf("VALIDATION ERROR: No aircraft configuration available\n");
            return "CRITICAL_ERROR";
        }

        airlang_doub total_weight = calculateTotalWeight();
        airlang_doub cg_position = calculateCenterOfGravity();

        // Weight validation
        airlang_intg weight_ok = (total_weight <= current_aircraft->max_takeoff_weight);

        // CG validation
        airlang_intg cg_ok = (cg_position >= current_aircraft->forward_cg_limit &&
            cg_position <= current_aircraft->aft_cg_limit);

       //printf("\nWEIGHT & BALANCE VALIDATION:\n");
       //printf("========================================\n");
       //printf("Aircraft: %s\n", current_aircraft->aircraft_name);
       //printf("Total Weight: %.0f lbs (Limit: %.0f lbs)\n", total_weight, current_aircraft->max_takeoff_weight);
       //printf("Weight Status: %s\n", weight_ok ? "WITHIN LIMITS" : "OVERWEIGHT");
       //printf("CG Position: %.2f in (Limits: %.1f to %.1f in)\n",
       //    cg_position, current_aircraft->forward_cg_limit, current_aircraft->aft_cg_limit);
       //printf("CG Status: %s\n", cg_ok ? "WITHIN LIMITS" : "OUT OF LIMITS");
       //printf("========================================\n");

        if (!weight_ok && !cg_ok) {
            printf("CRITICAL: Both weight and CG are out of limits!\n");
            return "CRITICAL_ERROR";
        }
        else if (!weight_ok) {
            printf("WARNING: Aircraft is overweight - reduce load\n");
            return "OVERWEIGHT";
        }
        else if (!cg_ok) {
            printf("WARNING: CG is out of limits - redistribute load\n");
            return "CG_OUT_OF_LIMITS";
        }
        else {
            printf("APPROVED: Weight and Balance within certified limits\n");
            return "WITHIN_LIMITS";
        }
    }

    /*
     ************************************************************
     * Enhanced Expression Evaluation with W&B
     *		Extended expression evaluator supporting weight and balance
     *		calculations. Integrates TOTALWEIGHT, WEIGHTBAL, and VALIDATEWB
     *		functions with existing aviation calculations. Provides seamless
     *		access to aircraft performance and safety calculations.
     * Parameters:
     *		expr: Expression containing W&B functions (const airlang_strg)
     * Return:
     *		Result of W&B calculation or original expression value (airlang_doub)
     * Purpose:
     *		Evaluates weight and balance expressions in flight planning
     ************************************************************
     */
    airlang_doub evaluate_expression_with_wb(const airlang_strg expr) {
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

        // Check for weight and balance functions
        if (strcmp(clean_expr, "TOTALWEIGHT") == 0) {
            return calculateTotalWeight();
        }

        if (strcmp(clean_expr, "WEIGHTBAL") == 0) {
            return calculateWeightBalanceMoment();
        }

        if (strcmp(clean_expr, "CENTEROFGRAVITY") == 0) {
            return calculateCenterOfGravity();
        }

        if (strcmp(clean_expr, "VALIDATEWB") == 0) {
            // Return numeric code: 1 = OK, 0 = Error
            const airlang_strg result = validateWeightAndBalance();
            if (strcmp(result, "WITHIN_LIMITS") == 0) {
                return 1.0;
            }
            else {
                return 0.0;
            }
        }

        // If not W&B function, try distance/wind functions
        return evaluate_expression_with_distance(clean_expr);
    }

    /*
     ************************************************************
     * Initialize Aircraft Context
     *		Sets up aircraft context for weight and balance calculations
     *		based on AircraftType variable from flight plan. Automatically
     *		loads appropriate aircraft configuration from certified database.
     * Parameters:
     *		None (searches for AircraftType variable)
     * Return:
     *		1 if aircraft loaded successfully, 0 if failed (airlang_intg)
     * Purpose:
     *		Initializes aircraft database context for calculations
     ************************************************************
     */
    airlang_intg initializeAircraftContext() {
        // Look for AircraftType variable
        airlang_intg aircraft_idx = find_variable("AircraftType");
        if (aircraft_idx == -1) {
            // Try alternative names
            aircraft_idx = find_variable("AircraftID");
            if (aircraft_idx == -1) {
                printf("AIRCRAFT ERROR: No aircraft type specified in flight plan\n");
                return 0;
            }
        }

        if (variables[aircraft_idx].type == STRING) {
            // Extract aircraft ID from string (e.g., "Boeing 747-400" -> "B747")
            const airlang_strg aircraft_name = variables[aircraft_idx].value.str_value;

            if (strstr(aircraft_name, "747-400") || strstr(aircraft_name, "Boeing 747")) {
                return findAircraftConfig("B747") != NULL;
            }
            else if (strstr(aircraft_name, "Cessna 172") || strstr(aircraft_name, "C172")) {
                return findAircraftConfig("C172") != NULL;
            }
            else if (strstr(aircraft_name, "Cherokee") || strstr(aircraft_name, "PA-28")) {
                return findAircraftConfig("PA28") != NULL;
            }
            else {
                // Try direct lookup with the string value
                return findAircraftConfig(aircraft_name) != NULL;
            }
        }

        printf("AIRCRAFT WARNING: Unknown aircraft type, using default calculations\n");
        return 0;
    }