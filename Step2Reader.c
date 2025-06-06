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
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */

#include <ctype.h>
#include <string.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

#ifndef READER_H_
#include "Step2Reader.h"
#endif

/*
***********************************************************
* Function name: readerCreate
* Purpose: Creates the buffer reader according to capacity, increment
	factor and operational mode ('f', 'a', 'm')
* Author: Svillen Ranev / Paulo Sousa
* History/Versions: S22
* Called functions: calloc(), malloc()
* Parameters:
*   size = initial capacity
*   increment = increment factor
*   mode = operational mode
* Return value: bPointer (pointer to reader)
* Algorithm: Allocation of memory according to inicial (default) values.
* TODO ......................................................
*	- Adjust datatypes for your LANGUAGE.
*   - Use defensive programming
*	- Check boundary conditions
*	- Check flags.
*************************************************************
*/

BufferPointer readerCreate(airlang_intg size) {
	BufferPointer readerPointer = NULL; 

	/* TO_DO: Defensive programming: size */
	if (size <= 0) {
		errorPrint("Invalid size for buffer reader: %d ", size);
		return NULL;
	}

	/* TO_DO: readerPointer allocation */
	/* TO_DO: Defensive programming: readerPointer */
	/*readerPointer = calloc(1, sizeof(Buffer));*/
	readerPointer = (BufferPointer)calloc(1, sizeof(Buffer));
	if (readerPointer == NULL) {
		printf("Error");
		return NULL;
	}
	
	/* TO_DO: content allocation */
	airlang_strg content = (airlang_strg)malloc(size * sizeof(airlang_char));
	//readerPointer->content = (airlang_strg)malloc(size * sizeof(airlang_char));
	
	/* TO_DO: Defensive programming: content */
	if (content == NULL) {
		free(readerPointer);
		errorPrint("Error: Cannot allocate buffer content.");
		return NULL; 
	}
	readerPointer->content = content;
	
	/* TO_DO: Initialize the histogram */
	for (airlang_intg i = 0; i < NCHAR; ++i) {
		readerPointer->histogram[i] = 0;
	}
	/* TO_DO: Initialize errors */
	readerPointer->numReaderErrors = 0;

	/* TO_DO: Update the properties */
	readerPointer->size = size; 
	/* TO_DO: Initialize flags */
	/* TO_DO: The created flag must be signalized as EMP */
	readerPointer->flags.isEmpty = AirLang_TRUE;
	readerPointer->flags.isFull = AirLang_FALSE;
	readerPointer->flags.isRead = AirLang_FALSE;
	readerPointer->flags.isMoved = AirLang_FALSE;
	readerPointer->position.read = 0; 
	readerPointer->position.wrte = 0;
	readerPointer->position.mark = 0;
	readerPointer->checkSum = 0; 
	return readerPointer;
}


/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a char to buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   ch = char to be added
* Return value:
*	readerPointer (pointer to Buffer Reader)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

BufferPointer readerAddChar(BufferPointer const readerPointer, airlang_char ch) {
	airlang_strg tempReader = NULL;
	airlang_intg newSize = 0;

	/* TO_DO: Defensive programming */
	if (readerPointer == NULL || readerPointer->content == NULL) {
		return NULL;
	}
	if (ch < 0 || ch > NCHAR) {
		readerPointer->numReaderErrors++;
		return NULL;
	}


	if (readerPointer->position.wrte >= readerPointer->size) {

		newSize = readerPointer->size * 2;
		if (newSize > 0) {
			tempReader = realloc(readerPointer->content, newSize * sizeof(airlang_char));
			if (tempReader == NULL) {
				errorPrint("%s%s", "Error:  Cannot reallocate memory for Buffer Reader.\n");
				readerPointer->numReaderErrors++;
				return NULL;
			}
			if (tempReader != readerPointer->content) {
				readerPointer->flags.isMoved = AirLang_TRUE;
			}
			readerPointer->content = tempReader;
			readerPointer->size = newSize;
		}
		readerPointer->flags.isFull = AirLang_FALSE;
	}
	/* TO_DO: Buffer not full: set flag */
	/*if (readerPointer->size >= READER_MAX_SIZE) {
		readerPointer->flags.isFull = AirLang_TRUE;  //BUFFER  is not full acc to video will have to update flag struct
		readerPointer->numReaderErrors++;
		return NULL;
	}*/

	/* TO_DO: Add the char */
	readerPointer->content[readerPointer->position.wrte++] = ch;
	/* TO_DO: Updates histogram */
	readerPointer->histogram[(unsigned)ch]++;
	return readerPointer;
}


