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
************************************************************
* File name: Writer.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef WRITER_H_
#define WRITER_H_

#define MAX_VARS 100
#define MAX_EXPR_LEN 256
#define MAX_LINES 100

#define WRITE "PRINT"

#define EOS '\0'
#define ZERO 0.0
#define STREMPTY ""
#define LPAR '('
#define RPAR ')'
#define PLUS '+'
#define MINUS '-'
#define MULT '*'
#define DIV '/'

#define EQUALS '='
#define MOD '%'
#define SPACE ' '
#define TAB '\t'
#define NEWLINE '\n'
#define RETURN '\r'
#define QUOTES '"'
#define QUOTE '\''

#define TRUE "true"
#define FALSE "false"

typedef enum { NUMERIC, STRING, BOOLEAN, CHAR } VarType;

typedef struct {
    airlang_char name[32];
    VarType type;
    union {
        airlang_doub num_value;
        airlang_char str_value[256];
        airlang_intg bool_value; // For BOOLEAN type (1 for true, 0 for false)
        airlang_char char_value; // For CHAR type
    } value;
} Variable;

airlang_intg find_variable(const airlang_strg name);

airlang_void assign_string_variable(const airlang_strg name, const airlang_strg value);
const airlang_strg get_string_value(const airlang_strg name);

airlang_void handle_write(airlang_strg expression);
airlang_void process_file(const airlang_strg filename);
airlang_void process_content(airlang_strg content);

airlang_void calculate(const airlang_strg expression);
airlang_void assign_numeric_variable(const airlang_strg name, airlang_doub value);
airlang_doub evaluate_expression(const airlang_strg expr);
airlang_intg evaluate_condition(const airlang_strg condition);
airlang_intg handle_if_else(airlang_strg expression);
//airlang_doub get_numeric_value(const airlang_strg name);
//airlang_void assign_boolean_variable(const airlang_strg name, airlang_intg value);

#endif
