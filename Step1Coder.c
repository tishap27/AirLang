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
# ECHO "[CODER SCRIPT ..........................]"
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



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

/*
***********************************************************
* Function name: vigenereFile
* Purpose: Perform Vigenère cipher (encode or decode) on a file and write to output file.
* Parameters:
*   - inputFileName: Name of the input file (string)
*   - outputFileName: Name of the output file (string)
*   - key: AIRLANG (string)
*   - encode: CYPHER or DECYPHER mode (int) defined in step1coder.h
* Returns: void
* 
*   1. Opens input and output files
*   2. For each character in input file:
*      - If character is in the visible ASCII range, apply Vigenère transformation
*      - For encoding: shifts character forward based on current key letter
*      - For decoding: shifts character backward based on current key letter
*   3. Write transformed character to output file
*   4. Close both files
 ************************************************************
 */
void vigenereFile(const airlang_strg inputFileName, const airlang_strg outputFileName, const airlang_strg key, airlang_intg encode) {
	// TO_DO: Define the input and output files (ex: FILE* inputFile, FILE* outputFile)
    
    //check input parameters
    if (inputFileName == NULL || outputFileName == NULL || key == NULL) {
        errorPrint("Error: Invalid input parameters.\n");
        return;
    }
    if (strlen(key) == 0) {
        errorPrint("Error: Key cannot be empty.\n");
        return;
    }

    //opens the output file
    FILE* inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        //errorPrint("Error: Cannot open input file %s\n", inputFileName);
        return;
    }
   
    FILE* outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        errorPrint("Error: Cannot open output file %s\n", outputFileName);
        fclose(inputFile);
        return;
    }


	// Define local variables for cipher operation
    airlang_intg keyLen = (airlang_intg) strlen(key);        // getting the length of the key AIRLANG
    airlang_intg keyIndex = 0;                               //tracking the position in the key 
    airlang_intg ch;                                         //to store charc read from input

	
    /*
     * Vigenère Cipher Implementation:
     * For each character in visible ASCII range:
     * - When encoding: Add the numerical value of the key character (modulo ASCII_RANGE)
     * - When decoding: Subtract the numerical value of the key character (modulo ASCII_RANGE)
     *
     * The formula ensures we stay within the visible ASCII character range by:
     * 1. Normalizing the character to 0-based index from ASCII_START
     * 2. Applying the shift (adding or subtracting)
     * 3. Using modulo to wrap around within our range
     * 4. Converting back to ASCII by adding ASCII_START
     */

    while ((ch = fgetc(inputFile)) != EOF) {
        if (ch >= ASCII_START && ch <= ASCII_END) {
            int shift = key[keyIndex % keyLen];         //gets corresponding key character
            if (encode == CYPHER) {
                ch = ((ch - ASCII_START) + (shift - ASCII_START)) % ASCII_RANGE + ASCII_START;
            }
            else if (encode == DECYPHER) {
                ch = ((ch - ASCII_START) - (shift - ASCII_START) + ASCII_RANGE) % ASCII_RANGE + ASCII_START;
            }
            else {
                errorPrint("Error: Invalid encode parameter %d - Must be 1(CYPHER) or 0(DECYPHER)\n", encode);
                fclose(inputFile);
                fclose(outputFile);
                return;
            }
            keyIndex++;     // move to the next key 
        }
        fputc(ch, outputFile); 
    }

    // TO_DO: Close the files
    fclose(inputFile);
    fclose(outputFile);
}

/*
***********************************************************
* Function: vigenereMem
* Purpose : Perform Vigenère cipher (encode/decode) on a file and return result as string.
* Parameters: 
*    - inputFileName: input file name
*    - key : AIRLANG (cipher key str)
*    -encode: CYPHER or DECYPHER mode (int) defined in step1coder.h
* Returns : Pointer to result string (must be freed by caller)
* 
*   1. Opens input file and determines its size
*   2. Allocates memory for output string
*   3. For each character in input file:
*      - If character is in the visible ASCII range, apply Vigenère transformation
*      - For encoding: shifts character forward based on current key letter
*      - For decoding: shifts character backward based on current key letter
*   4. Stores transformed characters in memory buffer
*   5. Returns pointer to the buffer (caller must free)
************************************************************
*/
/* then finds the size of the file by moving the file pointer to EOF, then getting current position of fp via ftell 
and so finding fsize rewind file pointer back to the start of the file.
*/

