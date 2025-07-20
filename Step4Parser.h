/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2024
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
# ECHO "[PARSER SCRIPT .........................]"
# ECHO "                                         "
*/


/*
************************************************************
* File name: Parser.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A32.
* Date: May 01 2023
* Professor: Paulo Sousa
* Purpose: This file is the main header for Parser (.h)
************************************************************
*/

#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef READER_H_
#include "Reader.h"
#endif
#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global vars */
static Token			lookahead;
extern BufferPointer	stringLiteralTable;
extern airlang_intg		line;
extern Token			tokenizer(airlang_void);
extern airlang_strg		keywordTable[KWT_SIZE];
static airlang_intg		syntaxErrorNumber = 0;

#define MIN_AIRCRAFT_REQ  200      // IN THE AIRCRAFT BLOCK FOR CRUISESPEED/FUEL ROUGH MINIMUM REQUIREMENT IF ITS BELOW THIS THROWS SAFETY WARNING. 

#define LANG_WRTE		"print&"
#define LANG_READ		"input&"
#define LANG_MAIN		"MAIN"
#define LANG_ENDMAIN    "ENDMAIN"
#define LANG_BRIEFING   "BRIEFING"
#define LANG_ENDBRIEFING "ENDBRIEFING"
#define LANG_DISPATCH	 "DISPATCH"
#define LANG_ENDDISPATCH "ENDDISPATCH"
#define LANG_REPORT		 "REPORT"
#define LANG_ENDREPORT   "ENDREPORT"
#define LANG_AIRCRAFT    "AIRCRAFT"
#define LANG_FLIGHT		 "FLIGHT"
#define LANG_ROUTE		 "ROUTE"

/* TO_DO: Create ALL constants for keywords (sequence given in table.h) */

/* Constants */
enum KEYWORDS {
	NO_ATTR = -1,
	KW_MAIN,		    /* KW00 */
	KW_BRIEFING,		/* KW01 */
	KW_AIRCRAFT,		/* KW02 */
	KW_FLIGHT,		/* KW03 */
	KW_ROUTE,		/* KW04 */
	KW_DISPATCH,	    /* KW05 */
	KW_IF,		    /* KW06 */
	KW_THEN,			/* KW07 */
	KW_ELSE,			/* KW08 */
	KW_ENDIF,		/* KW09 */
	KW_PRINT,		/* KW010 */
	KW_REPORT,		/* KW11 */
	KW_CALCULATE,	/* KW12 */
	KW_ENDBRIEFING,	/* KW13 */
	KW_ENDREPORT,	/* KW14 */
	KW_ENDDISPATCH,	/* KW15 */
	KW_ENDMAIN,		/* KW16 */
	KW_LOADSHEET,	/* KW17*/
	KW_ENDLOADSHEET, /* KW18*/
	KW_REQUEST,		/* KW19 */
	KW_METAR,		/* KW20 */
	KW_NOTAM,		/* KW21 */
	KW_FROM,		/* KW22 */
	KW_WITHCONFIG   /* KW23 */

};

/* TO_DO: Define the number of BNF rules */
#define NUM_BNF_RULES 32

/* Parser */
typedef struct parserData {
	airlang_intg parsHistogram[NUM_BNF_RULES];	/* Number of BNF Statements */
} ParserData, * pParsData;

/* Number of errors */
airlang_intg numParserErrors;

/* Scanner data */
ParserData psData;

/* Function definitions */
airlang_void startParser();
airlang_void matchToken(airlang_intg, airlang_intg);
airlang_void syncErrorHandler(airlang_intg);
airlang_void printError();
airlang_void printBNFData(ParserData psData);

/* List of BNF statements */
enum BNF_RULES {
	BNF_error,										/*  0: Error token */
	BNF_codeSession,								/*  1 */
	BNF_comment,									/*  2 */
	BNF_dataSession,								/*  3 */
	BNF_optVarListDeclarations,						/*  4 */
	BNF_optionalStatements,							/*  5 */
	BNF_outputStatement,							/*  6 */
	BNF_outputVariableList,							/*  7 */
	BNF_program,									/*  8 */
	BNF_statement,									/*  9 */
	BNF_statements,									/* 10 */
	BNF_statementsPrime,							/* 11 */
	BNF_optParams,									/* 12 */
	BNF_returnStatement,							/* 13 */
	BNF_mainBlock,
	BNF_briefingBlock,
	BNF_dispatchBlock,
	BNF_reportBlock,
	BNF_aircraftRecord, 
	BNF_flightRecord, 
	BNF_routeRecord,
	BNF_ifStatementRecord,
	BNF_reportRecord,
	BNF_loadsheetBlock,
	BNF_assignmentStatement,
	BNF_expression,
	BNF_term,
	BNF_factor,
	BNF_requestStatement,
	BNF_methodCall, 
	BNF_withConfigBlock, 
	BNF_optConfigList
};


/* TO_DO: Define the list of keywords */
static airlang_strg BNFStrTable[NUM_BNF_RULES] = {
	"BNF_error",
	"BNF_codeSession",
	"BNF_comment",
	"BNF_dataSession",
	"BNF_optVarListDeclarations",
	"BNF_optionalStatements",
	"BNF_outputStatement",
	"BNF_outputVariableList",
	"BNF_program",
	"BNF_statement",
	"BNF_statements",
	"BNF_statementsPrime",
	"BNF_optParams",
	"BNF_returnStatement",
	"BNF_mainBlock",
	"BNF_briefingBlock",
	"BNF_dispatchBlock", 
	"BNF_reportBlock",
	"BNF_aircraftRecord",
	"BNF_flightRecord",
	"BNF_routeRecord",
	"BNF_ifStatementRecord",
	"BNF_reportRecord",
	"BNF_loadsheetBlock",
	"BNF_assignmentStatement",
	"BNF_expression",
	"BNF_term",
	"BNF_factor",
	"BNF_requestStatement",
	"BNF_methodCall",
	"BNF_withConfigBlock",
	"BNF_optConfigList"

};

/* TO_DO: Place ALL non-terminal function declarations */
//airlang_void codeSession();
airlang_void comment();
//airlang_void dataSession();
airlang_void optVarListDeclarations();
airlang_void optionalStatements();
airlang_void outputStatement();
airlang_void outputVariableList();
airlang_void program();
airlang_void statement();
airlang_void statements();
airlang_void statementsPrime();
airlang_void optParams();
airlang_void paramList();

/*Airlang specific*/
airlang_void mainBlock();
airlang_void briefingBlock();
airlang_void dispatchBlock();

airlang_void aircraftRecord();
airlang_void flightRecord();
airlang_void routeRecord();
//airlang_void aircraftData();
airlang_void aircraftStructure();
airlang_void aircraftValue();
airlang_void flightData();
airlang_void flightStructure();
airlang_void flightValue();
airlang_void routeData(); 
airlang_void routeStructure(); 
airlang_void routeValue();


airlang_void requestStatement();
airlang_void requestList();
airlang_void ifStatement();
airlang_void reportStatement();
airlang_void reportCall();
airlang_void briefingContent();
//airlang_void printCurrentToken();
airlang_void performanceBlock();
airlang_void performanceContent();
//airlang_void assignmentStatement();
airlang_void expression();
airlang_void term();
airlang_void factor();

airlang_void methodCall();
airlang_void optwithConfigBlock();
airlang_void optionalConfigList();
airlang_void configAssignment();
airlang_void optConfigStatement();




#endif
