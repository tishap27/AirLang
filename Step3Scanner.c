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
* File name: Scanner.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2024
* Purpose: This file contains all functionalities from Scanner.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

/* #define NDEBUG to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Step2Reader.h"
#endif

#ifndef SCANNER_H_
#include "Step3Scanner.h"
#endif

/*
----------------------------------------------------------------
TO_DO: Global vars definitions
----------------------------------------------------------------
*/

/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern BufferPointer stringLiteralTable;	/* String literal table */
airlang_intg line;								/* Current line number of the source code */
extern airlang_intg errorNumber;				/* Defined in platy_st.c - run-time error number */

extern airlang_intg stateType[NUM_STATES];
extern airlang_strg keywordTable[KWT_SIZE];

extern PTR_ACCFUN finalStateTable[NUM_STATES];
extern airlang_intg transitionTable[NUM_STATES][CHAR_CLASSES];

/* Local(file) global objects - variables */
static BufferPointer lexemeBuffer;			/* Pointer to temporary lexeme buffer */
static BufferPointer sourceBuffer;			/* Pointer to input source buffer */

/*
 ************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ***********************************************************
 */
 /* TO_DO: Follow the standard and adjust datatypes */

airlang_intg startScanner(BufferPointer psc_buf) {
	/* TO_DO: Start histogram */
	airlang_intg i = 0;
	for (i=0; i<NUM_TOKENS;i++)
		scData.scanHistogram[i] = 0;
	/* Basic scanner initialization */
	/* in case the buffer has been read previously  */
	readerRecover(psc_buf);
	readerClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS; /*0*/
}

/*
 ************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states 
 *		in the Transition Diagram).
 ***********************************************************
 */