airlang_strg vigenereMem(const airlang_strg inputFileName, const airlang_strg key, airlang_intg encode) {


    if (inputFileName == NULL || key == NULL) {
        errorPrint("Error: Invalid input parameters");
        return NULL;
    }

    if (strlen(key) == 0) {
        errorPrint("Error: Key cannot be empty");
        return NULL;
    }

    FILE* inputFile = fopen(inputFileName, "r");
    if (inputFile ==  NULL) {
        //errorPrint("Error: Cannot open input file %s", inputFileName);
        return NULL;
    }

    // Get file size by seeking to end, getting position, then rewinding
    if (fseek(inputFile, 0, SEEK_END) != 0) {
        errorPrint("Error: fseek failed on the file %s\n", inputFileName);
        fclose(inputFile);
        return NULL; 
    }

 
    // Get current position (size of file)
    airlang_intg fileSize = ftell(inputFile);
    if (fileSize < 0) {
        errorPrint("Error: ftell failed on the file %s\n", inputFileName);
        fclose(inputFile);
        return NULL;
    }
    rewind(inputFile);


    //allocating memory for output string +1 for nullterminator
    airlang_strg output = NULL;
    output = (airlang_strg)malloc(fileSize + 1);
    if (output == NULL) {
        errorPrint("Error: memory allocation failed");
        fclose(inputFile); 
        return NULL; 
    }

	// define the return type and local variables of cipher
    airlang_intg keyIndex = 0;
    airlang_intg keyLen = (airlang_intg) strlen(key); 
    airlang_intg outputIndex = 0;
    airlang_intg ch;
    
    /*
    * Vigenère Cipher Processing:
    * For each character:
    * 1. Read from file
    * 2. If in visible ASCII range, apply cipher transformation
    * 3. Store in output buffer
    * 4. Ensure we don't exceed buffer size
    */

    while ((ch = fgetc(inputFile)) != EOF && outputIndex < fileSize) {
        if (ch >= ASCII_START && ch <= ASCII_END) {
            int shift = key[keyIndex % keyLen]; 
            if (encode == CYPHER) {
                ch = ((ch - ASCII_START) + (shift - ASCII_START)) % ASCII_RANGE + ASCII_START;
            }
            else if (encode == DECYPHER) {
                ch = ((ch - ASCII_START) - (shift - ASCII_START) + ASCII_RANGE) % ASCII_RANGE + ASCII_START;
            }
            else {
                //Invalid Operation
                errorPrint("Error: Invalid encode parameter %d - Must be 1(CYPHER) OR 0(DECYPHER)\n", encode);
                free(output);     
                fclose(inputFile);
                return NULL;
            }
            keyIndex++;
        }
        output[outputIndex++] = (airlang_char)ch;
    }
    output[outputIndex] = '\0';     // Ensure string is null-terminated
    
    fclose(inputFile);
	return output;
}

// Function to encode (cypher)
void cypher(const airlang_strg inputFileName, const airlang_strg outputFileName, const airlang_strg key) {
    vigenereFile(inputFileName, outputFileName, key, CYPHER);
}

// Function to decode (decypher)
void decypher(const airlang_strg inputFileName, const airlang_strg outputFileName, const airlang_strg key) {
    vigenereFile(inputFileName, outputFileName, key, DECYPHER);
}


/*
***********************************************************
* Function name: getSizeOfFile
* Purpose: Utility to get the size of a file.
* Parameters:
*   - filename: Name of the file (string)
* Returns: Size of the file in bytes (int), or -1 on error.
* 
*   1. Opens the file
*   2. Seeks to the end of file
*   3. Gets current position (which equals file size)
*   4. Closes file
*   5. Returns size
************************************************************
*/

airlang_intg getSizeOfFile(const airlang_strg filename) {
	airlang_intg size = 0;

    //validate the file 
    if (filename == NULL) {
        errorPrint("Error: NULL filename passed to getSizeOfFile");
        return -1;               // file size is always zero or positive.
    }

    //open the file 
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        errorPrint("Error: Cannot open file %s", filename);
        return -1;
    }

    //go (seek) to the EOF
    if (fseek(file, 0, SEEK_END) != 0) {
        errorPrint("Error: fseek failed for file %s", filename);
            fclose(file);
            return -1; 
    }

    //Get current position(size of file)
    size = (airlang_intg)ftell(file);
    if (size < 0) {
        errorPrint("Error: ftell failed for file %s", filename);
        fclose(file);
        return -1;
    }


    //close the file, return the size
    fclose(file);
    return size;
}
