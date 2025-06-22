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
# ECHO "[CODER SCRIPT ..........................]"
# ECHO "                                         "
*/
/*
************************************************************
* File name: Scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2024
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Step2Reader.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1  /* Value for run-time error */

/* TO_DO: Define the number of tokens */
#define NUM_TOKENS 15

/* TO_DO: Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	MNID_T,		/*  1: Method name identifier token (start: &) */
	ID_T ,
	INT_T,		/*  2: Integer literal token */
	STR_T,		/*  3: String literal token */
	LPR_T,		/*  4: Left parenthesis token */
	RPR_T,		/*  5: Right parenthesis token */
	LBR_T,		/*  6: Left brace token */
	RBR_T,		/*  7: Right brace token */
	KW_T,		/*  8: Keyword token */
	EOS_T,		/*  9: End of statement (semicolon) */
	RTE_T,		/* 10: Run-time error token */
	SEOF_T,		/* 11: Source end-of-file token */
	CMT_T, 		/* 12: Comment token */
	COLON_T
};

/* TO_DO: Define the list of keywords */
static airlang_strg tokenStrTable[NUM_TOKENS] = {
	"ERR_T",
	"MNID_T",
	"ID_T",
	"INT_T",
	"STR_T",
	"LPR_T",
	"RPR_T",
	"LBR_T",
	"RBR_T",
	"KW_T",
	"EOS_T",
	"RTE_T",
	"SEOF_T",
	"CMT_T",
	"COLON_T"
};

/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* TO_DO: Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	airlang_intg codeType;      /* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	airlang_intg intValue;				/* integer literal attribute (value) */
	airlang_intg keywordIndex;			/* keyword index in the keyword table */
	airlang_intg contentString;			/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	airlang_real floatValue;				/* floating-point literal attribute (value) */
	airlang_char idLexeme[VID_LEN + 1];	/* variable identifier token attribute */
	airlang_char errLexeme[ERR_LEN + 1];	/* error token attribite */
} TokenAttribute;

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	airlang_byte flags;			/* Flags information */
	union {
		airlang_intg intValue;				/* Integer value */
		airlang_real floatValue;			/* Float value */
		airlang_strg stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	airlang_intg code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

/* Scanner */
typedef struct scannerData {
	airlang_intg scanHistogram[NUM_TOKENS];	/* Statistics of chars */
} ScannerData, * pScanData;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* TO_DO: Define lexeme FIXED classes */
/* EOF definitions */
#define EOS_CHR '\0'	// CH00
#define EOF_CHR 0xFF	// CH01
#define UND_CHR '_'		// CH02
//#define AMP_CHR '&'		// CH03
#define QUT_CHR '"'		// CH04
#define HST_CHR '%'		// CH05
#define TAB_CHR '\t'	// CH06
#define SPC_CHR ' '		// CH07
#define NWL_CHR '\n'	// CH08
#define SCL_CHR ';'		// CH09
#define LPR_CHR '('		// CH10
#define RPR_CHR ')'		// CH11
#define LBR_CHR '{'		// CH12
#define RBR_CHR '}'		// CH13
#define COLON_CHR ':'     // CH 14


/*  Special case tokens processed separately one by one in the token-driven part of the scanner:
 *  LPR_T, RPR_T, LBR_T, RBR_T, EOS_T, SEOF_T and special chars used for tokenis include _, & and ' */


/* TO_DO: Error states and illegal state */
#define ESNR	8		/* Error state with no retract */
#define ESWR	9		/* Error state with retract */
#define FS		11		/* Illegal state */

 /* TO_DO: State transition table definition */
#define NUM_STATES		11
#define CHAR_CLASSES	8

/* TO_DO: Transition table - type of states defined in separate table */
static airlang_intg transitionTable[NUM_STATES][CHAR_CLASSES] = {
/*    [A-z],[0-9],    _,    &,   \', SEOF,    %, other
	   L(0), D(1), U(2), M(3), Q(4), E(5), C(6),  O(7) */
	{     1,   10, ESNR, ESNR,    4, ESWR,	  6, ESNR},	// S0: NOAS
	{     1,    1,    1,    2,	  3,    3,   3,    3},	// S1: NOAS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS},	// S2: ASNR (MVID)
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS},	// S3: ASWR (KEY)
	{     4,    4,    4,    4,    5, ESWR,	  4,    4},	// S4: NOAS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS},	// S5: ASNR (SL)
	{     6,    6,    6,    6,    6, ESWR,	  7,    6},	// S6: NOAS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS},	// S7: ASNR (COM)
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS},	// S8: ASNR (ES)
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS}, // S9: ASWR (ER)
	{    FS,  10,  FS,   FS,   FS,   FS,	 FS,   FS  } // S10: ASWR (IL) - New state for integers
};

