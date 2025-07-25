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

 airlang_char currentFieldName[256];

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

	if (tokenCode == KW_T) {
		if (lookahead.code != KW_T || lookahead.attribute.codeType != tokenAttribute) {
			matchFlag = 0;
			printf("%s%s\n", STR_LANGNAME, ": ERROR - Incorrect keyword used");
		}
	}
	else {
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
	else {
		syncErrorHandler(tokenCode);
	}
}
	/*switch (lookahead.code) {
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
}*/

/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */
/* TO_DO: This is the function to handler error - adjust basically datatypes */
airlang_void syncErrorHandler(airlang_intg syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	numParserErrors++;
	while (1) {
		if (lookahead.code != syncTokenCode || lookahead.code == SEOF_T || lookahead.code == RBR_T) {
			//exit(syntaxErrorNumber);
		lookahead = tokenizer();
			break;
		}
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
	case MINUS_T:
		printf("MINUS_T:\n");
		break;
	case FLOAT_T:
		printf("FLOAT_T:\n");
		break;
	case DEC_T:
		printf("DEC_T:\n");
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

	while (lookahead.code == CMT_T) {
		comment();  // Consume all leading comments
	}
	while (lookahead.code == KW_T && lookahead.attribute.codeType == KW_REQUEST) {
		requestStatement();
	}
	while (lookahead.code == CMT_T) {
		comment();  // Consume all leading comments
	}
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
			//briefingContent();
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
	case KW_T:
		if (lookahead.attribute.codeType == KW_PRINT) {  
			statements();
			break;
		}
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
	case KW_T:
		if (lookahead.attribute.codeType == KW_PRINT) {  
			statements();
			break;
		}
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
		case KW_PRINT:  
			outputStatement();
			break;
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

	if (lookahead.code == KW_T && lookahead.attribute.codeType == KW_PRINT) {
		matchToken(KW_T, KW_PRINT);
		matchToken(LBR_T, NO_ATTR);
		outputVariableList();
		matchToken(RBR_T, NO_ATTR);
		matchToken(EOS_T, NO_ATTR);    //SEMICOLON MANDATORY AFTER PRINT STATEMENT FOR NOW 
		printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
	}
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

	printElement();

	while (lookahead.code == PLUS_T) {
		printf("%s: Concatenation operator parsed\n", STR_LANGNAME);
		matchToken(PLUS_T, NO_ATTR);
		printElement();
	}

	/*switch (lookahead.code) {
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		break;
	default:
		;
	}*/
	printf("%s%s\n", STR_LANGNAME, ": Output variable list parsed");
}


airlang_void printElement() {
	switch (lookahead.code) {
	case STR_T:     // String literals like "Fuel Capacity: "
		matchToken(STR_T, NO_ATTR);
		printf("%s: String literal parsed in print\n", STR_LANGNAME);
		break;
	case ID_T:      // Variables like FuelCapacity
		matchToken(ID_T, NO_ATTR);
		printf("%s: Variable parsed in print\n", STR_LANGNAME);
		break;
	case INT_T:     // Integer literals
		matchToken(INT_T, NO_ATTR);
		printf("%s: Integer literal parsed in print\n", STR_LANGNAME);
		break;
	case FLOAT_T:   // Float literals
		matchToken(FLOAT_T, NO_ATTR);
		printf("%s: Float literal parsed in print\n", STR_LANGNAME);
		break;
	case AIRCRAFT_ID_T:  // Aircraft IDs if they can be printed
		matchToken(AIRCRAFT_ID_T, NO_ATTR);
		printf("%s: Aircraft ID parsed in print\n", STR_LANGNAME);
		break;
	case FLIGHT_ID_T:    // Flight IDs if they can be printed
		matchToken(FLIGHT_ID_T, NO_ATTR);
		printf("%s: Flight ID parsed in print\n", STR_LANGNAME);
		break;
	default:
		printf("%s: ERROR - Invalid print element\n", STR_LANGNAME);
		printError();
		// Skip invalid token and continue
		lookahead = tokenizer();
		break;
	}
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

		while (lookahead.code != RBR_T && lookahead.code != SEOF_T) {
			if (lookahead.code == CMT_T) {
				comment();  // Handles comment
				continue;

			}
			briefingBlock();  // <briefing_block>
			
			if (lookahead.code == KW_T && lookahead.attribute.codeType == KW_LOADSHEET) {
				performanceBlock();
			}
			
			dispatchBlock();
			
			
		}
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
		printf("%s%s\n", STR_LANGNAME, ": MAIN block parsed ");
	
}


// <briefing_block> ::= "BRIEFING" "{" <aircraft_block> <flight_block> <route_block> "}" "ENDBRIEFING" ";"
airlang_void briefingBlock() {
	psData.parsHistogram[BNF_briefingBlock]++;
	matchToken(KW_T, KW_BRIEFING);
	matchToken(LBR_T, NO_ATTR);

	briefingContent();

	matchToken(RBR_T, NO_ATTR);
	//matchToken(KW_T, KW_ENDBRIEFING);
	//matchToken(EOS_T, NO_ATTR);


	if (lookahead.code == KW_T ) {
		if (lookahead.attribute.codeType != KW_ENDBRIEFING) {
			printf("%s: ERROR - Expected ENDBRIEFING but not found \n",
				STR_LANGNAME);//lookahead.attribute.errLexeme)
			syntaxErrorNumber++;
			// Don't consume the token - let syncErrorHandler recover
			syncErrorHandler(KW_ENDBRIEFING);
			return;
		}
		matchToken(KW_T, KW_ENDBRIEFING);
		matchToken(EOS_T, NO_ATTR);
	}
	else {
		printf("%s: ERROR - Missing ENDBRIEFING\n", STR_LANGNAME);
		syntaxErrorNumber++;
		syncErrorHandler(KW_ENDBRIEFING);
		return;
	}


	printf("%s: Briefing block parsed\n", STR_LANGNAME);
}
/*airlang_void printCurrentToken() {
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
}*/
airlang_void briefingContent() {
	//printf("DEBUG: Entering briefingContent\n");
	while (lookahead.code != RBR_T && lookahead.code != SEOF_T) {
		if (lookahead.code == KW_T) {
			switch (lookahead.attribute.codeType) {
			case KW_AIRCRAFT:
				aircraftRecord();
				break;
			case KW_FLIGHT:
				flightRecord();
				break;
			case KW_ROUTE:
				routeRecord();
				break;
			case KW_PRINT:
				optionalStatements();
				break;
			default:
				// Skip unknown keywords but continue parsing
				lookahead = tokenizer();
				break;
			}
		}
		else if (lookahead.code == CMT_T) {
			comment();  // Handle comments
		}
		else {
			// Skip unexpected tokens but continue parsing
			//printf("DEBUG: Unexpected token, skipping\n");
			lookahead = tokenizer();
		}
	}

}
airlang_void aircraftRecord() {
	psData.parsHistogram[BNF_aircraftRecord]++;
	//printf("DEBUG: Starting AIRCRAFT record\n");

	// Match AIRCRAFT keyword
	matchToken(KW_T, KW_AIRCRAFT);

	// Match opening brace
	matchToken(LBR_T, NO_ATTR);

	// Parse all aircraft fields
	while (lookahead.code != RBR_T && lookahead.code != SEOF_T) {
		//printCurrentToken();

		if (lookahead.code == ID_T || lookahead.code == AIRCRAFT_ID_T) {
			aircraftStructure();
		}
		else if (lookahead.code == CMT_T) {
			comment();
		}
		else if (lookahead.code == KW_T && lookahead.attribute.codeType == KW_PRINT) {
			outputStatement();
		}
		else {
			printf("DEBUG: Unexpected token in aircraft block, skipping\n");
			lookahead = tokenizer();
		}
	}

	// Match closing brace
	matchToken(RBR_T, NO_ATTR);
	printf("%s: AIRCRAFT RECORD parsed\n", STR_LANGNAME);
}
airlang_void aircraftStructure() {

	// Match field name
	if (lookahead.code == ID_T) {
		strcpy_s(currentFieldName, 256, lookahead.attribute.idLexeme);
		matchToken(ID_T, NO_ATTR);
	}
	else if (lookahead.code == AIRCRAFT_ID_T) {
		matchToken(AIRCRAFT_ID_T, NO_ATTR);
	}
	else {
		printf("DEBUG: Expected field name, got %d\n", lookahead.code);
		printError();
		return;
	}

	// Match colon
	matchToken(COLON_T, NO_ATTR);

	// Match value
	aircraftValue();

	// Match semicolon
	matchToken(EOS_T, NO_ATTR);
}
airlang_void aircraftValue() {

	switch (lookahead.code) {
	case AIRCRAFT_ID_T:
		matchToken(AIRCRAFT_ID_T, NO_ATTR);
		break;
	case ID_T:
		matchToken(ID_T, NO_ATTR);
		break;
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		break;
	case INT_T:
		if (lookahead.attribute.intValue <= MIN_AIRCRAFT_REQ) {   // defined in parser.h 
			printf("SAFETY WARNING: The registered %s value is set to %d --Please confirm value within certified limits\n",
				 currentFieldName, lookahead.attribute.intValue);
		}
		matchToken(INT_T, NO_ATTR);
		break;
	case FLOAT_T:
		matchToken(FLOAT_T, NO_ATTR);
		break;
	case BOOL_T:
		matchToken(BOOL_T, NO_ATTR);
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
	while(lookahead.code != RBR_T && lookahead.code != SEOF_T){
		if (lookahead.code == ID_T) {
			flightStructure();
		}
		else if (lookahead.code == KW_T && lookahead.attribute.codeType == KW_PRINT) {
			outputStatement(); 
		}
		else if (lookahead.code == CMT_T) {
			comment();  // Handle comments
		}
		else {
			// Skip unexpected tokens but continue parsing
			lookahead = tokenizer();
		}
	}
	//switch (lookahead.code) {
	//case ID_T:
		//while (lookahead.code == ID_T) {
		//	flightStructure();
		//}
		//break;
	//default:
		//; // Empty - optional content
	//}
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
	case BOOL_T:
		matchToken(BOOL_T, NO_ATTR);
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

	while (lookahead.code != RBR_T && lookahead.code != SEOF_T) {
		if (lookahead.code == ID_T) {
			routeStructure();
		}
		else if (lookahead.code == KW_T && lookahead.attribute.codeType == KW_PRINT) {
			outputStatement();  
		}
		else if (lookahead.code == CMT_T) {
			comment();  // Handle comments
		}
		else {
			// Skip unexpected tokens but continue parsing
			lookahead = tokenizer();
		}
	}

	/*switch (lookahead.code) {
	case ID_T:
		while (lookahead.code == ID_T) {
			routeStructure();
		}
		break;
	default:
		; // Empty - optional content
	}*/
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
	case BOOL_T:
		matchToken(BOOL_T, NO_ATTR);
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

	while (lookahead.code != RBR_T && lookahead.code != SEOF_T) {
		if (lookahead.code == KW_T) {
			switch (lookahead.attribute.codeType) {
			case KW_IF:
				ifStatement();
				break;
			case KW_REPORT:
				reportStatement();
				break;
			case KW_PRINT:
				outputStatement();
				break;
			default:
				// Skip unknown keywords but continue parsing
				lookahead = tokenizer();
				break;
			}
		}
		else if (lookahead.code == CMT_T) {
			comment();  // Handle comments
		}
		else {
			// Skip unexpected tokens but continue parsing
			lookahead = tokenizer();
		}
	}

	
	matchToken(RBR_T, NO_ATTR);
	matchToken(KW_T, KW_ENDDISPATCH);
	matchToken(EOS_T, NO_ATTR);
	printf("%s: Dispatch block parsed\n", STR_LANGNAME);
}

airlang_void ifStatement() {
	psData.parsHistogram[BNF_ifStatementRecord]++;
	matchToken(KW_T, KW_IF);

	matchToken(ID_T, NO_ATTR);


	//Currently only using != and > but if needed more symbol will just add in this block ad update scanner if not a token yet
	if (lookahead.code == NOT_EQ_T) {
		matchToken(NOT_EQ_T, NO_ATTR);
	}
	else if (lookahead.code == EQL_T) {
		matchToken(EQL_T, NO_ATTR);
	}
	else if (lookahead.code == GT_T) {
		matchToken(GT_T, NO_ATTR);
	}
	else {
		// Error handling - unexpected token
		printf("Expected comparison operator");
	}

	//matchToken(NOT_EQ_T, NO_ATTR);

	//after operator can compare with string or ID can also add number but eh not required in my AirLang right now 
	if (lookahead.code == ID_T) {
		matchToken(ID_T, NO_ATTR);
	}
	else if (lookahead.code == STR_T) {
		matchToken(STR_T, NO_ATTR);
	}
	else if (lookahead.code == BOOL_T) {
		matchToken(BOOL_T, NO_ATTR);
	}
	else {
		// Error handling - unexpected token
		printf("Expected identifier or string literal");
	}

	//matchToken(STR_T, NO_ATTR);

	matchToken(KW_T, KW_THEN);

	while (lookahead.code == KW_T && lookahead.attribute.codeType == KW_PRINT) {
		outputStatement();
	}

	//matchToken(KW_T, KW_PRINT);

	//matchToken(STR_T, NO_ATTR);
	//matchToken(EOS_T, NO_ATTR);


	if (lookahead.code == KW_T && lookahead.attribute.codeType == KW_ELSE) {
		matchToken(KW_T, KW_ELSE);
		//matchToken(KW_T, KW_PRINT);
		while (lookahead.code == KW_T && lookahead.attribute.codeType == KW_PRINT) {
			outputStatement();
		}
		//matchToken(STR_T, NO_ATTR);
		//matchToken(EOS_T, NO_ATTR);
		
	}

	//matchToken(KW_T, KW_ELSE);
	//matchToken(KW_T, KW_PRINT);

	//matchToken(STR_T, NO_ATTR);
	//matchToken(EOS_T, NO_ATTR);

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
	while (lookahead.code != RBR_T && lookahead.code != SEOF_T) {
		if (lookahead.code == MNID_T) {
			methodCall();
			//matchToken(MNID_T, NO_ATTR);
			//matchToken(EOS_T, NO_ATTR);
		}
		else if (lookahead.code == KW_T && lookahead.attribute.codeType == KW_PRINT) {
			outputStatement();  
		}
		else if (lookahead.code == CMT_T) {
			comment();  // Handle comments
		}
		else {
			// Skip unexpected tokens but continue parsing
			lookahead = tokenizer();
		}
	}
}

//DISTANCE() WITHCONFIG { BLA BLA };
airlang_void methodCall() {
	psData.parsHistogram[BNF_methodCall]++;

	matchToken(MNID_T, NO_ATTR);

	if (lookahead.code == KW_T && lookahead.attribute.codeType == KW_WITHCONFIG) {
		optwithConfigBlock();
	}

	matchToken(EOS_T, NO_ATTR);

	printf("%s: Method Call parsed\n", STR_LANGNAME);
}

airlang_void optwithConfigBlock() {
	psData.parsHistogram[BNF_withConfigBlock]++;

	matchToken(KW_T, KW_WITHCONFIG);

	matchToken(LBR_T, NO_ATTR);

	optionalConfigList();

	matchToken(RBR_T, NO_ATTR);

	printf("%s: Optional ConfigBlock parsed\n", STR_LANGNAME);
}

airlang_void optionalConfigList() {
	psData.parsHistogram[BNF_optConfigList]++;

	if (lookahead.code == ID_T) {
		configAssignment();

		while (lookahead.code == ID_T) {
			configAssignment();
		}
	}
}
airlang_void configAssignment() {

	if (lookahead.code == ID_T) {
		//printf("ID_T: %s\n", lookahead.attribute.idLexeme);
		matchToken(ID_T, NO_ATTR);
	}
	else {
		printError();
		return;
	}


	//printf("EQL_T: =\n");
	matchToken(EQL_T, NO_ATTR);

	//optConfigStatement();
	if (lookahead.code == ID_T) {
		matchToken(ID_T, NO_ATTR);

		if (lookahead.code == DEC_T) {
			matchToken(DEC_T, NO_ATTR);

			if (lookahead.code == ID_T) {
				//printf("ID_T: %s\n", lookahead.attribute.idLexeme);
				matchToken(ID_T, NO_ATTR);
			}
			else {
				printError();
			}
		}
	}


	matchToken(EOS_T, NO_ATTR);

	printf("%s: optional Config assignment parsed\n", STR_LANGNAME);
}

airlang_void optConfigStatement() {
	if (lookahead.code == ID_T) {
		//printf("ID_T: %s\n", lookahead.attribute.idLexeme);
		matchToken(ID_T, NO_ATTR);

		
		if (lookahead.code == DEC_T) {  
			matchToken(DEC_T, NO_ATTR);
			printf("ID_T: %s\n", lookahead.attribute.idLexeme);
			matchToken(ID_T, NO_ATTR); 
		}
	}
	else {
		switch (lookahead.code) {
		case INT_T:
			matchToken(INT_T, NO_ATTR);
			break;
		case FLOAT_T:
			matchToken(FLOAT_T, NO_ATTR);
			break;
		case STR_T:
			matchToken(STR_T, NO_ATTR);
			break;
		default:
			printError();
			break;
		}
	}
}


airlang_void performanceBlock() {
	psData.parsHistogram[BNF_loadsheetBlock]++;
	//printf("DEBUG: Entering performanceBlock\n");
	matchToken(KW_T, KW_LOADSHEET);
	matchToken(LBR_T, NO_ATTR);

	while (lookahead.code != RBR_T) {
		if (lookahead.code == ID_T) {
			performanceContent();
		}
		else if (lookahead.code == CMT_T) {
			comment();
		}
		else if (lookahead.code == KW_T) {
			switch (lookahead.attribute.codeType) {
			case KW_IF:
				ifStatement();
				break;
			case KW_PRINT:
				optionalStatements();
				break;
			default:
				// Skip unknown keywords but continue parsing
				lookahead = tokenizer();
				break;
			}
		}
		else {
			lookahead = tokenizer(); // skip unexpected tokens
		}
	}
	matchToken(RBR_T, NO_ATTR);
	matchToken(KW_T, KW_ENDLOADSHEET);
	matchToken(EOS_T, NO_ATTR);
	printf("%s: Loadsheet block parsed\n", STR_LANGNAME);
}
airlang_void performanceContent() {
	//printf("DEBUG: Entering performanceContent\n");
	matchToken(ID_T, NO_ATTR);

	// Equals sign
	if (lookahead.code == EQL_T) {
		matchToken(EQL_T, NO_ATTR);
	}
	else if (lookahead.code == COLON_T) {
		matchToken(COLON_T, NO_ATTR);
	}
	else {
		printError();
	}

	if (lookahead.code == KW_T && lookahead.attribute.codeType == KW_AIRPATH) {
		matchToken(KW_T, KW_AIRPATH);  // Handle AIRPATH keyword
		printf("%s: AIRPATH keyword parsed\n", STR_LANGNAME);
	}
	else {
		// Parse right-hand expression
		expression();
	}
	// Semicolon
	matchToken(EOS_T, NO_ATTR);
	printf("%s: Assignment statement parsed\n", STR_LANGNAME);

}

airlang_void expression() {
	//printf("DEBUG: Parsing expression\n");
	psData.parsHistogram[BNF_expression]++;
	term();
	while (lookahead.code == PLUS_T ) {
		printf("AirLang: Operator + parsed\n");
		//printf("Operator: %c\n", lookahead.code == PLUS_T ? '+' : '-');
		matchToken(PLUS_T, NO_ATTR);
		// matchToken(lookahead.code, NO_ATTR);
		term();
	}
}
airlang_void term() {
	//printf("DEBUG: Parsing term\n");
	psData.parsHistogram[BNF_term]++;
	factor();
	while (lookahead.code == MULTI_T || lookahead.code == DIV_T) {
		printf("AirLang: Operator %c parsed\n", lookahead.code == MULTI_T ? '*' : '/');
		matchToken(lookahead.code, NO_ATTR);
		factor();
	}
}

airlang_void factor() {
	//printf("DEBUG: Parsing factor\n");
	psData.parsHistogram[BNF_factor]++;
	switch (lookahead.code) {
	case ID_T:
		//printf("Variable: %s\n", lookahead.attribute.idLexeme);
		matchToken(ID_T, NO_ATTR);
		break;
	case INT_T:
		matchToken(INT_T, NO_ATTR);
		break;
	case FLOAT_T:
		matchToken(FLOAT_T, NO_ATTR);
		break;
	case LPR_T:  // Handling (-70.07) expressions
		matchToken(LPR_T, NO_ATTR);
		//matchToken(INT_T, NO_ATTR);
		if (lookahead.code == INT_T || lookahead.code == FLOAT_T) {
			//printf("%d\n",lookahead.code == INT_T ? INT_T : FLOAT_T);
			matchToken(lookahead.code, NO_ATTR);
		}
		else {
			printf("Error: Expected number inside parentheses\n");
			printError();
		}
		matchToken(RPR_T, NO_ATTR);  //  closing
		break;
	case KW_T:  
		if (lookahead.attribute.codeType == KW_AIRPATH) {
			matchToken(KW_T, KW_AIRPATH);
			printf("%s: AIRPATH keyword in expression\n", STR_LANGNAME);
		}
		else {
			printf("Error: Unexpected keyword in expression\n");
			printError();
		}
		break;
	default:
		printf("Error: Unexpected factor\n");
		printError();
		break;
	}
}

//REQUEST METAR FROM "STRING"  
//Basically Import of Airlang 
airlang_void requestStatement() {
	psData.parsHistogram[BNF_requestStatement]++;
	
	matchToken(KW_T, KW_REQUEST);

	requestList();
	//matchToken(KW_T, KW_METAR);
	matchToken(KW_T, KW_FROM);
	matchToken(STR_T, NO_ATTR);  // URL
	matchToken(EOS_T, NO_ATTR);

	printf("%s: REQUEST statement parsed\n", STR_LANGNAME);


}
airlang_void requestList() {
	// will include more options that time switch case 
	if (lookahead.code == KW_T && (lookahead.attribute.codeType == KW_METAR || lookahead.attribute.codeType == KW_NOTAM)) {  
		matchToken(KW_T, lookahead.attribute.codeType);
	}
	else {
		printf("%s: Wrong keyword used\n", STR_LANGNAME);
		printError();
	}
}
