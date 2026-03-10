
/*
************************************************************
* Author: Tisha Patel
* AirLang.h
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|           COMPILERS                   |"
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
# ECHO "[Header file ............................]"
# ECHO "                                         "
*/
/*
 * AirLang.h  --  Embedded AirLang Runtime  v3.0-embedded
 * ============================================================
 * Drop this header + AirLang_Runtime.c into any C project.
 * No separate pipeline binary needed.
 *
 * Supports the full AirLang language:
 *   MAIN / BRIEFING / AIRCRAFT / FLIGHT / ROUTE /
 *   LOADSHEET / DISPATCH / WEATHER / WINDANALYSIS /
 *   SAFETYALERT / RECEIVEDDATA / RUNWAYDATA blocks,
 *   IF / ELSE / ENDIF, PRINT {}, AIRPATH, HEADWIND(),
 *   CROSSWIND(), REQUEST METAR FROM, inline METAR: "...",
 *   W&B keywords (PA28 / C172 / B747), arithmetic operators.
 *
 * Quick start:
 *
 *   #include "AirLang.h"
 *
 *   int main(void) {
 *       AIR_Runtime *rt = AIR_Init();
 *
 *       AIR_RunString(rt,
 *           "MAIN {\n"
 *           "  BRIEFING {\n"
 *           "    AIRCRAFT { AircraftType: \"Piper Cherokee PA-28-140\"; }\n"
 *           "    ROUTE {\n"
 *           "      DepartureCoords: 45.3225, -75.6692;\n"
 *           "      ArrivalCoords:   43.6777, -79.6248;\n"
 *           "    }\n"
 *           "  } ENDBRIEFING;\n"
 *           "  DISPATCH {\n"
 *           "    Distance = AIRPATH;\n"
 *           "    PRINT {\"Distance: \" + Distance + \" nm\"};\n"
 *           "  } ENDDISPATCH;\n"
 *           "} ENDMAIN;\n"
 *       );
 *
 *       double d = 0;
 *       AIR_GetNumber(rt, "Distance", &d);
 *       printf("%.2f nm\n", d);   // ~196 nm
 *
 *       AIR_Free(rt);
 *   }
 *
 * Build -- no METAR (MSVC):
 *   cl /W3 /DAIRLANG_NO_CURL your_app.c AirLang_Runtime.c
 *
 * Build -- live METAR (MSVC + vcpkg libcurl):
 *   cl /W3 your_app.c AirLang_Runtime.c ^
 *      /I C:\vcpkg\installed\x64-windows\include ^
 *      /link C:\vcpkg\installed\x64-windows\lib\libcurl.lib
 *
 * Build -- GCC/Linux:
 *   gcc -Wall your_app.c AirLang_Runtime.c -lcurl -lm -o app
 *   gcc -Wall -DAIRLANG_NO_CURL your_app.c AirLang_Runtime.c -lm -o app
 *
 * ============================================================
 * Branch:  v3.0-embedded
 * Author:  Tisha Patel
 * License: MIT
 * ============================================================
 */

#ifndef AIRLANG_H
#define AIRLANG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

    /* version */
#define AIRLANG_VERSION_MAJOR  3
#define AIRLANG_VERSION_MINOR  0
#define AIRLANG_VERSION_PATCH  0
#define AIRLANG_VERSION_STR    "3.0.0-embedded"

/* -- compile-time switches ----------------------------------------- */
/*
 * Define AIRLANG_NO_CURL (compiler flag or before this include)
 * to build without libcurl.  REQUEST METAR FROM will print a
 * warning instead of hitting AviationWeather.gov.
 */

 /* -- tuneable limits ----------------------------------------------- */
#ifndef AIR_MAX_VARS
#  define AIR_MAX_VARS  200   /* max variables per execution context */
#endif
#ifndef AIR_MAX_EXPR
#  define AIR_MAX_EXPR  512   /* max expression / string length      */
#endif