/*
***********************************************************
* Function name: readerClear
* Purpose: Clears the buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
airlang_boln readerClear(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL || readerPointer->content == NULL) {
		return AirLang_FALSE;
	}
	/* TO_DO: Adjust positions to zero */
	readerPointer->position.read = 0; 
	readerPointer->position.wrte = 0; 
	readerPointer->position.mark = 0; 

	/* TO_DO: Adjust flags original */
	readerPointer->flags.isEmpty = AirLang_TRUE;
	readerPointer->flags.isFull = AirLang_FALSE;
	readerPointer->flags.isRead = AirLang_FALSE;
	readerPointer->flags.isMoved = AirLang_FALSE;
	return AirLang_TRUE;
}

/*
***********************************************************
* Function name: readerFree
* Purpose: Releases the buffer address
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
airlang_boln readerFree(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) {
		return AirLang_FALSE;
	}
	/* Free memory (buffer/content) */
	if (readerPointer->content) {
		free(readerPointer->content);
	}
	free(readerPointer);
	return AirLang_TRUE;
}

/*
***********************************************************
* Function name: readerIsFull
* Purpose: Checks if buffer reader is full
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
airlang_boln readerIsFull(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) {
		return AirLang_FALSE; 
	}

	/* TO_DO: Check flag if buffer is FUL */

	return (readerPointer->flags.isFull == AirLang_TRUE) ? AirLang_TRUE: AirLang_FALSE;
}


/*
***********************************************************
* Function name: readerIsEmpty
* Purpose: Checks if buffer reader is empty.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
airlang_boln readerIsEmpty(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) {
		return AirLang_FALSE;
	}

	/* TO_DO: Check flag if buffer is EMP */

	return(readerPointer->flags.isEmpty == AirLang_TRUE) ? AirLang_TRUE : AirLang_FALSE;
}

/*
***********************************************************
* Function name: readerSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
airlang_boln readerSetMark(BufferPointer const readerPointer, airlang_intg mark) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL || mark <0 || mark > readerPointer->position.wrte) {
		return AirLang_FALSE;
	}
	/* TO_DO: Adjust mark */
	readerPointer->position.mark = mark; 
	return AirLang_TRUE;
}


/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
airlang_intg readerPrint(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming (including invalid chars) */
	if (readerPointer == NULL || readerPointer->content == NULL) {
		return 0; 
	}
	/* TO_DO: Print the buffer content */
	for (airlang_intg i = 0; i < readerPointer->position.wrte; ++i) {
		putchar(readerPointer->content[i]);
	}
	return readerPointer->position.wrte;
}

/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/


airlang_intg readerLoad(BufferPointer const readerPointer, airlang_strg fileName) {
	// TO_DO: Defensive programming 
	if (readerPointer == NULL || fileName == NULL) {
		return -1;
	}
	/* TO_DO: Loads the file */ 
	FILE* file = fopen(fileName, "r");
	if (file == NULL) {
		//errorPrint("Error: couldn't open file to read");
		readerPointer->numReaderErrors++;
		return -1;
	}
	// Clear the buffer before loading new content
	readerClear(readerPointer);

	/* TO_DO: Creates the string calling vigenereMem(fileName, STR_LANGNAME, DECYPHER)*/
	airlang_strg output = vigenereMem(fileName, STR_LANGNAME, DECYPHER);
	if (output == NULL) {
		readerPointer->numReaderErrors++;
		return -1;
	}

	// Add output to buffer and count successful adds
	airlang_intg size = (airlang_intg)strlen(output);
	airlang_intg count = 0;
	for (airlang_intg i = 0; i < size; i++) {
		if (readerAddChar(readerPointer, output[i]) == NULL) {
			break;
		}
		count++;
	}
	free(output); 

	return count; 
}


/*
***********************************************************
* Function name: readerRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
airlang_boln readerRecover(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) {
		return AirLang_FALSE; 
	}
	/* TO_DO: Adjust read and mark to zero */
	readerPointer->position.read = 0; 
	readerPointer->position.mark = 0;

	return AirLang_TRUE;
}


/*
***********************************************************
* Function name: readerRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
airlang_boln readerRetract(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) {
		return AirLang_FALSE;
	}
	/* TO_DO: Retract (return 1 pos read) */
	if (readerPointer->position.read > 0) {
		readerPointer->position.read--;
		return AirLang_TRUE;
	}
	return AirLang_FALSE;
}


