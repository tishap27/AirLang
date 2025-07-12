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
		case CMT_T:
        printf("CMT_T:\n");
        break;
		case DATE_T:
			printf("DATE_T:\n");
			break;

		case ID_T:
			printf("ID_T:\n");
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
		//program();
		
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
			briefingContent();
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
		//case KW_REPORT:
			//reportBlock();
			//break;
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
			//dataSession();
			//codeSession();
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

/*airlang_void dataSession() {
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
}*/

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
/*airlang_void codeSession() {
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
}*/

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

	briefingContent();

	matchToken(RBR_T, NO_ATTR);
	matchToken(KW_T, KW_ENDBRIEFING);
	matchToken(EOS_T, NO_ATTR);
	printf("%s: Briefing block parsed\n", STR_LANGNAME);
}
void printCurrentToken() {
	printf("DEBUG: Current token - Code: %d, ", lookahead.code);
	if (lookahead.code == ID_T || lookahead.code == MNID_T || lookahead.code == AIRCRAFT_ID_T) {
		printf("Lexeme: %s\n", lookahead.attribute.idLexeme);
	}
	else if (lookahead.code == KW_T) {
		printf("Keyword: %d\n", lookahead.attribute.codeType);
	}
	else {
		printf("Simple token\n");
	}
}
airlang_void briefingContent() {
	printf("DEBUG: Entering briefingContent\n");
	while (lookahead.code != RBR_T && lookahead.code != SEOF_T) {
		if (lookahead.code == KW_T) {
			switch (lookahead.attribute.codeType) {
			case KW_AIRCRAFT:
				printf("DEBUG: Found AIRCRAFT block\n");
				aircraftRecord();
				break;
			case KW_FLIGHT:
				printf("DEBUG: Found FLIGHT block\n");
				flightRecord();
				break;
			case KW_ROUTE:
				printf("DEBUG: Found ROUTE block\n");
				routeRecord();
				break;
			default:
				// Skip unknown keywords but continue parsing
				lookahead = tokenizer();
				break;
			}
		}
		else if (lookahead.code == CMT_T) {
			
			printf("DEBUG: Found comment\n");
			comment();  // Handle comments
		}
		else {
			// Skip unexpected tokens but continue parsing
			printf("DEBUG: Unexpected token, skipping\n");
			lookahead = tokenizer();
		}
	}
	printf("DEBUG: Exiting briefingContent\n");

}
airlang_void aircraftRecord() {
	psData.parsHistogram[BNF_aircraftRecord]++;
	printf("DEBUG: Starting AIRCRAFT record\n");

	// Match AIRCRAFT keyword
	matchToken(KW_T, KW_AIRCRAFT);
	printf("DEBUG: Matched AIRCRAFT keyword\n");

	// Match opening brace
	matchToken(LBR_T, NO_ATTR);
	printf("DEBUG: Matched {\n");

	// Parse all aircraft fields
	while (lookahead.code != RBR_T && lookahead.code != SEOF_T) {
		printCurrentToken();

		if (lookahead.code == ID_T || lookahead.code == AIRCRAFT_ID_T) {
			printf("DEBUG: Found aircraft field\n");
			aircraftStructure();
		}
		else if (lookahead.code == CMT_T) {
			printf("DEBUG: Found comment in aircraft block\n");
			comment();
		}
		else {
			printf("DEBUG: Unexpected token in aircraft block, skipping\n");
			lookahead = tokenizer();
		}
	}

	// Match closing brace
	matchToken(RBR_T, NO_ATTR);
	printf("%s: AIRCRAFT RECORD parsed\n", STR_LANGNAME);
	printf("DEBUG: Finished AIRCRAFT record\n");
}
airlang_void aircraftStructure() {
	printf("DEBUG: Parsing aircraft structure\n");

	// Match field name
	if (lookahead.code == ID_T) {
		printf("DEBUG: Found ID field: %s\n", lookahead.attribute.idLexeme);
		matchToken(ID_T, NO_ATTR);
	}
	else if (lookahead.code == AIRCRAFT_ID_T) {
		printf("DEBUG: Found AIRCRAFT_ID field: %s\n", lookahead.attribute.idLexeme);
		matchToken(AIRCRAFT_ID_T, NO_ATTR);
	}
	else {
		printf("DEBUG: Expected field name, got %d\n", lookahead.code);
		printError();
		return;
	}

	// Match colon
	matchToken(COLON_T, NO_ATTR);
	printf("DEBUG: Matched :\n");

	// Match value
	aircraftValue();
	printf("DEBUG: Matched value\n");

	// Match semicolon
	matchToken(EOS_T, NO_ATTR);
	printf("DEBUG: Matched ;\n");
}
airlang_void aircraftValue() {
	printf("DEBUG: Parsing aircraft value - token code: %d\n", lookahead.code);

	switch (lookahead.code) {
	case AIRCRAFT_ID_T:
		printf("DEBUG: Matching AIRCRAFT_ID value\n");
		matchToken(AIRCRAFT_ID_T, NO_ATTR);
		break;
	case ID_T:
		printf("DEBUG: Matching ID value\n");
		matchToken(ID_T, NO_ATTR);
		break;
	case STR_T:
		printf("DEBUG: Matching STR value\n");
		matchToken(STR_T, NO_ATTR);
		break;
	case INT_T:
		printf("DEBUG: Matching INT value\n");
		matchToken(INT_T, NO_ATTR);
		break;
	case FLOAT_T:
		printf("DEBUG: Matching FLOAT value\n");
		matchToken(FLOAT_T, NO_ATTR);
		break;
	default:
		printf("DEBUG: Invalid value token: %d\n", lookahead.code);
		printError();
		// Skip invalid value
		lookahead = tokenizer();
		break;
	}
}