Token tokenizer(airlang_void) {

	/* TO_DO: Follow the standard and adjust datatypes */

	Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	airlang_char c;			/* input symbol */
	airlang_intg state = 0;	/* initial state of the FSM */
	airlang_intg lexStart;	/* start offset of a lexeme in the input char buffer (array) */
	airlang_intg lexEnd;		/* end offset of a lexeme in the input char buffer (array)*/
	
	airlang_intg lexLength;	/* token length */
	airlang_intg i;			/* counter */
	///airlang_char newc;		// new char

	/* Starting lexeme */
	airlang_strg lexeme;	/* lexeme (to check the function) */
	lexeme = (airlang_strg)malloc(VID_LEN * sizeof(airlang_char));
	if (!lexeme)
		return currentToken;
	lexeme[0] = EOS_CHR;

	while (1) { /* endless loop broken by token returns it will generate a warning */
		c = readerGetChar(sourceBuffer);

		// TO_DO: Defensive programming
		if (c < 0 || c >= NCHAR)
			return currentToken;

		/* ------------------------------------------------------------------------
			Part 1: Implementation of token driven scanner.
			Every token is possessed by its own dedicated code
			-----------------------------------------------------------------------
		*/

		/* TO_DO: All patterns that do not require accepting functions */
		switch (c) {

			/* Cases for spaces */
		case SPC_CHR:
		case TAB_CHR:
			break;
		case NWL_CHR:
			line++;
			break;

			/* Cases for symbols */

		case SCL_CHR:
			currentToken.code = EOS_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case DEC_CHR:
			currentToken.code = DEC_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case COMMA_CHR:
			currentToken.code = COMMA_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case EQL_CHR:
			currentToken.code = EQL_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		
		case LPR_CHR:
			currentToken.code = LPR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case RPR_CHR:
			currentToken.code = RPR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case LBR_CHR:
			currentToken.code = LBR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case RBR_CHR:
			currentToken.code = RBR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
			/* Cases for END OF FILE */
		case EOS_CHR:
			currentToken.code = SEOF_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.seofType = SEOF_0;
			return currentToken;
		case (airlang_char)EOF_CHR:
			currentToken.code = SEOF_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.seofType = SEOF_255;
			return currentToken;
		case COLON_CHR:
			currentToken.code = COLON_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case NOT_CHR:
		{
			// Peek at next character to check for '='
			airlang_char next_c = readerGetChar(sourceBuffer);
			if (next_c == '=') {
				// It's a != operator
				currentToken.code = NOT_EQ_T;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;
			}
			else {
				// It's just a ! operator
				readerRetract(sourceBuffer); // Hence Put the character back
				currentToken.code = NOT_T;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;
			}
		}
		case SLCOM_CHR:
		{
			// Check if it's a comment (^^)
			airlang_char next_c = readerGetChar(sourceBuffer);
			if (next_c == '^') {
				// It's a comment
				lexStart = readerGetPosRead(sourceBuffer) - 2;
				readerSetMark(sourceBuffer, lexStart);

				// Read until newline or EOF
				while ((c = readerGetChar(sourceBuffer)) != NWL_CHR && c != EOF_CHR) {
					// Just consume characters
				}
				if (c == NWL_CHR) {
					line++;
					readerRetract(sourceBuffer); // Don't include newline in comment
				}

				lexEnd = readerGetPosRead(sourceBuffer);
				lexLength = lexEnd - lexStart;
				lexemeBuffer = readerCreate(lexLength + 2);
				if (!lexemeBuffer) {
					fprintf(stderr, "Scanner error: Cannot create buffer\n");
					exit(1);
				}
				readerRestore(sourceBuffer);
				for (i = 0; i < lexLength; i++) {
					readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
				}
				readerAddChar(lexemeBuffer, READER_TERMINATOR);
				lexeme = readerGetContent(lexemeBuffer, 0);
				currentToken = funcCMT(lexeme);
				readerRestore(lexemeBuffer);
				return currentToken;
			}
			else {
				// Just a caret character (not a comment)
				readerRetract(sourceBuffer); // Put the character back
				currentToken.code = ERR_T; // Or define a token type for caret if needed
				scData.scanHistogram[currentToken.code]++;
				return currentToken;
			}
		}
		break;
		case SLQUT_CHR:  // Single quote for dates
		{
			lexStart = readerGetPosRead(sourceBuffer) - 1;
			readerSetMark(sourceBuffer, lexStart);

			// Read until closing single quote
			while ((c = readerGetChar(sourceBuffer)) != '\'' && c != EOF_CHR) {
				if (c == NWL_CHR) line++;
			}

			if (c == EOF_CHR) {
				readerRetract(sourceBuffer);
				currentToken = funcErr(lexeme);
				return currentToken;
			}

			lexEnd = readerGetPosRead(sourceBuffer);
			lexLength = lexEnd - lexStart;
			lexemeBuffer = readerCreate(lexLength + 2);
			if (!lexemeBuffer) {
				fprintf(stderr, "Scanner error: Cannot create buffer\n");
				exit(1);
			}

			readerRestore(sourceBuffer);
			for (i = 0; i < lexLength; i++) {
				readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
			}
			readerAddChar(lexemeBuffer, READER_TERMINATOR);
			lexeme = readerGetContent(lexemeBuffer, 0);
			currentToken = funcDATE(lexeme);
			readerRestore(lexemeBuffer);
			return currentToken;
		}
		break;
		/* ------------------------------------------------------------------------
			Part 2: Implementation of Finite State Machine (DFA) or Transition Table driven Scanner
			Note: Part 2 must follow Part 1 to catch the illegal symbols
			-----------------------------------------------------------------------
		*/

		/* TO_DO: Adjust / check the logic for your language */

		default: // general case
			state = nextState(state, c);
			lexStart = readerGetPosRead(sourceBuffer) - 1;
			readerSetMark(sourceBuffer, lexStart);

			/* Special handling for numbers */
			if (isdigit(c) || c == '.' || c == '-') {
				int hasDigits = 0;
				int hasDecimal = 0;
				int isValidNumber = 1;
				if (c == '-' && !isdigit(readerGetChar(sourceBuffer))) {
					readerRetract(sourceBuffer);
					break;  // Not a number, just a minus operator
				}
				readerRetract(sourceBuffer);       //back to start

				while (1) {

					c = readerGetChar(sourceBuffer);

					/*FOR INVALID MNID */

					/*FOR INVALID NUMS*/
					if (isdigit(c)) {
						hasDigits = 1;
						state = nextState(state, c);
					}
					//if (isdigit(c) || c == '.') {
					//	state = nextState(state, c);
					//}
					else if (c == '.' && !hasDecimal) {
						hasDecimal = 1;
						state = nextState(state, c);
					}

					else if (isalpha(c)) {
						//  Found alphabet after digits - this is invalid!
						if (hasDigits) {
							// Continue reading the invalid token to get full lexeme
							while (isalnum(c) || c == '_') {
								c = readerGetChar(sourceBuffer);
							}
							readerRetract(sourceBuffer); // Put back the non-alphanumeric char

							// Create error token
							lexEnd = readerGetPosRead(sourceBuffer);
							lexLength = lexEnd - lexStart;
							lexemeBuffer = readerCreate((airlang_intg)lexLength + 2);
							if (!lexemeBuffer) {
								fprintf(stderr, "Scanner error: Can not create buffer\n");
								exit(1);
							}
							readerRestore(sourceBuffer);
							for (i = 0; i < lexLength; i++)
								readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
							readerAddChar(lexemeBuffer, READER_TERMINATOR);
							lexeme = readerGetContent(lexemeBuffer, 0);
							currentToken = funcErr(lexeme);
							readerRestore(lexemeBuffer);
							return currentToken;
						}
						else {
							// No digits seen yet, this is an identifier 
							readerRetract(sourceBuffer);
							break;
						}
					}

					else {
						readerRetract(sourceBuffer);
						break;
					}
				}
				// If we reach here, it's a valid number
				if (hasDigits) {
					lexEnd = readerGetPosRead(sourceBuffer);
					lexLength = lexEnd - lexStart;
					lexemeBuffer = readerCreate((airlang_intg)lexLength + 2);
					if (!lexemeBuffer) {
						fprintf(stderr, "Scanner error: Can not create buffer\n");
						exit(1);
					}
					readerRestore(sourceBuffer);
					for (i = 0; i < lexLength; i++)
						readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
					readerAddChar(lexemeBuffer, READER_TERMINATOR);
					lexeme = readerGetContent(lexemeBuffer, 0);
					currentToken = funcIL(lexeme);
					readerRestore(lexemeBuffer);
					return currentToken;
				}
			
			}
			else {
				/* Normal token processing */
				while (stateType[state] == NOFS) {
					c = readerGetChar(sourceBuffer);
					state = nextState(state, c);
				}
				if (stateType[state] == FSWR)
					readerRetract(sourceBuffer);
			}

			lexEnd = readerGetPosRead(sourceBuffer);
			lexLength = lexEnd - lexStart;
			lexemeBuffer = readerCreate((airlang_intg)lexLength + 2);
			if (!lexemeBuffer) {
				fprintf(stderr, "Scanner error: Can not create buffer\n");
				exit(1);
			}
			readerRestore(sourceBuffer);
			for (i = 0; i < lexLength; i++)
				readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
			readerAddChar(lexemeBuffer, READER_TERMINATOR);
			lexeme = readerGetContent(lexemeBuffer, 0);
			// TO_DO: Defensive programming
			if (!lexeme)
				return currentToken;
			currentToken = (*finalStateTable[state])(lexeme);
			readerRestore(lexemeBuffer);
			return currentToken;
		} // switch

	} //while

} // tokenizer


