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
		printf("  - Source files (.txt): Auto-encrypted and compiled to bytecode\n");
		printf("  - Encrypted files (.enc): Auto-compiled to bytecode\n");
		printf("  - Bytecode files (.air): Executed directly\n");
		printf("Examples:\n");
		printf("  %s program.txt\n", argv[0]);
		printf("  %s program.enc\n", argv[0]);
		printf("  %s program.air\n", argv[0]);
		return EXIT_FAILURE;
	}

	airlang_char* input_file = argv[1];
	airlang_intg len = (airlang_intg)strlen(input_file);
	airlang_intg result;
	airlang_char encrypted_file[256];
	airlang_char bytecode_file[256];

	// Check if it's a bytecode file (.air extension)
	if (len > 4 && strcmp(input_file + len - 4, ".air") == 0) {
		// It's already bytecode - execute directly
		printf("Detected bytecode file: %s\n", input_file);
		printf("Executing directly...\n\n");
		result = runVirtualMachine(input_file);
	}
	else if (len > 4 && strcmp(input_file + len - 4, ".enc") == 0) {

		// It's an encrypted file - generate bytecode first
		printf("Detected encrypted file: %s\n", input_file);
		printf("Generating bytecode...\n\n");

		// Calling Step 6 Generator to create bytecode
		airlang_char* gen_argv[3];
		gen_argv[0] = argv[0];
		gen_argv[1] = "6";              // passing step6 by default
		gen_argv[2] = (char*)input_file;

		airlang_intg gen_result = main6Generator(3, gen_argv);
		if (gen_result != EXIT_SUCCESS) {
			printf("ERROR: Failed to generate bytecode!\n");
			return EXIT_FAILURE;
		}

		// Create bytecode filename ( replacing .enc with .air)
		const airlang_char* last_dot = strrchr(input_file, '.');
		if (last_dot) {
			size_t base_len = last_dot - input_file;
			strncpy_s(bytecode_file, sizeof(bytecode_file), input_file, base_len);
			bytecode_file[base_len] = '\0';
			strcat_s(bytecode_file, sizeof(bytecode_file), ".air");

			printf("Generated bytecode: %s\n", bytecode_file);
			printf("Executing bytecode...\n\n");

			// Execute the generated bytecode
			result = runVirtualMachine(bytecode_file);
		}
	}
		else {
			//plane .txt file --so will need to encrypt first than bytecode than vm 
			printf("Detected source file: %s\n", input_file);
			printf("Starting encryption process...\n\n");

			// Step 1: Encrypt the source file
		// Create encrypted filename (replace extension with .enc)
			const airlang_char* last_dot = strrchr(input_file, '.');
			const airlang_char* last_slash = strrchr(input_file, '/');
			const airlang_char* last_backslash = strrchr(input_file, '\\');

			// Find filename start (handle paths)
			const airlang_char* filename_start = input_file;
			if (last_slash && last_slash > filename_start) filename_start = last_slash + 1;
			if (last_backslash && last_backslash > filename_start) filename_start = last_backslash + 1;

			if (last_dot && last_dot > filename_start) {
				// Replace extension: "program.txt" -> "program.enc"
				size_t base_len = last_dot - input_file;
				strncpy_s(encrypted_file, sizeof(encrypted_file), input_file, base_len);
				encrypted_file[base_len] = '\0';
				strcat_s(encrypted_file, sizeof(encrypted_file), ".enc");
			}
			else {
				// No extension: "program" -> "program.enc"
				strcpy_s(encrypted_file, sizeof(encrypted_file), input_file);
				strcat_s(encrypted_file, sizeof(encrypted_file), ".enc");
			}

			printf("--- Step 1: ENCRYPTION ---\n");
			printf("Encrypting: %s -> %s\n", input_file, encrypted_file);

			// Call Step 1 Coder (encryption)
			airlang_char* coder_argv[5];
			coder_argv[0] = argv[0];
			coder_argv[1] = "1";
			coder_argv[2] = "1";  // Encryption mode (1 = encrypt, 0 = decrypt)
			coder_argv[3] = (char*)input_file;
			coder_argv[4] = encrypted_file;

			airlang_intg coder_result = main1Coder(5, coder_argv);
			if (coder_result != EXIT_SUCCESS) {
				printf("ERROR: Failed to encrypt source file!\n");
				return EXIT_FAILURE;
			}

			printf("Encryption completed successfully!\n\n");

			// Step 2: Generate bytecode from encrypted file
			printf("--- BYTECODE GENERATION ---\n");
			printf("Generating bytecode from encrypted file...\n");

			char* gen_argv[3];
			gen_argv[0] = argv[0];
			gen_argv[1] = "6";
			gen_argv[2] = encrypted_file;

			airlang_intg gen_result = main6Generator(3, gen_argv);
			if (gen_result != EXIT_SUCCESS) {
				printf("ERROR: Failed to generate bytecode!\n");
				return EXIT_FAILURE;
			}

			// Create bytecode filename from encrypted file (replace .enc with .air)
			last_dot = strrchr(encrypted_file, '.');
			if (last_dot) {
				size_t base_len = last_dot - encrypted_file;
				strncpy_s(bytecode_file, sizeof(bytecode_file), encrypted_file, base_len);
				bytecode_file[base_len] = '\0';
				strcat_s(bytecode_file, sizeof(bytecode_file), ".air");
			}

			printf("Generated bytecode: %s\n", bytecode_file);
			printf("Executing bytecode...\n\n");

			// Step 3: Execute the generated bytecode
			result = runVirtualMachine(bytecode_file);
		}

		if (result) {
			printf("\n=== EXECUTION COMPLETED SUCCESSFULLY ===\n");
			return EXIT_SUCCESS;
		}
		else {
			printf("\n=== EXECUTION FAILED ===\n");
			return EXIT_FAILURE;
		}
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
