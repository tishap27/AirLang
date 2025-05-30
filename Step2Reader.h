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
# ECHO "[READER SCRIPT ..........................]"
# ECHO "                                         "
*/


/*
************************************************************
* File name: Reader.h
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

#ifndef READER_H_
#define READER_H_

/* TIP: Do not change pragmas, unless necessary .......................................*/
/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Constants about controls (not need to change) */
#define READER_ERROR				(-1)	/* General error message */
#define READER_TERMINATOR			'\0'	/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME .................................. */

/* TO_DO: You should add your own constant definitions here */
#define READER_MAX_SIZE	1000					/* maximum capacity */ 

#define READER_DEFAULT_SIZE			250		/* default initial buffer reader capacity */

/* Add your bit-masks constant definitions here */
/* BITS                                    (  76543210) */
#define READER_DEFAULT_FLAG 0x00 		/* (0b00000000) = (0x00)_16 = (000)_10 */

/* TO_DO: BIT 3: END = End of buffer flag */
#define READER_SET_FLAG_END 0x08       // Bit 3 : 00001000
/* TO_DO: BIT 2: REL = Rellocation memory flag */
#define READER_SET_FLAG_REL 0x04       //Bit 2 :  00000100 
/* TO_DO: BIT 1: EMP = Buffer empty flag */
#define READER_SET_FLAG_FUL 0x02      // Bit 1 : 00000010
/* TO_DO: BIT 0: FUL = Buffer full flag */
#define READER_SET_FLAG_EMP 0x01     // Bit 0 : 00000001 no flags set ( doesnt mean empty)

#define NCHAR				128			/* Chars from 0 to 127 */

#define CHARSEOF			(-1)		/* EOF Code for Reader */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME .................................. */

/* TODO: Adjust datatypes */

/* Offset declaration */
typedef struct position {
	airlang_intg read;			/* the offset to the get a char position (in chars) */
	airlang_intg wrte;			/* the offset to the add chars (in chars) */
	airlang_intg mark;			/* the offset to the mark position (in chars) */
} Position;

typedef struct flag {
	airlang_boln isEmpty; /* checks if there is no content */
	airlang_boln isFull; /* the content is using all size */
	airlang_boln isRead; /* all content was read */
	airlang_boln isMoved; /* the content was moved in reallocation */
} Flag;

/* Buffer structure */
typedef struct bufferReader {
	airlang_strg		content;			/* pointer to the beginning of character array (character buffer) */
	airlang_intg		size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	Flag		flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position		position;			/* Offset / position field */
	airlang_intg		histogram[NCHAR];	/* Statistics of chars */
	airlang_intg		numReaderErrors;	/* Number of errors from Reader */
	airlang_byte		checkSum;			/* Sum of bytes(chars) */
} Buffer, *BufferPointer;

/* FUNCTIONS DECLARATION:  .................................. */

/* General Operations */
BufferPointer	readerCreate		(airlang_intg);
BufferPointer	readerAddChar		(BufferPointer const, airlang_char);
airlang_boln		readerClear		    (BufferPointer const);
airlang_boln		readerFree		    (BufferPointer const);
airlang_boln		readerIsFull		(BufferPointer const);
airlang_boln		readerIsEmpty		(BufferPointer const);
airlang_boln		readerSetMark		(BufferPointer const, airlang_intg);
airlang_intg		readerPrint		    (BufferPointer const);
airlang_intg		readerLoad			(BufferPointer const, airlang_strg);
airlang_boln		readerRecover		(BufferPointer const);
airlang_boln		readerRetract		(BufferPointer const);
airlang_boln		readerRestore		(BufferPointer const);
airlang_intg		readerChecksum		(BufferPointer const);
/* Getters */
airlang_char		readerGetChar		(BufferPointer const);
airlang_strg		readerGetContent	(BufferPointer const, airlang_intg);
airlang_intg		readerGetPosRead	(BufferPointer const);
airlang_intg		readerGetPosWrte	(BufferPointer const);
airlang_intg		readerGetPosMark	(BufferPointer const);
airlang_intg		readerGetSize		(BufferPointer const);
airlang_byte		readerGetFlags		(BufferPointer const);
airlang_void		readerPrintStat		(BufferPointer const);
airlang_intg		readerNumErrors		(BufferPointer const);

#endif