/*
***********************************************************
* Function name: readerRestore
* Purpose: Resets the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
airlang_boln readerRestore(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) {
		return AirLang_FALSE; 
	}
	/* TO_DO: Restore read to mark */
	readerPointer->position.read = readerPointer->position.mark;
	return AirLang_TRUE;
}



/*
***********************************************************
* Function name: readerGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
airlang_char readerGetChar(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL || readerPointer->content == NULL) {
		return READER_TERMINATOR;
	}
	/* TO_DO: Returns size in the read position and updates read */
	if (readerPointer->position.read >= readerPointer->position.wrte) {
		return READER_TERMINATOR;
	}
	readerPointer->flags.isRead = AirLang_TRUE; 
	return readerPointer->content[readerPointer->position.read++];
}


/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
airlang_strg readerGetContent(BufferPointer const readerPointer, airlang_intg pos) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL || readerPointer->content == NULL) {
		return	NULL;
	}
	if (pos <0  || pos > readerPointer->position.wrte) {
		return NULL;
	}

	/* TO_DO: Return content (string) */

	return readerPointer->content + pos;
}

/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the value of getCPosition.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The read position offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
airlang_intg readerGetPosRead(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL ) {
		return READER_ERROR;
	}
	/* TO_DO: Return read */
	return readerPointer->position.read;
}


/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Write position
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
airlang_intg readerGetPosWrte(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) {
		return READER_ERROR;
	}
	/* TO_DO: Return wrte */
	return readerPointer->position.wrte;
}


/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Mark position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
airlang_intg readerGetPosMark(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) {
		return READER_ERROR;
	}
	/* TO_DO: Return mark */
	return readerPointer->position.mark;
}


/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
airlang_intg readerGetSize(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) {
		return 0; 
	}
	/* TO_DO: Return size */
	return readerPointer->size;
}

/*
***********************************************************
* Function name: readerGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Flags from Buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
airlang_byte readerGetFlags(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) {
		return READER_ERROR;
	}
	/* TO_DO: Return flags */
	airlang_byte result = 0; 
	if (readerPointer->flags.isEmpty) result |= 0x01; // bit 0
	if (readerPointer->flags.isFull)  result |= 0x02; // bit 1
	if (readerPointer->flags.isRead)  result |= 0x04; // bit 2
	if (readerPointer->flags.isMoved) result |= 0x08; // bit 3
	return result;
}

/*
***********************************************************
* Function name: readerShowStat
* Purpose: Shows the char statistic.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value: (Void)
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
airlang_void readerPrintStat(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) {
		return;
	}
	/* TO_DO: Print statistics */
	for (airlang_intg i = 0; i < NCHAR; ++i) {
		if (readerPointer->histogram[i] > 0) {
			//printf("char c: %d count: %d\t", i, readerPointer->histogram[i]);
			if (i == ' ') {
				printf("B[ ] = %d\t", readerPointer->histogram[i]);
			}
			else if (i == '\n') {
				printf("B[\\n] = %d\t", readerPointer->histogram[i]);
			}
			else if (i == '\t') {
				printf("B[\\t] = %d\t", readerPointer->histogram[i]);
			}
			else if (i >= 32 && i <= 126) {
				printf("B[%c] = %d\t", i, readerPointer->histogram[i]);
			}
			else {
				printf("B[%02X] = %d\t", i, readerPointer->histogram[i]);
			}
		}
	}
	printf("\n");

}

/*
***********************************************************
* Function name: readerNumErrors
* Purpose: Returns the number of errors found.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of errors.
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
airlang_intg readerNumErrors(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) {
		return 0; 
	}
	/* TO_DO: Return the number of errors */
	return readerPointer->numReaderErrors;
}

/*
***********************************************************
* Function name: readerChecksum
* Purpose: Sets the checksum of the reader (4 bits).
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	[None]
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

airlang_intg readerChecksum(BufferPointer readerPointer) {
	airlang_intg  checksum = 0;

	/* TO_DO: Defensive programming */
	if (readerPointer == NULL || readerPointer->content == NULL) {
		errorPrint("%s%s", "Invalid buffer reader or content ");
		return 0; 
	}
	/* TO_DO: Return the checksum (given by the content) */
	for (airlang_intg i = 0; i < readerPointer->position.wrte; ++i) {
		//checksum = (checksum + (unsigned char)readerPointer->content[i]) % 255;
		checksum += readerPointer->content[i];
	}
		//readerPointer->checkSum = (airlang_byte)checksum;
		//checksum += readerPointer->content[i];
	return checksum;
}
