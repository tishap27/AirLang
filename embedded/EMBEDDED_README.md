# AirLang v3.0-embedded

Embed the full AirLang aviation DSL into any C project with **two files**.

```
AirLang.h            ← single include
AirLang_Runtime.c    ← implementation (add to your build)
```

No separate compiler binary. No installation. Works on Windows (MSVC),
Linux, and macOS.

---

## Quick Start

```c
#include "AirLang.h"

int main(void) {
    AIR_Runtime *rt = AIR_Init();

    AIR_RunString(rt,
        "MAIN {\n"
        "  BRIEFING {\n"
        "    ROUTE {\n"
        "      DepartureCoords: 45.3225, -75.6692;\n"   /* CYOW */
        "      ArrivalCoords:   43.6777, -79.6248;\n"   /* CYYZ */
        "    }\n"
        "  } ENDBRIEFING;\n"
        "  DISPATCH {\n"
        "    Distance = AIRPATH;\n"
        "    PRINT {\"Distance: \" + Distance + \" nm\"};\n"
        "  } ENDDISPATCH;\n"
        "} ENDMAIN;\n"
    );

    double d = 0;
    AIR_GetNumber(rt, "Distance", &d);   /* 196.04 nm */
    AIR_Free(rt);
}
```

---

## Build

### Windows — MSVC (no network)
```bat
cl /W3 /DAIRLANG_NO_CURL your_app.c AirLang_Runtime.c
```

### Windows — MSVC + live METAR (vcpkg libcurl)
```bat
cl /W3 your_app.c AirLang_Runtime.c ^
   /I C:\vcpkg\installed\x64-windows\include ^
   /link C:\vcpkg\installed\x64-windows\lib\libcurl.lib
```

### Linux / macOS — GCC or Clang
```bash
# with live METAR
gcc -Wall your_app.c AirLang_Runtime.c -lcurl -lm -o your_app

# without network
gcc -Wall -DAIRLANG_NO_CURL your_app.c AirLang_Runtime.c -lm -o your_app
```

### Demo binary
```bash
gcc -Wall -DAIRLANG_NO_CURL airlang_demo.c AirLang_Runtime.c -lm -o demo
./demo
```

---

## API Reference

### Lifecycle
| Function | Description |
|---|---|
| `AIR_Init()` | Allocate and return a new runtime context |
| `AIR_Free(rt)` | Release all memory; NULL is safe |
| `AIR_Reset(rt)` | Clear variables and state, reuse allocation |

### Configuration
| Function | Description |
|---|---|
| `AIR_SetOutputCallback(rt, cb, data)` | Redirect `PRINT` output away from stdout |
| `AIR_SetVerbose(rt, 1)` | Enable diagnostic messages (METAR fetch, aircraft DB load) |

### Execution
| Function | Description |
|---|---|
| `AIR_RunString(rt, source)` | Execute AirLang source from a C string |
| `AIR_RunFile(rt, path)` | Load and execute a `.air` file |

### Variable Access
| Function | Description |
|---|---|
| `AIR_GetNumber(rt, name, &out)` | Read a numeric variable |
| `AIR_GetString(rt, name, &out)` | Read a string variable |
| `AIR_GetVariable(rt, name, &out)` | Read any variable |
| `AIR_GetAllVariables(rt, vars, max, &count)` | Dump all variables |

### Direct Aviation Math
| Function | Description |
|---|---|
| `AIR_GreatCircleNM(lat1,lon1,lat2,lon2)` | Haversine distance in nautical miles |
| `AIR_CalcWind(wind_dir, wind_spd, rwy_hdg)` | Headwind + crosswind components |
| `AIR_ParseMETAR(raw_string)` | Parse a METAR string (pure C, no network) |
| `AIR_FetchMETAR(rt, "CYOW")` | Live METAR from AviationWeather.gov (needs libcurl) |

### Validation
| Function | Description |
|---|---|
| `AIR_ValidateICAO(code)` | Returns 1 for 4-letter ICAO identifiers |
| `AIR_IsAircraftIdentifier(value)` | Returns 1 for C-GNBL or AC123 style |
| `AIR_IsCoordFormat(value)` | Returns 1 for "lat, lon" decimal pairs |
| `AIR_StatusString(status)` | Human-readable status code |
| `AIR_Version()` | Returns `"3.0.0-embedded"` |

---

## AirLang Language Features Supported

| Feature | Example |
|---|---|
| Block structure | `MAIN { BRIEFING { … } ENDBRIEFING; } ENDMAIN;` |
| All block types | AIRCRAFT, FLIGHT, ROUTE, LOADSHEET, DISPATCH, WEATHER, WINDANALYSIS, SAFETYALERT, RECEIVEDDATA, RUNWAYDATA |
| Colon assignment | `AircraftType: "Piper Cherokee PA-28-140";` |
| Equals assignment | `Distance = AIRPATH;` |
| Arithmetic | `FuelBurn = FlightTime * 8;` |
| Great-circle keyword | `Distance = AIRPATH;` |
| Wind components | `HeadwindComp = HEADWIND(); CrosswindComp = CROSSWIND();` |
| PRINT | `PRINT {"Distance: " + Distance + " nm"};` |
| IF / ELSE / ENDIF | `IF HeadwindComp > 10 THEN … ELSE … ENDIF` |
| Inline METAR | `REQUEST METAR FROM "METAR CYOW 251630Z 27015G25KT …";` |
| Live METAR | `REQUEST METAR FROM "CYOW";` |
| METAR variable injection | `CYOW_WIND_DIR`, `CYOW_WIND_SPEED`, `CYOW_TEMP`, `CYOW_ALTIMETER`, … |
| Aircraft W&B DB | PA28, C172, B747 — auto-loaded from `AircraftType:` |
| String variables | Aircraft IDs (`C-GNBL`), flight IDs (`AC882`), dates (`'2025-08-03'`) |

---

## METAR Variable Naming (Step5Writer convention)

After `REQUEST METAR FROM "ICAO"` the runtime injects:

```
ICAO_WIND_DIR     numeric  degrees true (-1 if VRB)
ICAO_WIND_SPEED   numeric  knots
ICAO_WIND_GUST    numeric  knots (0 if none)
ICAO_TEMP         numeric  °C
ICAO_DEWPOINT     numeric  °C
ICAO_VISIBILITY   numeric  SM
ICAO_ALTIMETER    numeric  inHg
METAR_ICAO        string   raw METAR text
```

---

## Git Workflow

```bash
git checkout -b v3.0-embedded
git add AirLang.h AirLang_Runtime.c airlang_demo.c EMBEDDED_README.md
git commit -m "feat: v3.0 embedded runtime — drop-in C library"
git tag v3.0-embedded
git push origin v3.0-embedded --tags
```

---

## Version History

| Version | Description |
|---|---|
| v1.0 | CLI pipeline — Scanner → Parser → Writer (METAR) |
| v2.0 | Full compiler — Generator + VM bytecode execution |
| v3.0-embedded | Drop-in C library (`AirLang.h` + `AirLang_Runtime.c`) |

---

## Limits & Notes

- Up to 200 variables per runtime context (override `AIR_MAX_VARS`)
- Expression buffer: 512 characters (override `AIR_MAX_EXPR`)
- Variables accumulate across `AIR_RunString()` calls on the same `rt`;
  call `AIR_Reset()` to start fresh
- `AIR_FetchMETAR()` requires libcurl; define `AIRLANG_NO_CURL` to omit
- Thread safety: use one `AIR_Runtime*` per thread

---

*AirLang v3.0-embedded — Tisha Patel*