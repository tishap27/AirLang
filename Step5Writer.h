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

#define MAX_VARS 500
#define MAX_EXPR_LEN 1024
#define MAX_LINES 500

//#define PI 3.14159265358979323846
#define EARTH_RADIUS_KM 6371.0
#define km_to_nm 0.539957

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

void trimWhitespace(char* str);
void safe_concat(char* dest, size_t dest_size, const char* src);
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

airlang_intg is_aircraft_identifier(const airlang_strg value);

airlang_intg is_date_format(const airlang_strg value);
airlang_intg is_coordinate_format(const airlang_strg value);
airlang_intg parse_coordinates(const airlang_strg coord_str, airlang_doub* lat, airlang_doub* lon);



// Math functions for distance calculation
airlang_doub my_sin(airlang_doub x);
airlang_doub my_cos(airlang_doub x);
airlang_doub my_sqrt(airlang_doub n);
airlang_doub my_atan2(airlang_doub y, airlang_doub x);
airlang_doub toRadians(airlang_doub degree);

// Distance calculation functions
airlang_doub calculateAirpath(airlang_doub lat1, airlang_doub lon1, airlang_doub lat2, airlang_doub lon2);
//airlang_doub calculate_distance(const airlang_strg coord1_name, const airlang_strg coord2_name);

// Enhanced expression evaluation
airlang_doub evaluate_expression_with_distance(const airlang_strg expr);
airlang_doub calcLastLegDistance();

airlang_void handleRequestStatement(airlang_strg expression);
airlang_intg validateRequestStatement(const airlang_strg expression);
airlang_void parseMetar(const airlang_strg metar_string, const airlang_strg station_id);
airlang_void handle_metar_assignment(airlang_strg expression);
airlang_void windInfo(const airlang_strg metar, const airlang_strg station_id);
airlang_void visibilityInfo(const airlang_strg metar, const airlang_strg station_id);
airlang_void altimeterInfo(const airlang_strg metar, const airlang_strg station_id);
airlang_void temperatureInfo(const airlang_strg metar, const airlang_strg station_id);

//WIND CALCULATION
airlang_doub headwind(airlang_doub windDirection , airlang_doub windSpeed , airlang_doub runwayHdg);
airlang_doub crosswind(airlang_doub windDirection, airlang_doub windSpeed, airlang_doub runwayHdg);
//airlang_intg is_flight_number(const airlang_strg value);

//airlang_intg is_aircraft_id(const airlang_strg value);

//airlang_doub get_numeric_value(const airlang_strg name);
//airlang_void assign_boolean_variable(const airlang_strg name, airlang_intg value);

#endif