/*
 ************************************************************
 * Get Next State
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	(*) assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:
	(*) Assertion failed: test, file filename, line linenum.
	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUG is used - see the top of the file.
 ***********************************************************
 */
 /* TO_DO: Just change the datatypes */

airlang_intg nextState(airlang_intg state, airlang_char c) {
	airlang_intg col;
	airlang_intg next;
	col = nextClass(c);
	next = transitionTable[state][col];
	if (DEBUG)
		printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
	assert(next != FS);
	if (DEBUG)
		if (next == FS) {
			printf("Scanner Error: Illegal state:\n");
			printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
			exit(1);
		}
	return next;
}

/*
 ************************************************************
 * Get Next Token Class
	* Create a function to return the column number in the transition table:
	* Considering an input char c, you can identify the "class".
	* For instance, a letter should return the column for letters, etc.
 ***********************************************************
 */
/* TO_DO: Use your column configuration */

/* Adjust the logic to return next column in TT */
/*    [A-z],[0-9],    _,    &,   \', SEOF,    %, other
	   L(0), D(1), U(2), M(3), Q(4), E(5), C(6),  O(7) */

airlang_intg nextClass(airlang_char c) {
	airlang_intg val = -1;
	switch (c) {
	case UND_CHR:
		val = 2;
		break;
	/*case AMP_CHR:
		val = 3;
		break;*/
	case LPR_CHR:
		val = 3;
		break;
	case RPR_CHR:
		val = 8;
		break;
	case QUT_CHR:
		val = 4;
		break;
	case '\'':  // Single quote for dates
		val = 13;  // New class just for single quotes
		break;
	case HST_CHR:
		val = 6;
		break;
	case '.':
		val =  9;
		break; 
	case SCL_CHR:        
		val = 10;
		break;
	case '-':
		val = 11; 
		break; 
	case '^':
	{
		// Check for comment start (^^)
		airlang_char next_c = readerGetChar(sourceBuffer);
		readerRetract(sourceBuffer); // Put it back immediately

		if (next_c == '^') {
			val = 12; // Special class for single-line comment
		}
		else {
			val = 7; // Standalone caret (not a comment)
		}
		break;
	}
	case EOS_CHR:
	case (airlang_char) EOF_CHR:
		val = 5;
		break;
	default:
		if (isalpha(c))
			val = 0;
		else if (isdigit(c))
			val = 1;
		else
			val = 7;
	}
	return val;
}

