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
# ECHO "[COMPILER SCRIPT .......................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: compilers.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12, A22, A32.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main program of Compilers Project
* Function list: main().
************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

/*
 * ............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: This file must be updated according to each assignment
 * (see the progression: reader > scanner > parser).
 * ............................................................................
 */

/*
***********************************************************
* Function name: main
* Purpose: Main function
* Author: Paulo Sousa
* History/Versions: Ver F22
* Called functions: mainReader(), mainScanner(), mainParser()
* Parameters: Command line arguments - argc, argv
* Return value: Status
* Algorithm: -
*************************************************************
*/
#ifndef BUILD_VM_ONLY


airlang_intg main(int argc, char** argv) {
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // to check memory leaks included in compilers.h
	airlang_intg i;
	printLogo();
	if (DEBUG) {
		for (i = 0; i < argc; ++i)
			printf("argv[%d] = %s\n", i, argv[i]);
	}
	if (argc < 2) {
		printf("%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s", "OPTIONS:\n* [",
			PGM_CDR, "] - Coder\n* [",
			PGM_RDR, "] - Reader\n* [",
			PGM_SCN, "] - Scanner\n* [",
			PGM_PSR, "] - Parser\n* [",
			PGM_WRT, "] - Writer\n* [",
			PGM_GEN, "] - Generator\n* [",
			PGM_VM , "] - VirtualMachine\n");
		return EXIT_FAILURE;
	}
	airlang_char option = argv[1][0];
	switch (option) {
	case PGM_CDR:
		printf("%s%c%s", "\n[Option '", PGM_CDR, "': Starting CODER .....]\n\n");
		main1Coder(argc, argv);
		break;
	case PGM_RDR:
		printf("%s%c%s", "\n[Option '", PGM_RDR, " ': Starting READER....]\n\n");
		main2Reader(argc, argv);
		break;
	case PGM_SCN:
	printf("%s%c%s" , "\n[Option '", PGM_SCN, " ':Starting SCANNER....]\n\n");
		main3Scanner(argc, argv);
	    break;
	case PGM_PSR:
		printf("%s%c%s", "\n[Option '", PGM_PSR, " ':Starting PARSER....]\n\n");
		main4Parser(argc, argv);
		break;
	case PGM_WRT:
		printf("%s%c%s", "\n[Option '", PGM_WRT, " ':Starting WRITER....]\n\n");
		main5Writer(argc, argv);
		break;
	case PGM_GEN:
		printf("%s%c%s", "\n[Option '", PGM_GEN, " ':Starting GENERATOR....]\n\n");
		main6Generator(argc, argv);
		break;
	case PGM_VM:
		printf("%s%c%s", "\n[Option '", PGM_VM, " ':Starting VIRTUAL MACHINE....]\n\n");
		main7Vm(argc, argv);
		break;

	default:
		printf("%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s", "OPTIONS:\n* [",
			PGM_CDR, "] - Coder\n* [",
			PGM_RDR, "] - Reader\n* [",
			PGM_SCN, "] - Scanner\n* [",
			PGM_PSR, "] - Parser\n* [",
			PGM_WRT, "] - Writer\n* [",
			PGM_GEN, "] - Generator\n* [",
			PGM_VM, "] - VirtualMachine\n");
		break;
	}
	return EXIT_SUCCESS;
}


#else
// VM-ONLY MAIN FOR STANDALONE EXECUTABLE
airlang_intg main(int argc, char** argv) {
	printf("%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"\t=---------------------------------------=\n",
		"\t|        AIRLANG VIRTUAL MACHINE       |\n",
		"\t=---------------------------------------=\n",
		"\t    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    \n",
		"\t    @@                             @@    \n",
		"\t    @@            __|__            @@    \n",
		"\t    @@-------@--o--(_)--o--@-------@@    \n",
		"\t    @@                             @@    \n",
		"\t    @@        A I R L A N G        @@    \n",
		"\t    @@                             @@    \n",
		"\t    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    \n",
		"\t                                         \n",
		"\t[VIRTUAL MACHINE RUNTIME ................]\n",
		"\t                                         \n"
	);

	if (argc != 2) {
		printf("Usage: %s <input_file>\n", argv[0]);
		printf("Supported files:\n");
		printf("  - Source files (.txt, etc.): Auto-compiled to bytecode\n");
		printf("  - Bytecode files (.air): Executed directly\n");
		printf("Examples:\n");
		printf("  %s program.txt\n", argv[0]);
		printf("  %s program.air\n", argv[0]);
		return EXIT_FAILURE;
	}

	const char* input_file = argv[1];
	airlang_intg len = strlen(input_file);
	airlang_intg result;

	// Check if it's a bytecode file (.air extension)
	if (len > 4 && strcmp(input_file + len - 4, ".air") == 0) {
		// It's already bytecode - execute directly
		printf("Detected bytecode file: %s\n", input_file);
		printf("Executing directly...\n\n");
		result = runVirtualMachine(input_file);
	}
	else {
		// It's a source file - generate bytecode first
		printf("Detected source file: %s\n", input_file);
		printf("Generating bytecode...\n\n");

		// Calling Step 6 Generator to create bytecode
		char* gen_argv[3];
		gen_argv[0] = argv[0];
		gen_argv[1] = "6";              // passing step6 by default
		gen_argv[2] = (char*)input_file;

		airlang_intg gen_result = main6Generator(3, gen_argv);
		if (gen_result != EXIT_SUCCESS) {
			printf("ERROR: Failed to generate bytecode!\n");
			return EXIT_FAILURE;
		}

		// Create bytecode filename (source + .air)
		char bytecode_file[256];
		strcpy_s(bytecode_file, sizeof(bytecode_file), input_file);
		strcat_s(bytecode_file, sizeof(bytecode_file), ".air");

		printf("Generated bytecode: %s\n", bytecode_file);
		printf("Executing bytecode...\n\n");

		// Execute the generated bytecode
		result = runVirtualMachine(bytecode_file);
	}

	return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
#endif


/*
***********************************************************
* Function name: printLogo
* Purpose: Print Logo
* Author: Tisha Patel
* History/Versions: Ver S25
* Called functions: -
* Parameters: -
* Return value: (Null)
* Algorithm: -
*************************************************************
*/

airlang_void printLogo() {
	printf("%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"\t=---------------------------------------=\n",
		"\t|  COMPILERS - ALGONQUIN COLLEGE (S25)  |\n",
		"\t=---------------------------------------=\n",
		"\t    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    \n",
		"\t    @@                             @@    \n",
		"\t    @@            __|__            @@    \n",
		"\t    @@-------@--o--(_)--o--@-------@@    \n",
		"\t    @@                             @@    \n",
		"\t    @@        A I R L A N G        @@    \n",
		"\t    @@                             @@    \n",
		"\t    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    \n",
		"\t                                         \n",
		"\t[COMPILER SCRIPT .......................]\n",
		"\t                                         \n"
	);
}

/*
************************************************************
* Error printing function with variable number of arguments
* Params: Variable arguments, using formats from C language.
*	- Internal vars use list of arguments and types from stdarg.h
*   - NOTE: The format is using signature from C Language
************************************************************
*/

airlang_void errorPrint(airlang_strg fmt, ...) {
	/* Initialize variable list */
	va_list ap;
	va_start(ap, fmt);

	(airlang_void)vfprintf(stderr, fmt, ap);
	va_end(ap);

	/* Move to new line */
	if (strchr(fmt, '\n') == NULL)
		fprintf(stderr, "\n");
}
