/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2025
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (S25)  |"
# ECHO "=---------------------------------------="
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@           %&@@@@@@@@@@@     @@    ”
# ECHO "    @@       @%% (@@@@@@@@@  @     @@    ”
# ECHO "    @@      @& @   @ @       @     @@    ”
# ECHO "    @@     @ @ %  / /   @@@@@@     @@    ”
# ECHO "    @@      & @ @  @@              @@    ”
# ECHO "    @@       @/ @*@ @ @   @        @@    ”
# ECHO "    @@           @@@@  @@ @ @      @@    ”
# ECHO "    @@            /@@    @@@ @     @@    ”
# ECHO "    @@     @      / /     @@ @     @@    ”
# ECHO "    @@     @ @@   /@/   @@@ @      @@    ”
# ECHO "    @@     @@@@@@@@@@@@@@@         @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@       A I R L A N G         @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
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

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
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

// Function to perform the Vigenère cipher (encoding or decoding)
void vigenereFile(const airlang_strg inputFileName, const airlang_strg outputFileName, const airlang_strg key, airlang_intg encode) {
	// TO_DO: Define the input and output files (ex: FILE* inputFile, FILE* outputFile
    FILE* inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        fprintf(stderr, "Error: Cannot open input file %s\n", inputFileName);
        return;
    }
    FILE* outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        fprintf(stderr, "Error: Cannot open output file %s\n", outputFileName);
        fclose(inputFile);
        return;
    }
	// TO_DO: Use defensive programming (checking files)
	// TO_DO: Define local variables
    airlang_intg keyLen = strlen(key);
    airlang_intg keyIndex = 0;
    airlang_intg ch;
	// TO_DO: Logic: check if it is encode / decode to change the char (using Vigenere algorithm) - next function
	
    while ((ch = fgetc(inputFile)) != EOF) {
        if (ch >= ASCII_START && ch <= ASCII_END) {
            int shift = key[keyIndex % keyLen]; 
            if (encode == CYPHER) {
                ch = ((ch - ASCII_START) + (shift - ASCII_START)) % ASCII_RANGE + ASCII_START;
            }
            else
                ch = ((ch - ASCII_START) - (shift - ASCII_START) + ASCII_RANGE) % ASCII_RANGE + ASCII_START;
            keyIndex++;
        }
        fputc(ch, outputFile);
    }

    // TO_DO: Close the files
    fclose(inputFile);
    fclose(outputFile);
}

// Function to perform the Vigenère cipher (encoding or decoding)
airlang_strg vigenereMem(const airlang_strg inputFileName, const airlang_strg key, airlang_intg encode) {
    if (inputFileName == NULL || key == NULL) {
        errorPrint("Error: Invalid input parameters");
        return NULL;
    }

    FILE* inputFile = fopen(inputFileName, "r");
    if (inputFile ==  NULL) {
        errorPrint("Error: Cannot open input file %s", inputFileName);
        return NULL;
    }

    fseek(inputFile, 0, SEEK_END);
    airlang_intg fileSize = ftell(inputFile);
    rewind(inputFile);

    airlang_strg output = (airlang_strg)malloc(fileSize + 1);
    if (output == NULL) {
        errorPrint("Error: memory allocation failed");
        fclose(inputFile); 
        return NULL; 
    }

    //if key is empty 


	// TO_DO define the return type and local variables
    airlang_intg keyIndex = 0;
    airlang_intg keyLen = strlen(key); 
    airlang_intg outputIndex = 0;
    //airlang_strg output = NULL;
    airlang_intg ch;
     

    //process chars
    while ((ch = fgetc(inputFile)) != EOF) {
        if (ch >= ASCII_START && ch <= ASCII_END) {
            int shift = key[keyIndex % keyLen]; 
            if (encode == CYPHER) {
                ch = ((ch - ASCII_START) + (shift - ASCII_START)) % ASCII_RANGE + ASCII_START;
            }
            else if (encode == DECYPHER) {
                ch = ((ch - ASCII_START) - (shift - ASCII_START) + ASCII_RANGE) % ASCII_RANGE + ASCII_START;
            }
            keyIndex++;
        }
        output[outputIndex++] = (airlang_char)ch;
    }

	// + airlang_strg output = NULL;
	// TO_DO: Check defensive programming
	// TO_DO: Use the logic to code/decode - consider the logic about visible chars only

    output[outputIndex] = '\0';
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

// TO_DO: Get file size (util method)
airlang_intg getSizeOfFile(const airlang_strg filename) {
	airlang_intg size = 0;

    if (filename == NULL) {
        errorPrint("Error: NULL filename passed to getSizeOfFile");
        return -1;               // file size is always zero or positive.
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        errorPrint("Error: Cannot open file %s", filename);
        return -1;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        errorPrint("Error: fseek failed for file %s", filename);
            fclose(file);
            return -1; 
    }
    size = (airlang_intg)ftell(file);
    if (size < 0) {
        errorPrint("Error: ftell failed for file %s", filename);
        fclose(file);
        return -1;
    }


    // TO_DO: Use the logic to get the size of the file
    fclose(file);
    return size;
}
