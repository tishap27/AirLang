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
# ECHO "[COMPILER SCRIPT .......................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Compilers.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A1-A5.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file defines the functions called by main function.
* Function list: mainReader(), mainScanner(), mainParser().
*************************************************************/
//#include <crtdbg.h>  // added this to check memory leaks

#ifndef COMPILERS_H_
#define COMPILERS_H_

#define DEBUG 0

/*
 * ............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: This file must be updated according to each assignment
 * (see the progression: reader > scanner > parser).
 * ............................................................................
 */

/* AirLang */
#define STR_LANGNAME	"AirLang"

/* Logical constants - adapt for your language */
#define AirLang_TRUE  1        //afirmative
#define AirLang_FALSE 0		  // negative 

#ifndef ERROR
#define ERROR (-1)
#endif // !1



#define INVALID NULL
#define EOF_CHAR '\0'
#define EARTH_RADIUS 6371  //km
#define PI 3.1415926
#define DEG_TO_RAD (PI/180.0)


/*
------------------------------------------------------------
Data types definitions
NOTE: Some types may not be directly used by your language,
		but they can be necessary for conversions.
------------------------------------------------------------
*/
/* TO_DO: Define your typedefs */
typedef char			airlang_char;
typedef char*			airlang_strg;
typedef int				airlang_intg;
typedef float			airlang_real;
typedef void			airlang_void;

typedef unsigned char	airlang_boln;
typedef unsigned char	airlang_byte;

typedef long			airlang_long;
typedef double			airlang_doub;

typedef airlang_strg    airlang_airport;   // Airport code (e.g., "YOW")
typedef airlang_strg    airlang_aircraft;  // Aircraft type or registration (e.g., "B747", "N123AB")
typedef airlang_real    airlang_speed;     // Speed in knots
typedef airlang_real    airlang_altitude;  // Altitude in feet
typedef airlang_real    airlang_distance;  // Distance in nautical miles
typedef airlang_real    airlang_eta;       // Estimated time of arrival (hours)
typedef airlang_strg    airlang_status;    // Flight status (e.g., "delayed", "on_time")
typedef airlang_strg    airlang_weather;   // Weather condition (e.g., "clear", "storm")
typedef airlang_strg    airlang_date;      // Date string (e.g., "2025-05-14")



// fuel calc , W/B might be a tricky CG = MOMENT*WEIGHT (Weight will need acc distance from arm) . no need  to get that done now 

/* 
------------------------------------------------------------
Coordinates Structure
------------------------------------------------------------
*/

typedef struct Coordinates {
	airlang_real latitude; 
	airlang_real longitude; 
} coord;


/*
------------------------------------------------------------
AirportInformation  Structure
------------------------------------------------------------
*/
typedef struct airportInformation {
	airlang_airport code; 
	airlang_strg name; 
	coord location; 
}airport;

/*
------------------------------------------------------------
AircraftInformation  Structure
------------------------------------------------------------
*/
typedef struct aircraftInfomation {
	airlang_aircraft registration; 
	airlang_strg type; 
	airlang_real fuelCapacity; 
	airlang_intg cruiseSpeed; 
	airlang_real fuelPerNM;
}aircraft;

/*
------------------------------------------------------------
flightInformation  Structure
------------------------------------------------------------
*/
typedef struct flightInfo {
	airlang_date date; 
	airlang_real time; 
	airlang_status status; 
	airlang_weather weather; 
	airlang_eta eta; 
}flight;



/*
------------------------------------------------------------
Programs:
1: Coder - invokes Main1Coder code
2: Reader - invokes Main2Reader code
3: Scanner - invokes Main3Scanner code
4: Parser - invokes Main4Parser code
5: Writer - invokes Main5Writer code
6: Generator - invokes Main6Generator code
7: VM   -   invokes Main7Vm code 
------------------------------------------------------------
*/
enum PROGRAMS {
	PGM_CDR = '1', // Coder:	05pt
	PGM_RDR = '2', // Reader:	05pt
	PGM_SCN = '3', // Scanner:	15pt
	PGM_PSR = '4', // Parser:	15pt
	PGM_WRT = '5', // Writer:	10pt
	PGM_GEN = '6',  // Generator
	PGM_VM  = '7',  // Virtual Machine
};

/*
------------------------------------------------------------
Main functions signatures
(Code will be updated during next assignments)
------------------------------------------------------------
*/

extern airlang_intg runVirtualMachine(const airlang_strg bytecode_file);
airlang_intg main1Coder	(airlang_intg argc, airlang_strg* argv);
airlang_intg main2Reader(airlang_intg argc, airlang_strg* argv);
airlang_intg main3Scanner(airlang_intg argc, airlang_strg* argv);
airlang_intg main4Parser(airlang_intg argc, airlang_strg* argv);
airlang_intg main5Writer(airlang_intg argc, airlang_strg* argv);
airlang_intg main6Generator(airlang_intg argc, airlang_strg* argv);
airlang_intg main7Vm(airlang_intg argc, airlang_strg* argv);


airlang_void printLogo();
airlang_void errorPrint(airlang_strg fmt, ...);

#endif

/*
	  " args": [
		1,
		1,
		"README.txt",
		"CODED.txt"
	  ]
	  "args": [
		4,
		"CODED.txt"
	  ]
*/