/*
 ************************************************************
 * Acceptance State Function COM
 *		Function responsible to identify COM (comments).
 ***********************************************************
 */
 /* TO_DO: Adjust the function for IL */

Token funcCMT(airlang_strg lexeme) {
	Token currentToken = { 0 };
	airlang_intg i = 0, len = (airlang_intg)strlen(lexeme);
	currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);
	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == NWL_CHR)
			line++;
	}
	currentToken.code = CMT_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}


 /*
  ************************************************************
  * Acceptance State Function IL
  *		Function responsible to identify IL (integer literals).
  * - It is necessary respect the limit (ex: 2-byte integer in C).
  * - In the case of larger lexemes, error shoul be returned.
  * - Only first ERR_LEN characters are accepted and eventually,
  *   additional three dots (...) should be put in the output.
  ***********************************************************
  */
  /* TO_DO: Adjust the function for IL */
/*
Token funcIL(airlang_strg lexeme) {
	Token currentToken = { 0 };
	airlang_long tlong;
	double tfloat;
	char tempLexeme[NUM_LEN + 2]; // Enough space
	size_t len = strlen(lexeme);

	// If lexeme ends with a semicolon, strip it
	if (len > 0 && lexeme[len - 1] == ';') {
		strncpy(tempLexeme, lexeme, len - 1);
		tempLexeme[len - 1] = '\0';
		lexeme = tempLexeme;
		len--;
	}

		// Check if lexeme is all digits
		int isAllDigits = 1;
		for (size_t i = 0; i < len; ++i) {
			if (!isdigit(lexeme[i])) {
				isAllDigits = 0;
				break;
			}
		}

		if (isAllDigits && len > 0) {
			tlong = atol(lexeme);
			if (tlong >= 0 && tlong <= SHRT_MAX) {
				currentToken.code = INT_T;
				scData.scanHistogram[currentToken.code]++;
				currentToken.attribute.intValue = (airlang_intg)tlong;
			}
			else {
				// Out of range, handle as error
				currentToken = (*finalStateTable[ESNR])(lexeme);
			}
		}
		else {
			// Not all digits, treat as identifier or error
			currentToken.code = ID_T;
			strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN - 1);
			currentToken.attribute.idLexeme[VID_LEN - 1] = '\0';
		}
	
	return currentToken;
}
*/


