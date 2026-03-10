
 /*
 ************************************************************
 * Author: Tisha Patel
 * AirLang_Runtime.c 
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
 # ECHO "[Runtime SCRIPT .........................]"
 # ECHO "                                         "
 */


#define _CRT_SECURE_NO_WARNINGS
#include "AirLang.h"
#include <stdlib.h>
#include <string.h>

struct AIR_Runtime {
    AIR_OutputCallback cb;
    void* cb_data;
    int                verbose;
};


AIR_Runtime* AIR_Init(void) {
    return (AIR_Runtime*)calloc(1, sizeof(AIR_Runtime));
}

void AIR_Free(AIR_Runtime* rt) { if (rt) free(rt); }

AIR_Status AIR_Reset(AIR_Runtime* rt) {
    if (!rt) return AIR_ERR_NULL;
    AIR_OutputCallback cb = rt->cb; void* d = rt->cb_data; int v = rt->verbose;
    memset(rt, 0, sizeof(*rt));
    rt->cb = cb; rt->cb_data = d; rt->verbose = v;
    return AIR_OK;
}

void AIR_SetOutputCallback(AIR_Runtime* rt, AIR_OutputCallback cb, void* data) {
    if (rt) { rt->cb = cb; rt->cb_data = data; }
}

void AIR_SetVerbose(AIR_Runtime* rt, int v) { if (rt) rt->verbose = v; }

const char* AIR_Version(void) { return AIRLANG_VERSION_STR; }

//not all are implemented yet 

AIR_Status      AIR_RunString(AIR_Runtime* rt, const char* s) { (void)rt;(void)s; return AIR_OK; }
AIR_Status      AIR_RunFile(AIR_Runtime* rt, const char* f) { (void)rt;(void)f; return AIR_OK; }
AIR_Status      AIR_GetNumber(AIR_Runtime* rt, const char* n, double* o) { (void)rt;(void)n; if (o)*o = 0; return AIR_ERR_NOT_FOUND; }
AIR_Status      AIR_GetString(AIR_Runtime* rt, const char* n, const char** o) { (void)rt;(void)n;(void)o; return AIR_ERR_NOT_FOUND; }
AIR_Status      AIR_GetVariable(AIR_Runtime* rt, const char* n, AIR_Variable* o) { (void)rt;(void)n;(void)o; return AIR_ERR_NOT_FOUND; }
AIR_Status      AIR_GetAllVariables(AIR_Runtime* rt, AIR_Variable* v, int m, int* c) { (void)rt;(void)v;(void)m; if (c)*c = 0; return AIR_OK; }
double          AIR_GreatCircleNM(double a, double b, double c, double d) { (void)a;(void)b;(void)c;(void)d; return 0.0; }
AIR_WindComponents AIR_CalcWind(double a, double b, double c) { AIR_WindComponents r = { 0 };(void)a;(void)b;(void)c; return r; }
AIR_METARResult AIR_ParseMETAR(const char* s) { AIR_METARResult r;memset(&r, 0, sizeof(r));r.wind_dir = -1;(void)s; return r; }
AIR_METARResult AIR_FetchMETAR(AIR_Runtime* rt, const char* s) { AIR_METARResult r;memset(&r, 0, sizeof(r));r.wind_dir = -1;(void)rt;(void)s; return r; }
int             AIR_ValidateICAO(const char* c) { (void)c; return 0; }
int             AIR_IsAircraftIdentifier(const char* v) { (void)v; return 0; }
int             AIR_IsCoordFormat(const char* v) { (void)v; return 0; }

const char* AIR_StatusString(AIR_Status s) {
    switch (s) {
    case AIR_OK:            return "OK";
    case AIR_ERR_ALLOC:     return "Memory allocation failure";
    case AIR_ERR_FILE:      return "File not found or unreadable";
    case AIR_ERR_SYNTAX:    return "Syntax error";
    case AIR_ERR_RUNTIME:   return "Runtime error";
    case AIR_ERR_NOT_FOUND: return "Variable not found";
    case AIR_ERR_TYPE:      return "Variable type mismatch";
    case AIR_ERR_NULL:      return "NULL argument";
    default:                return "Unknown error";
    }
}