/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

/* TO_DO: Define list of acceptable states */
static airlang_intg stateType[NUM_STATES] = {
	NOFS, /* 00 */
	NOFS, /* 01 */
	FSNR, /* 02 (MID) - Methods */
	FSWR, /* 03 (KEY) */
	NOFS, /* 04 */
	FSNR, /* 05 (SL) */
	NOFS, /* 06 */
	FSNR, /* 07 (COM) */
	FSNR, /* 08 (Err1 - no retract) */
	FSWR,  /* 09 (Err2 - retract) */
	FSNR  /* 10 (IL) - New state for integer literals */
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
airlang_intg			startScanner(BufferPointer psc_buf);
static airlang_intg	nextClass(airlang_char c);					/* character class function */
static airlang_intg	nextState(airlang_intg, airlang_char);		/* state machine function */
airlang_void			printScannerData(ScannerData scData);
Token				tokenizer(airlang_void);

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(airlang_strg lexeme);

/* Declare accepting states functions */ /*can i declare funcAircraftID? , fncflightId?*/
Token funcSL	(airlang_strg lexeme);
Token funcIL	(airlang_strg lexeme);
Token funcID	(airlang_strg lexeme);
Token funcCMT   (airlang_strg lexeme);
Token funcKEY	(airlang_strg lexeme);
Token funcErr	(airlang_strg lexeme);

/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

/* TO_DO: Define final state table */
static PTR_ACCFUN finalStateTable[NUM_STATES] = {
	NULL,		/* -    [00] */
	NULL,		/* -    [01] */
	funcID,		/* MNID	[02] */
	funcID,	    /* KEY  [03] */
	NULL,		/* -    [04] */
	funcSL,		/* SL   [05] */
	NULL,		/* -    [06] */
	funcCMT,	/* COM  [07] */
	funcErr,	/* ERR1 [06] */
	funcErr,		/* ERR2 [07] */
	funcIL		/* IL   [10] - New function for integer literals */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */
#define KWT_SIZE 17

/* TO_DO: Define the list of keywords */
static airlang_strg keywordTable[KWT_SIZE] = {
	"MAIN",		    /* KW00 */
	"BRIEING",		/* KW01 */
	"AIRCRAFT",		/* KW02 */
	"FLIGHT",		/* KW03 */
	"ROUTE",		/* KW04 */
	"DISPATCH",	    /* KW05 */
	"if",		    /* KW06 */
	"then",			/* KW07 */
	"else",			/* KW08 */
	"end",			/* KW09 */
	"print",		/* KW010 */
	"REPORT",		/* KW11 */
	"calculate"	,	/* KW12 */
	"endbriefing",	/* KW13 */
	"endreport"	,	/* KW14 */
	"enddispatch",	/* KW15 */
	"endmain",		/* KW16 */
};

/* NEW SECTION: About indentation */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT TAB_CHR  /* Tabulation */

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct languageAttributes {
	airlang_char indentationCharType;
	airlang_intg indentationCurrentPos;
	/* TO_DO: Include any extra attribute to be used in your scanner (OPTIONAL and FREE) */
} LanguageAttributes;

/* Number of errors */
airlang_intg numScannerErrors;

/* Scanner data */
ScannerData scData;

#endif