/*
 *  OPAQUE HANDLE
 */

 /**
  * AIR_Runtime
  *   One self-contained AirLang execution context.
  *   Multiple independent instances are safe in one process.
  */
    typedef struct AIR_Runtime AIR_Runtime;

    /*
     *  STATUS CODES
     */

    typedef enum {
        AIR_OK = 0,
        AIR_ERR_ALLOC = -1,   /* malloc / realloc failed      */
        AIR_ERR_FILE = -2,   /* file not found / unreadable  */
        AIR_ERR_SYNTAX = -3,   /* parse / syntax error         */
        AIR_ERR_RUNTIME = -4,   /* execution error              */
        AIR_ERR_NOT_FOUND = -5,   /* variable not found           */
        AIR_ERR_TYPE = -6,   /* wrong variable type          */
        AIR_ERR_NULL = -7    /* NULL argument passed         */
    } AIR_Status;

    /*
     *  VARIABLE TYPES
     *  Mirrors the Variable / VarType in Step5Writer.h
     */

    typedef enum {
        AIR_TYPE_NUMBER = 0,   /* NUMERIC  in Writer  */
        AIR_TYPE_STRING = 1,   /* STRING   in Writer  */
        AIR_TYPE_UNKNOWN = -1
    } AIR_VarType;

    /**
     * AIR_Variable -- read-only snapshot of one AirLang variable.
     * name / string pointers are valid until the next AIR_Run* call
     * or AIR_Free() on the same runtime.
     */
    typedef struct {
        const char* name;
        AIR_VarType  type;
        double       number;   /* valid when type == AIR_TYPE_NUMBER */
        const char* string;   /* valid when type == AIR_TYPE_STRING */
    } AIR_Variable;

    /*
     *  OUTPUT CALLBACK
     */

     /**
      * AIR_OutputCallback
      *   If set, called instead of printf() for every PRINT {} output line.
      *   @param message   null-terminated text (no trailing \n)
      *   @param userdata  value you passed to AIR_SetOutputCallback()
      */
    typedef void (*AIR_OutputCallback)(const char* message, void* userdata);

    /*
     *  AVIATION DATA STRUCTURES
     */

     /**
      * AIR_WindComponents
      *   Returned by AIR_CalcWind().
      *   Mirrors headwind() / crosswind() in Step5Writer.c.
      */
    typedef struct {
        double headwind;   /* positive = headwind, negative = tailwind */
        double crosswind;  /* always >= 0 (absolute value)             */
    } AIR_WindComponents;

    /**
     * AIR_METARResult
     *   Returned by AIR_ParseMETAR() and AIR_FetchMETAR().
     *   Mirrors the fields populated by windInfo(), visibilityInfo(),
     *   temperatureInfo(), altimeterInfo() in Step5Writer.c.
     */
    typedef struct {
        char   station[8];       /* ICAO station identifier              */
        int    wind_dir;         /* degrees true; -1 if variable (VRB)   */
        int    wind_speed_kt;    /* knots                                 */
        int    wind_gust_kt;     /* knots; 0 if no gust reported          */
        double visibility;       /* statute miles (SM) as reported        */
        double temp_c;           /* temperature deg C                     */
        double dewpoint_c;       /* dewpoint deg C                        */
        double altimeter_inhg;   /* altimeter setting inHg                */
        char   raw[512];         /* original METAR string (copy)          */
        int    valid;            /* 1 on successful parse, 0 on error     */
    } AIR_METARResult;

    /*
     *  LIFECYCLE
     */

     /** Allocate and initialise a fresh runtime. Returns NULL on failure. */
    AIR_Runtime* AIR_Init(void);

    /** Destroy the runtime and release all memory. NULL is a safe no-op. */
    void AIR_Free(AIR_Runtime* rt);

    /**
     * Clear variables and execution state, reusing the allocation.
     * Faster than Free + Init; preserves output callback and verbose flag.
     */
    AIR_Status AIR_Reset(AIR_Runtime* rt);

    /*
     *  CONFIGURATION
     */

     /**
      * Redirect PRINT output to your own function.
      * Pass callback = NULL to restore stdout behaviour (default).
      */
    void AIR_SetOutputCallback(AIR_Runtime* rt,
        AIR_OutputCallback callback,
        void* userdata);

    /**
     * verbose = 1  ->  enable diagnostic output:
     *   "AIRCRAFT DATABASE: Loaded ...", "Fetching METAR for ...", etc.
     * Default is 0 (silent).
     */
    void AIR_SetVerbose(AIR_Runtime* rt, int verbose);

    /*
     *  EXECUTION
     */

     /**
      * AIR_RunFile -- load and execute an AirLang source file.
      * Variables accumulate in rt after the call.
      *
      * @return AIR_OK, AIR_ERR_FILE, AIR_ERR_ALLOC, or AIR_ERR_RUNTIME.
      */
    AIR_Status AIR_RunFile(AIR_Runtime* rt, const char* filepath);

    /**
     * AIR_RunString -- execute AirLang source supplied as a C string.
     * Variables accumulate across calls on the same rt.
     * Call AIR_Reset() first to start fresh.
     *
     * @return AIR_OK or AIR_ERR_ALLOC.
     */
    AIR_Status AIR_RunString(AIR_Runtime* rt, const char* source);

    /*
     *  VARIABLE ACCESS  (call after AIR_Run*)
     */

     /**
      * AIR_GetNumber - read a NUMERIC variable.
      *
      * The naming convention after METAR parsing follows Step5Writer:
      *   CYOW_WIND_DIR, CYOW_WIND_SPEED, CYOW_WIND_GUST,
      *   CYOW_TEMP, CYOW_DEWPOINT, CYOW_VISIBILITY, CYOW_ALTIMETER
      *
      * @return AIR_OK, AIR_ERR_NOT_FOUND, or AIR_ERR_TYPE.
      */
    AIR_Status AIR_GetNumber(AIR_Runtime* rt,
        const char* name,
        double* out);

    /**
     * AIR_GetString -- read a STRING variable.
     *   String variables include: aircraft IDs (C-GNBL), flight IDs (AC123),
     *   dates ('2025-08-03'), coordinates ("45.32,-75.66"), METAR strings
     *   (METAR_CYOW), and any quoted string assigned in source.
     *
     * Pointer is valid until the next AIR_Run* call or AIR_Free().
     *
     * @return AIR_OK, AIR_ERR_NOT_FOUND, or AIR_ERR_TYPE.
     */
    AIR_Status AIR_GetString(AIR_Runtime* rt,
        const char* name,
        const char** out);

    /** Read any variable regardless of type. */
    AIR_Status AIR_GetVariable(AIR_Runtime* rt,
        const char* name,
        AIR_Variable* out);

    /**
     * Dump all variables into a caller-supplied array.
     * *count receives the total number of variables (may exceed max).
     */
    AIR_Status AIR_GetAllVariables(AIR_Runtime* rt,
        AIR_Variable* vars,
        int           max,
        int* count);

    /*
     *  DIRECT AVIATION MATH  (no runtime required)
     *  All implementations mirror Step5Writer.c exactly.
     */

     /**
      * AIR_GreatCircleNM
      *   Haversine great-circle distance. Matches calculateAirpath().
      *   Uses the same Taylor-series sin/cos as the original.
      *   @return nautical miles
      */
    double AIR_GreatCircleNM(double lat1_deg, double lon1_deg,
        double lat2_deg, double lon2_deg);

    /**
     * AIR_CalcWind
     *   Headwind and crosswind components. Matches headwind()/crosswind().
     *
     *   @param wind_dir_deg    wind direction true (0-360)
     *   @param wind_speed_kt   wind speed in knots
     *   @param runway_hdg_deg  runway heading true (0-360)
     */
    AIR_WindComponents AIR_CalcWind(double wind_dir_deg,
        double wind_speed_kt,
        double runway_hdg_deg);

    /**
     * AIR_ParseMETAR
     *   Parse a raw METAR string. Pure C, no network.
     *   Mirrors windInfo() + visibilityInfo() + temperatureInfo()
     *   + altimeterInfo() from Step5Writer.c.
     *
     *   Example: AIR_ParseMETAR(
     *     "METAR CYOW 251630Z 27015G25KT 15SM BKN030 08/06 A2995");
     */
    AIR_METARResult AIR_ParseMETAR(const char* metar_string);

    /**
     * AIR_FetchMETAR
     *   Fetch a live METAR from AviationWeather.gov and parse it.
     *   Mirrors fetch_metar_from_api() + fetchMetarData() from MetarFetcher.c.
     *   Requires libcurl; define AIRLANG_NO_CURL to get a stub.
     *
     *   On success the weather variables are also injected into rt
     *   (CYOW_WIND_DIR, CYOW_TEMP, etc.) -- same as REQUEST METAR FROM.
     *
     *   @param rt    runtime handle (may be NULL -- skips variable injection)
     *   @param icao  4-letter ICAO code, e.g. "CYOW"
     *   @return parsed result; result.valid == 0 on failure
     */
    AIR_METARResult AIR_FetchMETAR(AIR_Runtime* rt, const char* icao);

    /*
     *  VALIDATION UTILITIES
     */

     /**
      * AIR_ValidateICAO
      *   Returns 1 if code is exactly 4 alphabetic characters.
      */
    int AIR_ValidateICAO(const char* code);

    /**
     * AIR_IsAircraftIdentifier
     *   Returns 1 for IATA flight numbers (AL123) or ICAO
     *   aircraft registrations (C-GNBL). Mirrors is_aircraft_identifier().
     */
    int AIR_IsAircraftIdentifier(const char* value);

    /**
     * AIR_IsCoordFormat
     *   Returns 1 if value is "lat, lon" decimal format.
     *   Mirrors is_coordinate_format().
     */
    int AIR_IsCoordFormat(const char* value);

    /** Human-readable description of a status code. */
    const char* AIR_StatusString(AIR_Status status);

    /** Returns the library version string: "3.0.0-embedded". */
    const char* AIR_Version(void);

    /*
     *  CONVENIENCE MACROS
     */

     /** Run a source string, ignore the return value. */
#define AIR_EXEC(rt, src)   ((void)AIR_RunString((rt),(src)))

/**
 * Read a number, returning def if the variable is missing or wrong type.
 * Requires GCC/Clang (uses statement-expression extension).
 */
#ifdef __GNUC__
#  define AIR_NUM(rt, name, def) \
     ({ double _v = (def); AIR_GetNumber((rt),(name),&_v); _v; })
#endif

#ifdef __cplusplus
}
#endif

#endif /* AIRLANG_H */