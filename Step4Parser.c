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
* File name: Parser.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A32.
* Date: May 01 2023
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef SCANNER_H_
#include "Step3Scanner.h"
#endif

#ifndef PARSER_H_
#include "Step4Parser.h"
#endif

/* Parser data */
extern ParserData psData; /* BNF statistics */

/*
************************************************************
 * Process Parser
 ***********************************************************
 */
/* TO_DO: This is the function to start the parser - check your program definition */

airlang_void startParser() {
	/* TO_DO: Initialize Parser data */
	airlang_intg i = 0;
	for (i = 0; i < NUM_BNF_RULES; i++) {
		psData.parsHistogram[i] = 0;
	}
	/* Proceed parser */
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*
 ************************************************************
 * Match Token
 ***********************************************************
 */
/* TO_DO: This is the main code for match - check your definition */
airlang_void matchToken(airlang_intg tokenCode, airlang_intg tokenAttribute) {
	airlang_intg matchFlag = 1;
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType != tokenAttribute)
			matchFlag = 0;

	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
	}
	if (matchFlag && lookahead.code == SEOF_T)
		return;
	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		syncErrorHandler(tokenCode);
}

/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */
/* TO_DO: This is the function to handler error - adjust basically datatypes */
airlang_void syncErrorHandler(airlang_intg syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*
 ************************************************************
 * Print Error
 ***********************************************************
 */
/* TO_DO: This is the function to error printing - adjust basically datatypes */
airlang_void printError() {
	extern numParserErrors;			/* link to number of errors (defined in Parser.h) */
	Token t = lookahead;
	//printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	//printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("*ERROR*: %s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MNID_T:
		printf("MNID_T:\t\t%s\t\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("STR_T: %s\n", readerGetContent(stringLiteralTable, t.attribute.contentString));
		break;
	case KW_T:
		printf("KW_T: %s\n", keywordTable[t.attribute.codeType]);
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case LBR_T:
		printf("LBR_T\n");
		break;
	case RBR_T:
		printf("RBR_T\n");
		break;
	case EOS_T:
		printf("NA\n");
		break;
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
		numParserErrors++; // Updated parser error
	}
}

/*
 ************************************************************
 * Program statement
 * BNF: <program> -> main& { <opt_statements> }
 * FIRST(<program>)= {CMT_T, MNID_T (main&), SEOF_T}.
 ***********************************************************
 */
airlang_void program() {
	psData.parsHistogram[BNF_program]++;
	switch (lookahead.code) {
	case CMT_T:
		comment();
	case KW_T:
		switch (lookahead.attribute.codeType) {
		case KW_MAIN:
			mainBlock();
			break;
		case KW_BRIEFING:
			briefingBlock();
			break;
		case KW_AIRCRAFT:
			aircraftRecord();
			break;
		case KW_FLIGHT:
			flightRecord();
			break;
		case KW_ROUTE:
			routeRecord();
			break;
		case KW_DISPATCH:
			dispatchBlock();
			break;
		case KW_REPORT:
			reportBlock();
			break;
		default:
			printError();
			break;
		}
		break;


	case MNID_T:
		if (strncmp(lookahead.attribute.idLexeme, LANG_MAIN, 5) == 0) {
			matchToken(MNID_T, NO_ATTR);
			matchToken(LBR_T, NO_ATTR);
			//optParams();
			dataSession();
			codeSession();
			matchToken(RBR_T, NO_ATTR);
			break;
		}
		else {
			printError();
		}
	case SEOF_T:
		; // Empty
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}

/*
 ************************************************************
 * comment
 * BNF: comment
 * FIRST(<comment>)= {CMT_T}.
 ***********************************************************
 */
airlang_void comment() {
	psData.parsHistogram[BNF_comment]++;
	matchToken(CMT_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Comment parsed");
}


/*
 ************************************************************
 * optParams
 * BNF: <optParams> -> <paramList> | e
 * FIRST(<optParams>) = { e, KW_T (KW_int), KW_T (KW_real), KW_T (KW_string)}.
 ***********************************************************
 */
airlang_void optParams() {
	psData.parsHistogram[BNF_optParams]++;
	switch (lookahead.code) {
	case CMT_T:
		comment();
	case KW_T:
		paramList();
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional param list parsed");
}

/*
 ************************************************************
 * paramList
 * BNF: <paramList> -> <opt_varlist_declarations>
 * FIRST(<paramList>) = { KW_T (KW_int), KW_T (KW_real), KW_T (KW_string)}.
 ***********************************************************
 */
airlang_void paramList() {
	psData.parsHistogram[BNF_optParams]++;
	switch (lookahead.attribute.codeType) {
	default:
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Param list parsed");
}

/*
 ************************************************************
 * dataSession
 * BNF: <dataSession> -> data { <opt_varlist_declarations> }
 * FIRST(<program>)= {KW_T (KW_data)}.
 ***********************************************************
 */
airlang_void dataSession() {
	psData.parsHistogram[BNF_dataSession]++;
	switch (lookahead.code) {
	case CMT_T:
		comment();
	default:
		matchToken(KW_T, KW_data);
		matchToken(LBR_T, NO_ATTR);
		optVarListDeclarations();
		matchToken(RBR_T, NO_ATTR);
		printf("%s%s\n", STR_LANGNAME, ": Data Session parsed");
	}
}

/*
 ************************************************************
 * Optional Var List Declarations
 * BNF: <opt_varlist_declarations> -> <varlist_declarations> | e
 * FIRST(<opt_varlist_declarations>) = { e, KW_T (KW_int), KW_T (KW_real), KW_T (KW_string)}.
 ***********************************************************
 */
airlang_void optVarListDeclarations() {
	psData.parsHistogram[BNF_optVarListDeclarations]++;
	switch (lookahead.code) {
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional Variable List Declarations parsed");
}

/*
 ************************************************************
 * codeSession statement
 * BNF: <codeSession> -> code { <opt_statements> }
 * FIRST(<codeSession>)= {KW_T (KW_code)}.
 ***********************************************************
 */
airlang_void codeSession() {
	psData.parsHistogram[BNF_codeSession]++;
	switch (lookahead.code) {
	case CMT_T:
		comment();
	default:
		matchToken(KW_T, KW_code);
		matchToken(LBR_T, NO_ATTR);
		optionalStatements();
		matchToken(RBR_T, NO_ATTR);
		printf("%s%s\n", STR_LANGNAME, ": Code Session parsed");
	}
}

/* TO_DO: Continue the development (all non-terminal functions) */

/*
 ************************************************************
 * Optional statement
 * BNF: <opt_statements> -> <statements> | ϵ
 * FIRST(<opt_statements>) = { ϵ , IVID_T, FVID_T, SVID_T, KW_T(KW_if),
 *				KW_T(KW_while), MNID_T(print&), MNID_T(input&) }
 ***********************************************************
 */
airlang_void optionalStatements() {
	psData.parsHistogram[BNF_optionalStatements]++;
	switch (lookahead.code) {
	case CMT_T:
		comment();
	case MNID_T:
		if ((strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) ||
			(strncmp(lookahead.attribute.idLexeme, LANG_READ, 6) == 0)) {
			statements();
			break;
		}
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}

/*
 ************************************************************
 * Statements
 * BNF: <statements> -> <statement><statementsPrime>
 * FIRST(<statements>) = { IVID_T, FVID_T, SVID_T, KW_T(KW_if),
 *		KW_T(KW_while), MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
airlang_void statements() {
	psData.parsHistogram[BNF_statements]++;
	statement();
	statementsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}

/*
 ************************************************************
 * Statements Prime
 * BNF: <statementsPrime> -> <statement><statementsPrime> | ϵ
 * FIRST(<statementsPrime>) = { ϵ , IVID_T, FVID_T, SVID_T, 
 *		KW_T(KW_if), KW_T(KW_while), MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
airlang_void statementsPrime() {
	psData.parsHistogram[BNF_statementsPrime]++;
	switch (lookahead.code) {
	case MNID_T:
		if (strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) {
			statements();
			break;
		}
	default:
		; //empty string
	}
}

/*
 ************************************************************
 * Single statement
 * BNF: <statement> -> <assignment statement> | <selection statement> |
 *	<iteration statement> | <input statement> | <output statement>
 * FIRST(<statement>) = { IVID_T, FVID_T, SVID_T, KW_T(KW_if), KW_T(KW_while),
 *			MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
airlang_void statement() {
	psData.parsHistogram[BNF_statement]++;
	switch (lookahead.code) {
	case KW_T:
		switch (lookahead.attribute.codeType) {
		default:
			printError();
		}
		break;
	case MNID_T:
		if (strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) {
			outputStatement();
		}
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

/*
 ************************************************************
 * Output Statement
 * BNF: <output statement> -> print& (<output statementPrime>);
 * FIRST(<output statement>) = { MNID_T(print&) }
 ***********************************************************
 */
airlang_void outputStatement() {
	psData.parsHistogram[BNF_outputStatement]++;
	matchToken(MNID_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	outputVariableList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}

/*
 ************************************************************
 * Output Variable List
 * BNF: <opt_variable list> -> <variable list> | ϵ
 * FIRST(<opt_variable_list>) = { IVID_T, FVID_T, SVID_T, ϵ }
 ***********************************************************
 */
airlang_void outputVariableList() {
	psData.parsHistogram[BNF_outputVariableList]++;
	switch (lookahead.code) {
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		break;
	default:
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": Output variable list parsed");
}

/*
 ************************************************************
 * The function prints statistics of BNF rules
 * Param:
 *	- Parser data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */
/*
airlang_void printBNFData(ParserData psData) {
}
*/
airlang_void printBNFData(ParserData psData) {
	/* Print Parser statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_BNF_RULES; cont++) {
		if (psData.parsHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", BNFStrTable[cont], "]=", psData.parsHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}
/*
************************************************************
* AIRLANG mainBlock() Function
* Purpose: Parse the MAINBLOCK 
* Grammar: <main_block> -> "MAIN"   "{" <content> "}"   "ENDMAIN" ";"
************************************************************
*/

airlang_void mainBlock() {
	/* Update parser statistics */
	psData.parsHistogram[BNF_mainBlock]++;

	/* Match "MAIN" keyword */
	matchToken(KW_T, KW_MAIN);

	/* Match opening brace "{" */
	matchToken(LBR_T, NO_ATTR);


	briefingBlock();                // <briefing_block>
	dispatchBlock();
	/* YET TO Parse content inside MAIN block */
	/*rigth now ONLY find closing brace */
	/* Later  briefingBlock() and dispatchBlock() xyz  */

	/* Match closing brace "}" */
	matchToken(RBR_T, NO_ATTR);

	/* Match "ENDMAIN" keyword */
	matchToken(KW_T, KW_ENDMAIN);

	/* Match semicolon ";" */
	matchToken(EOS_T, NO_ATTR);

	/* Print successful parsing message */
	printf("%s%s\n", STR_LANGNAME, ": MAIN block parsed successfully");
}


// <briefing_block> ::= "BRIEFING" "{" <aircraft_block> <flight_block> <route_block> "}" "ENDBRIEFING" ";"
airlang_void briefingBlock() {
	psData.parsHistogram[BNF_briefingBlock]++;
	matchToken(KW_T, KW_BRIEFING);
	matchToken(LBR_T, NO_ATTR);

	aircraftRecord();
	flightRecord();
	routeRecord();

	matchToken(RBR_T, NO_ATTR);
	matchToken(KW_T, KW_ENDBRIEFING);
	matchToken(EOS_T, NO_ATTR);
	printf("%s: Briefing block parsed\n", STR_LANGNAME);
}

// <dispatch_block> ::= "DISPATCH" "{" <dispatch_block> <report block> //later "}" "ENDDISPATCH" ";"
airlang_void dispatchBlock() {
	psData.parsHistogram[BNF_dispatchBlock]++;
	matchToken(KW_T, KW_DISPATCH);
	matchToken(LBR_T, NO_ATTR);

	reportBlock();

	matchToken(RBR_T, NO_ATTR);
	matchToken(KW_T, KW_ENDDISPATCH);
	matchToken(EOS_T, NO_ATTR);
	printf("%s: Dispatch block parsed\n", STR_LANGNAME);
}
// <report_block> ::= "REPORT" "{"  "}" "ENDREPORT" ";"
airlang_void reportBlock() {
	psData.parsHistogram[BNF_reportBlock]++;
	matchToken(KW_T, KW_REPORT);
	matchToken(LBR_T, NO_ATTR);
	//will have to lookahead later 
	matchToken(RBR_T, NO_ATTR);
	matchToken(KW_T, KW_ENDREPORT);
	matchToken(EOS_T, NO_ATTR);
	printf("%s: Report block parsed\n", STR_LANGNAME);
}

airlang_void aircraftRecord() {
	psData.parsHistogram[BNF_aircraftRecord]++;
	matchToken(KW_T, KW_AIRCRAFT);
	matchToken(LBR_T, NO_ATTR);

	aircraftData();

	matchToken(RBR_T, NO_ATTR);
	printf("%s: AIRCRAFT RECORD parsed\n", STR_LANGNAME);
}
airlang_void aircraftData() {
	/* Parse multiple assignment statements */
	while (lookahead.code == ID_T) {
		aircraftStructure();
	}
}
airlang_void aircraftStructure() {
	// Match identifier like AircraftID, Type so on... 
	matchToken(ID_T, NO_ATTR);

	// Match colon 
	matchToken(COLON_T, NO_ATTR);

	// Match value (could be identifier, string, or number) 
	aircraftValue();

	// Match semicolon
	matchToken(EOS_T, NO_ATTR);
}
airlang_void aircraftValue() {
	switch (lookahead.code) {
	case AIRCRAFT_ID_T:     // Aircraft ID values like C-GHPQ 
		matchToken(AIRCRAFT_ID_T, NO_ATTR);
		break;
	case ID_T:     // Identifier values if for eg any id is typed Clear that will also work 
		matchToken(ID_T, NO_ATTR);
		break;
	case STR_T:     // String values like "B747" 
		matchToken(STR_T, NO_ATTR);
		break;
	case INT_T:     // Integer values like 26000 
		matchToken(INT_T, NO_ATTR);
		break;
	case FLOAT_T:     // Float values like 1.5 
		matchToken(FLOAT_T, NO_ATTR);
		break;
	default:
		printError();
		break;
	}
}



airlang_void flightRecord() {
	psData.parsHistogram[BNF_flightRecord]++;
	matchToken(KW_T, KW_FLIGHT);
	matchToken(LBR_T, NO_ATTR);

	matchToken(RBR_T, NO_ATTR);
	printf("%s: FLIGHT RECORD parsed\n", STR_LANGNAME);
}


airlang_void routeRecord() {
	psData.parsHistogram[BNF_routeRecord]++;
	matchToken(KW_T, KW_ROUTE);
	matchToken(LBR_T, NO_ATTR);

	matchToken(RBR_T, NO_ATTR);

	printf("%s: ROUTE RECORD parsed\n", STR_LANGNAME);
}
