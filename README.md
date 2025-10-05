```
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@                             @@
@@            __|__            @@
@@-------@--o--(_)--o--@-------@@
@@                             @@
@@        A I R L A N G        @@
@@                             @@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
```
Author: Tisha Patel

# AirLang ✈️
*A domain-specific programming language designed for aviation flight planning and dispatch operations*

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://github.com/tishap27/AirLang)
[![Version](https://img.shields.io/badge/version-1.0.0-blue)](./standalone/)

## 🌟 Why AirLang?

AirLang is a specialized domain-specific language engineered for aviation operations and flight planning workflows. Designed with aviation professionals in mind, it features **native aviation syntax** and comprehensive safety validation to streamline flight dispatch and planning processes.
```airlang
BRIEFING {
    AIRCRAFT {
        AircraftID: C-GNBL;
        AircraftType: "Piper Cherokee PA-28-140";
    }
    ROUTE {
        DepartureCoords: 45.3225, -75.6692;  ^^ Ottawa (CYOW)
        ArrivalCoords: 43.6777, -79.6248;    ^^ Toronto (CYYZ)
    }
} ENDBRIEFING;

DISPATCH {
    Distance = AIRPATH;  ^^ Automatic great circle calculation
    PRINT {"Flight distance: " + Distance + " nautical miles"};
} ENDDISPATCH;
```

**Output:**
```
AIRCRAFT DATABASE: Loaded PA-28-140 configuration
Flight distance: 196.10 nautical miles
DISPATCH STATUS: CLEARED FOR DEPARTURE
```

## 🚀 Quick Start

### Download 
1. **Download:** **[⬇️ AirLang ZIP](https://github.com/tishap27/AirLang/archive/refs/heads/main.zip)**
2. **Extract** to any folder (e.g., `C:\AirLang`)
3. **Open Command Prompt** in that folder and run:
```cmd
   airlang install
```
4. **Restart your terminal** and type:
```cmd
    airlang help
```
**Usage**
```cmd
airlang run flight_plan_demo.txt
airlang help
airlang version
```
### Build from Source
```bash
1. Clone: git clone https://github.com/tishap27/AirLang.git
2. Open AirLang.sln in Visual Studio 2022
3. Build → Build Solution (Ctrl+Shift+B)
4. Run: x64\Debug\airlang.exe your_flight_plan.air
```

## 📋 Key Features

### ✈️ Aviation-Native Syntax
- **Aircraft IDs:** `C-GNBL` (automatic ICAO validation)
- **Flight Numbers:** `AL123` (IATA format checking)
- **Coordinates:** `45.3225, -75.6692` (precision validation)
- **Weather:** Native METAR parsing

### 🛡️ Safety-First Design
- **Compile-time validation** of weight limits
- **Automatic checking** of fuel capacity constraints
- **Center of gravity** boundary enforcement
- **Weather minimums** validation

### 🧮 Built-in Aviation Mathematics
- **Great Circle Navigation:** Sub-1% accuracy using Haversine formula
- **Wind Components:** Automatic headwind/crosswind calculations
- **Weight & Balance:** Complete CG analysis with safety margins
- **Performance:** Fuel burn, flight time, ground speed calculations

### 📊 Workflow Integration
```
BRIEFING → WEATHER → LOADSHEET → DISPATCH
```
Mirrors real aviation documentation structure

## 🏗️ Architecture

AirLang features a complete **7-stage compiler pipeline**:

```
Source Code → Lexical Analysis → Parser → Semantic Analysis → Code Generation → Bytecode → Virtual Machine
```

### Technical Highlights
- **237+ VM Instructions** optimized for aviation calculations
- **Finite State Automata** for aviation identifier recognition
- **Recursive Descent Parser** with BNF grammar
- **Stack-based Virtual Machine** with type safety
- **Custom Mathematical Library** for navigation precision

## 📖 Examples

### Flight Planning
```airlang
MAIN {
    BRIEFING {
        AIRCRAFT {
            AircraftID: C-GNBL;
            AircraftType: "Piper Cherokee PA-28-140";
            MaxTakeoffWeight: 2150;
        }
        ROUTE {
            DepartureCoords: 45.3225, -75.6692;
            ArrivalCoords: 43.6777, -79.6248;
        }
    } ENDBRIEFING;
    
    LOADSHEET {
        PassengerCount: 3;
        PassengerWeight = PassengerCount * 170;
        TotalWeight = EmptyWeight + PassengerWeight + FuelWeight;
        
        IF TotalWeight > MaxTakeoffWeight THEN
            PRINT {"WARNING: Exceeds MTOW"};
        ENDIF;
    } ENDLOADSHEET;
    
    DISPATCH {
        Distance = AIRPATH;
        PRINT {"Flight cleared for " + Distance + " nm"};
    } ENDDISPATCH;
} ENDMAIN;
```

### Weather Analysis
```airlang
WEATHER {
    RECEIVEDDATA {
        METAR: "METAR CYOW 251630Z 27015G25KT 5000 -RA BKN008 OVC015 08/06 A2995 RMK";
    } ENDRECEIVEDDATA;
    
    RUNWAYDATA {
        RUNWAYHEADING() WITHCONFIG {
            CYOW_RUNWAY: 070;
        };
    } ENDRUNWAYDATA;
    
    WINDANALYSIS {
        CYOW = HEADWIND();
        CYOW = CROSSWIND();
    } ENDWINDANALYSIS;
} ENDWEATHER;
```

More examples in [`/examples`](./examples/) directory.

## 📚 Documentation

| Resource | Description |
|----------|-------------|
| [**User Guide**](./docs/USER_GUIDE.md) | Complete programming reference |
| [**Language Syntax**](./docs/grammar/syntax_guide.md) | All language constructs and grammar |
| [**Tokens Reference**](./docs/grammar/Token.md) | Built-in aviation functions |
| [**VM Architecture**](./docs/api/vm_instructions.md) | Virtual machine instruction set |
| [**Examples**](./examples/) | Sample programs and tutorials |

## 🎯 Use Cases

- **General Aviation:** Pre-flight planning and weight-balance
- **Aviation Students:** Learning aviation mathematics
- **Researchers:** Modeling aviation workflows and safety constraints
- **Flight Schools:** Teaching flight planning calculations

## 📊 Performance

| Metric | Value |
|--------|-------|
| **Compilation Speed** | 41-164ms for typical programs |
| **Navigation Accuracy** | ±0.1 nautical miles |
| **Memory Usage** | <50MB for complex flight plans |
| **Supported Aircraft** | 15+ built-in configurations |

## 🔧 Development

### Project Structure
```
/
├── src code           # Compiler source code
├── examples/          # Sample AirLang programs
├── docs/              # Complete documentation
├── tests/             # Test suite
└── standalone/        # Compiled executables
```

### Building
```bash
# Debug build
make debug

# Release build
make release

# Run tests
make test
```

## 🤝 Contributing

AirLang is designed as an educational and research project. Contributions welcome:

1. **Fork** the repository
2. **Create** a feature branch
3. **Add tests** for new functionality
4. **Submit** a pull request

See [CONTRIBUTING.md](./CONTRIBUTING.md) for detailed guidelines.

## 🎓 Academic Context

This project was developed as part of Computer Engineering Technology program at Algonquin College, demonstrating:

- **Compiler Design Principles** applied to domain-specific languages
- **Aviation Domain Expertise** informing language design
- **Safety-Critical Software** development practices
- **Complete Software Engineering** lifecycle

**Academic Research Paper:** [AirLang: A Domain-Specific Language for Aviation Flight Planning and Operations](./docs/paper/)

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.


## 📞 Contact

**Tisha Patel**
- **GitHub:** [@tishap27](https://github.com/tishap27)
- **Email:** tishaapatel08@gmail.com
- **Project Website:** [AirLang Official Site](https://tishap27.github.io/AirLang/)

---

**⭐ Star this repository if AirLang helped you understand domain-specific languages or aviation programming!**

*Built by a pilot who codes*