//just check if int or float forget everything else
Token funcIL(airlang_strg lexeme) {
	Token currentToken = { 0 };
	char* dotPos = strchr(lexeme , '.');
	int isValid = 1;  //assuming intially valid 
	int decimalCount = 0;  // can only go to 1 
	int hasDigits = 0; 
	int hasLetters = 0; 


	// Check for multiple decimal points or invalid characters
	for (int i = 0; lexeme[i] != '\0'; i++) {
		if (lexeme[i] == '.') {
			decimalCount++;
			// If more than one decimal point, invalid
			if (decimalCount > 1) {
				isValid = 0;
				break;
			}
		}
		else if (isdigit(lexeme[i])) {
		hasDigits = 1 ; 
		}

		else if (lexeme[i] == '-' && i == 0) {
			// Allow minus sign only at the beginning
			continue;
		}
		else if (isalpha(lexeme[i])) {
			if (hasDigits) {
				isValid = 0;
				break;
			}
			hasLetters = 1;         //letters mixed with digits 
		}
		else {//if (!isdigit(lexeme[i])) {
			// If character is neither digit nor decimal point
			isValid = 0;
			break;
		}
	}

	// If letters appear after digits (e.g., 77ABC), it's invalid
	if (hasLetters && hasDigits) {
		isValid = 0;
	}
	if (!isValid) {
		currentToken = funcErr(lexeme);
	}
	/* Check if lexeme contains a decimal point */
	//char* dotPos = strchr(lexeme, '.');

	if (dotPos != NULL && isValid) {
		/* It's a float */
		currentToken.code = FLOAT_T;
		currentToken.attribute.floatValue = strtof(lexeme , NULL);
	}
	else if (dotPos == NULL && isValid){
		/* It's an integer */
		currentToken.code = INT_T;
		currentToken.attribute.intValue = atoi(lexeme);
	}
	else {
		//Invalid token format like 1.5.7 
		currentToken = funcErr(lexeme);
		return currentToken;
	}
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}

/*
************************************************************
 * Acceptance State Function DATE
 *      Function responsible to identify DATE literals.
 * - The lexeme must be stored in the String Literal Table
 *   (stringLiteralTable).
 ***********************************************************
 */
Token funcDATE(airlang_strg lexeme) {
	Token currentToken = { 0 };
	airlang_intg i = 0, len = (airlang_intg)strlen(lexeme);
	airlang_intg dateIndex = 0;

	// Extract date content (without quotes)
	for (i = 1; i < len - 1 && dateIndex < 10; i++) {
		currentToken.attribute.dateValue[dateIndex++] = lexeme[i];
	}
	currentToken.attribute.dateValue[dateIndex] = '\0';

	currentToken.code = DATE_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}

/*
Token funcIL(airlang_strg lexeme) {
	Token currentToken = { 0 };
	airlang_long tlong;
	double tfloat;
	char tempLexeme[NUM_LEN + 10]; // Increased buffer size
	size_t len = strlen(lexeme);

	// Copy lexeme to temp buffer for processing
	strncpy(tempLexeme, lexeme, sizeof(tempLexeme) - 1);
	tempLexeme[sizeof(tempLexeme) - 1] = '\0';

	// If lexeme ends with a semicolon, strip it
	if (len > 0 && tempLexeme[len - 1] == ';') {
		tempLexeme[len - 1] = '\0';
		len--;
	}

	// Check if lexeme contains a decimal point
	char* decimalPos = strchr(tempLexeme, '.');

	if (decimalPos != NULL) {
		// Handle floating-point number
		int isValidFloat = 1;

		// Check if all characters except decimal point are digits
		for (size_t i = 0; i < len; ++i) {
			if (tempLexeme[i] != '.' && !isdigit(tempLexeme[i])) {
				isValidFloat = 0;
				break;
			}
		}

		// Ensure there's at least one digit before and after decimal point
		if (decimalPos == tempLexeme || decimalPos == tempLexeme + len - 1) {
			isValidFloat = 0; // Decimal point at start or end
		}

		// Count decimal points (should be exactly one)
		int decimalCount = 0;
		for (size_t i = 0; i < len; ++i) {
			if (tempLexeme[i] == '.') decimalCount++;
		}
		if (decimalCount != 1) isValidFloat = 0;

		if (isValidFloat && len > 2) { // At least "x.y" format
			tfloat = atof(tempLexeme);
			currentToken.code = FLOAT_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.floatValue = (airlang_real)tfloat;
		}
		else {
			// Invalid float format, handle as error
			currentToken = (*finalStateTable[ESNR])(lexeme);
		}
	}
	else {
		// Handle integer
		// Check if lexeme is all digits
		int isAllDigits = 1;
		for (size_t i = 0; i < len; ++i) {
			if (!isdigit(tempLexeme[i])) {
				isAllDigits = 0;
				break;
			}
		}

		if (isAllDigits && len > 0) {
			tlong = atol(tempLexeme);
			if (tlong >= 0 && tlong <= SHRT_MAX) {
				currentToken.code = INT_T;
				scData.scanHistogram[currentToken.code]++;
				currentToken.attribute.intValue = (airlang_intg)tlong;
			}
			else {
				// Out of range, handle as error
				currentToken = (*finalStateTable[ESNR])(lexeme);
			}
		}
		else {
			// Not all digits, shouldn't reach here in IL function
			currentToken = (*finalStateTable[ESNR])(lexeme);
		}
	}
	return currentToken;
}

*/



