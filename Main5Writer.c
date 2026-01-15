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
# ECHO "[WRITER SCRIPT .........................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: MainReader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12, A22, A32.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
* Function list: (...).
*************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

#ifndef WRITER_H_
#include "Step5Writer.h"
#endif


/*Function declaration*/
void displayWriterContent(const char* content);

/*
************************************************************
* Main function from Buffer
* Parameters:
*   argc / argv = Parameters from command prompt
* Return value:
*	Success operation.
************************************************************
*/


airlang_intg main5Writer(airlang_intg argc, airlang_strg* argv) {
	if (argc >= 2) {
		airlang_strg source = argv[2];
		/* load source file into input buffer  */
		printf("Reading file %s ....Please wait\n", source);
		airlang_intg size = getSizeOfFile(source);
		airlang_strg content = vigenereMem(source, STR_LANGNAME, DECYPHER);
		/* find the size of the file  */
		if (size <= 0) {
			printf("The input file %s %s\n", source, "is not completely loaded.");
		}
		else {
			displayWriterContent(content);
			process_content(content);
			
		}
	}
	return EXIT_SUCCESS;
}

void displayWriterContent(const char* content) {
	printf("\nPrinting input buffer parameters:\n\n");
	printf("%s\n", content);
	printf("\n");
	printf("Running the source file...\n");
	printf("\n");
}