airlang_void flightRecord() {
	psData.parsHistogram[BNF_flightRecord]++;
	matchToken(KW_T, KW_FLIGHT);
	matchToken(LBR_T, NO_ATTR);

	flightData();

	matchToken(RBR_T, NO_ATTR);
	printf("%s: FLIGHT RECORD parsed\n", STR_LANGNAME);
}
airlang_void flightData() {
	switch (lookahead.code) {
	case ID_T:
		while (lookahead.code == ID_T) {
			flightStructure();
		}
		break;
	default:
		; // Empty - optional content
	}
}
airlang_void flightStructure() {
	matchToken(ID_T, NO_ATTR); 
	matchToken(COLON_T, NO_ATTR); 

	flightValue(); 

	matchToken(EOS_T , NO_ATTR);

}
airlang_void flightValue() {
	switch (lookahead.code) {
	case FLIGHT_ID_T:     // FLIGHT ID values like C-GHPQ 
		matchToken(FLIGHT_ID_T, NO_ATTR);
		break;
	case DATE_T:     // DATE values 
		matchToken(DATE_T, NO_ATTR);
		break;
	case STR_T:     // String values like "CLEAR" 
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


airlang_void routeRecord() {
	psData.parsHistogram[BNF_routeRecord]++;
	matchToken(KW_T, KW_ROUTE);
	matchToken(LBR_T, NO_ATTR);

	routeData();

	matchToken(RBR_T, NO_ATTR);

	printf("%s: ROUTE RECORD parsed\n", STR_LANGNAME);
}
airlang_void routeData() {
	switch (lookahead.code) {
	case ID_T:
		while (lookahead.code == ID_T) {
			routeStructure();
		}
		break;
	default:
		; // Empty - optional content
	}
}
airlang_void routeStructure() {

	matchToken(ID_T, NO_ATTR); 
	matchToken(COLON_T, NO_ATTR);

	routeValue(); 

	matchToken(EOS_T, NO_ATTR); 
}
airlang_void routeValue() {
	
	switch (lookahead.code) {
	case STR_T:     // String values like "YOW" and  "Ottawa International Airport"
		matchToken(STR_T, NO_ATTR);
		break;
	case INT_T:     // Integer values even if i say 77 or 77.77 both should work .will try later with error file , but doesnt matter with my correct input file 
		matchToken(INT_T, NO_ATTR);
		// Check if this is part of a coordinate pair
		if (lookahead.code == COMMA_T) {
			matchToken(COMMA_T, NO_ATTR);
			// Expect another number after comma
			if (lookahead.code == INT_T) {
				matchToken(INT_T, NO_ATTR);
			}
			else if (lookahead.code == FLOAT_T) {
				matchToken(FLOAT_T, NO_ATTR);
			}
		}
		break;
	case FLOAT_T:   // Float values like 45.3225, -75.6692
		matchToken(FLOAT_T, NO_ATTR);
		// Check if this is part of a coordinate pair
		if (lookahead.code == COMMA_T) {
			matchToken(COMMA_T, NO_ATTR);
			// Expect another FLOAT_T after comma (your scanner handles negative as part of float)
			if (lookahead.code == FLOAT_T) {
				matchToken(FLOAT_T, NO_ATTR);
			}
			else if (lookahead.code == INT_T) {
				matchToken(INT_T, NO_ATTR);
			}
		}
		break;
	default:
		printError();
		break;
	}
}

airlang_void dispatchBlock() {
	psData.parsHistogram[BNF_dispatchBlock]++;
	matchToken(KW_T, KW_DISPATCH);
	matchToken(LBR_T, NO_ATTR);

	//ifStatement();
	reportStatement();
	
	matchToken(RBR_T, NO_ATTR);
	matchToken(KW_T, KW_ENDDISPATCH);
	matchToken(EOS_T, NO_ATTR);
	printf("%s: Dispatch block parsed\n", STR_LANGNAME);
}

airlang_void ifStatement() {
	psData.parsHistogram[BNF_ifStatementRecord]++;
	matchToken(KW_T, KW_IF);

	matchToken(ID_T, NO_ATTR);

	matchToken(NOT_EQ_T, NO_ATTR);

	matchToken(STR_T, NO_ATTR);

	matchToken(KW_T, KW_THEN);

	matchToken(KW_T, KW_PRINT);

	matchToken(STR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);

	matchToken(KW_T, KW_ELSE);
	matchToken(KW_T, KW_PRINT);

	matchToken(STR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);

	matchToken(KW_T, KW_ENDIF);
	matchToken(EOS_T, NO_ATTR);

	printf("%s: IF block parsed\n", STR_LANGNAME);

}

airlang_void reportStatement() {
	psData.parsHistogram[BNF_reportRecord]++;
	
	matchToken(KW_T, KW_REPORT);
	matchToken(LBR_T, NO_ATTR);

	reportCall();

	matchToken(RBR_T, NO_ATTR);
	matchToken(KW_T, KW_ENDREPORT);
	matchToken(EOS_T, NO_ATTR);
	printf("%s: REPORT block parsed\n", STR_LANGNAME);
}
airlang_void reportCall() {

	while (lookahead.code == MNID_T) {
		matchToken(MNID_T, NO_ATTR);
		matchToken(EOS_T, NO_ATTR);
	}
	}