/*
 ************************************************************
 * Acceptance State Function ID
 *		In this function, the pattern for IDs must be recognized.
 *		Since keywords obey the same pattern, is required to test if
 *		the current lexeme matches with KW from language.
 *	- Remember to respect the limit defined for lexemes (VID_LEN) and
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *    Remember to end each token with the \0.
 *  - Suggestion: Use "strncpy" function.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for ID */
/*Token funcID(airlang_strg lexeme) {
	Token currentToken = { 0 };
	size_t length = strlen(lexeme);
	airlang_char lastch = lexeme[length - 1];
	airlang_intg isID = AirLang_FALSE;
	switch (lastch) {
		case AMP_CHR:
			currentToken.code = MNID_T;
			scData.scanHistogram[currentToken.code]++;
			isID = AirLang_TRUE;
			break;
		default:
			// Test Keyword
			///lexeme[length - 1] = EOS_CHR;
			currentToken = funcKEY(lexeme);
			break;
	}
	if (currentToken.code == ERR_T) {               //isID == AirLang_TRUE
		currentToken.code = ID_T;
		scData.scanHistogram[currentToken.code]++;
		strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
		currentToken.attribute.idLexeme[VID_LEN] = EOS_CHR;
	}
	return currentToken;
}*/
Token funcID(airlang_strg lexeme) {
	Token currentToken = { 0 };
	size_t length = strlen(lexeme);

	/*SHOULD HAVE BOTH PARANTHESES*/
	if (strchr(lexeme, LPR_CHR) != NULL) { // CONTAINS (
		// Check for method identifier ending with exactly ()
		if (length >= 2 && lexeme[length - 1] == RPR_CHR && lexeme[length - 2] == LPR_CHR) {
			currentToken.code = MNID_T;
			scData.scanHistogram[currentToken.code]++;
			strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
			currentToken.attribute.idLexeme[VID_LEN] = EOS_CHR;
			return currentToken;
		}
		else {
			/*Invalid MNID*/
			currentToken = funcErr(lexeme);
			return currentToken;
		}
	}


	// Check if it is a keyword
	currentToken = funcKEY(lexeme);

	// If not a keyword, treat as regular identifier (ID_T)
	if (currentToken.code == ERR_T) {
		currentToken.code = ID_T;
		scData.scanHistogram[currentToken.code]++;
		strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
		currentToken.attribute.idLexeme[VID_LEN] = EOS_CHR;
	}

	return currentToken;
}






/*
************************************************************
 * Acceptance State Function SL
 *		Function responsible to identify SL (string literals).
 * - The lexeme must be stored in the String Literal Table 
 *   (stringLiteralTable). You need to include the literals in 
 *   this structure, using offsets. Remember to include \0 to
 *   separate the lexemes. Remember also to incremente the line.
 ***********************************************************
 */
/* TO_DO: Adjust the function for SL */

Token funcSL(airlang_strg lexeme) {
	Token currentToken = { 0 };
	airlang_intg i = 0, len = (airlang_intg)strlen(lexeme);

	/* Add tab separator if table is not empty */
	if (readerGetPosWrte(stringLiteralTable) > 0) {
		if (!readerAddChar(stringLiteralTable, '\t')) {  // Tab separator
			currentToken.code = RTE_T;
			return currentToken;
		}
	}

	currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);
	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == NWL_CHR)
			line++;
		if (!readerAddChar(stringLiteralTable, lexeme[i])) {
			currentToken.code = RTE_T;
			scData.scanHistogram[currentToken.code]++;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
	}
	if (!readerAddChar(stringLiteralTable, EOS_CHR)) {
		currentToken.code = RTE_T;
		scData.scanHistogram[currentToken.code]++;
		strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
		errorNumber = RTE_CODE;
		return currentToken;
	}
	currentToken.code = STR_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}


