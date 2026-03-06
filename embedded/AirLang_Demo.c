/*
 * airlang_demo.c -- AirLang v3.0-embedded usage examples
 * ======================================================
 * Demonstrates all major embedded runtime features.
 * Compile (no METAR):
 *   gcc -Wall -DAIRLANG_NO_CURL airlang_demo.c AirLang_Runtime.c -lm -o demo
 * Compile (live METAR):
 *   gcc -Wall airlang_demo.c AirLang_Runtime.c -lcurl -lm -o demo
 */

/*
#include "AirLang.h"
#include <stdio.h>

int main(void) {

// normal C code 
printf("Hello from C\n");

// now call AirLang 
AIR_Runtime* rt = AIR_Init();

AIR_RunString(rt,
    "MAIN {\n"
    "  DISPATCH {\n"
    "    PRINT {\"Hello from AirLang\"};\n"
    "  } ENDDISPATCH;\n"
    "} ENDMAIN;\n"
);

// back to normal C 
printf("Back in C\n");

AIR_Free(rt);
return 0;
}*/