/*
************************************************************
 * This function checks if one specific lexeme is a keyword.
 * - Tip: Remember to use the keywordTable to check the keywords.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for Keywords */

Token funcKEY(airlang_strg lexeme) {
	Token currentToken = { 0 };
	airlang_intg kwindex = -1, j = 0;
	airlang_intg len = (airlang_intg)strlen(lexeme);
	///lexeme[len - 1] = EOS_CHR;
	for (j = 0; j < KWT_SIZE; j++)
		if (!strcmp(lexeme, &keywordTable[j][0]))
			kwindex = j;
	if (kwindex != -1) {
		currentToken.code = KW_T;
		scData.scanHistogram[currentToken.code]++;
		currentToken.attribute.codeType = kwindex;
	}
	else {
		currentToken = funcErr(lexeme);
	}
	return currentToken;
}


/*
************************************************************
 * Acceptance State Function Error
 *		Function responsible to deal with ERR token.
 * - This function uses the errLexeme, respecting the limit given
 *   by ERR_LEN. If necessary, use three dots (...) to use the
 *   limit defined. The error lexeme contains line terminators,
 *   so remember to increment line.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for Errors */

Token funcErr(airlang_strg lexeme) {
	Token currentToken = { 0 };
	airlang_intg i = 0, len = (airlang_intg)strlen(lexeme);
	if (len > ERR_LEN) {
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = EOS_CHR;
		strcat(currentToken.attribute.errLexeme, "...");
	}
	else {
		strcpy(currentToken.attribute.errLexeme, lexeme);
	}
	for (i = 0; i < len; i++)
		if (lexeme[i] == NWL_CHR)
			line++;
	currentToken.code = ERR_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}


/*
 ************************************************************
 * The function prints the token returned by the scanner
 ***********************************************************
 */

airlang_void printToken(Token t) {
	extern airlang_strg keywordTable[]; /* link to keyword table in */
	switch (t.code) {
	case RTE_T:
		printf("RTE_T\t\t%s", t.attribute.errLexeme);
		/* Call here run-time error handling component */
		if (errorNumber) {
			printf("%d", errorNumber);
			exit(errorNumber);
		}
		printf("\n");
		break;
	case ERR_T:
		printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MNID_T:
		printf("MNID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case ID_T:
		printf("ID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case INT_T:  // Added case for integer literals
		printf("INT_T\t\t%d\n", t.attribute.intValue);
		break;
	case DATE_T:
		printf("DATE_T\t\t%s\n", t.attribute.dateValue);
		break;
	case FLOAT_T:
	printf("FLOAT_T\t\t%g\n", t.attribute.floatValue);
	break;

	case STR_T:
		printf("STR_T\t\t%d\t ", (airlang_intg)t.attribute.codeType);
		printf("%s\n", readerGetContent(stringLiteralTable, (airlang_intg)t.attribute.codeType));
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
	case DEC_T:
		printf("DEC_T\n");
		break; 
	case COMMA_T:
		printf("COMMA_T\n");
		break;
	case EQL_T:
		printf("EQL_T\n");
		break;
	case NOT_EQ_T:
		printf("NOT_EQ_T\n");
		break;
	case KW_T:
		printf("KW_T\t\t%s\n", keywordTable[t.attribute.codeType]);
		break;
	case CMT_T:
		printf("CMT_T\n");
		break;
	case EOS_T:
		printf("EOS_T\n");
		break;
	case COLON_T:
		printf("COLON_T\n");
		break;
	default:
		printf("Scanner error: invalid token code: %d\n", t.code);
	}
}

/*
 ************************************************************
 * The function prints statistics of tokens
 * Param:
 *	- Scanner data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */
airlang_void printScannerData(ScannerData scData) {
	/* Print Scanner statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_TOKENS; cont++) {
		if (scData.scanHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", tokenStrTable[cont], "]=", scData.scanHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}

/*
TO_DO: (If necessary): HERE YOU WRITE YOUR ADDITIONAL FUNCTIONS (IF ANY).
*/
