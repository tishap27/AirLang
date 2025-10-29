# Changelog

All notable changes to AirLang will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [2.0] - 2025-10-28

### 🚀 Added
- **Real-time METAR fetching** - Automatic weather data from AviationWeather.gov API
- **ICAO code detection** - Smart parsing of 4-letter airport codes
- **Comprehensive error handling** - User-friendly validation messages with examples
- **Network error recovery** - Graceful handling of API failures with helpful feedback
- **Multiple airport support** - Fetch weather for unlimited airports in single script
- **libcurl integration** - HTTP client library for API communication
- New module: `metarFetcher.c` / `metarFetcher.h` for API operations
- Automatic wind gust detection and parsing

### 🔧 Changed
- Enhanced `handleRequestStatement()` to detect ICAO codes vs. full METAR strings
- Improved METAR parsing with robust error handling
- Updated error messages to include specific format requirements

### ✅ Backward Compatibility
- All v1.0 code runs unchanged
- Manual METAR strings still fully supported (must start with "METAR")
- No parser or BNF grammar modifications required
- Zero breaking changes to existing syntax

### 📊 Technical Details
- Uses AviationWeather.gov public API (no API key required)
- JSON response parsing for METAR data extraction
- Automatic creation of weather variables: `ICAO_WIND_DIR`, `ICAO_TEMP`, etc.
- Thread-safe curl operations

### 🐛 Fixed
- Improved whitespace handling in REQUEST statements
- Better validation of empty input strings

---

## [1.0] - 2025-10-15

### 🎉 Initial Release

#### Core Language Features
- Complete compiler pipeline: Lexer → Parser → Semantic Analyzer → Code Generator → VM
- Aviation-native syntax for flight planning
- BNF grammar with recursive descent parser
- Stack-based virtual machine with 237+ instructions

#### Aviation Calculations
- **Great Circle Navigation** - Haversine formula implementation (sub-1% accuracy)
- **Weight & Balance** - Complete CG analysis with safety validation
  - Empty weight and moment calculations
  - Multi-station loading (pilot, passenger, fuel, baggage)
  - MTOW compliance checking
  - CG envelope validation
- **Wind Components** - Headwind and crosswind calculations
  - Trigonometric wind resolution
  - Runway heading integration
- **Flight Performance** - Fuel burn, ground speed, flight time

#### Data Types & Validation
- **Aircraft IDs** - ICAO format validation (e.g., C-GNBL)
- **Flight Numbers** - IATA format checking (e.g., AL123)
- **Coordinates** - Precision decimal degree format (lat, lon)
- **Dates** - Aviation date format ('YYYY-MM-DD')
- **METAR** - Manual weather string input

#### Block Structures
- `BRIEFING` - Aircraft and route configuration
- `WEATHER` - METAR parsing and wind analysis
  - `RECEIVEDDATA` - Manual METAR entry
  - `RUNWAYDATA` - Runway heading configuration
  - `WINDANALYSIS` - Headwind/crosswind calculations
  - `SAFETYALERT` - Weather minimums checking
- `LOADSHEET` - Weight & balance calculations
- `DISPATCH` - Final authorization and reporting

#### Built-in Functions
- `AIRPATH` - Great circle distance calculation
- `HEADWIND()` - Headwind component from wind data
- `CROSSWIND()` - Crosswind component from wind data
- `TOTALWEIGHT` - Sum of all loading stations
- `WEIGHTBAL` - Total moment calculation
- `VALIDATEWB` - Weight & balance envelope check

#### Aircraft Database
- Pre-configured aircraft specifications:
  - Piper Cherokee PA-28-140
  - Cessna 172 Skyhawk
  - Boeing 747-400
  - Piper Seneca PA-44
- Certified weight, CG, and fuel capacity data

#### Safety Features
- Compile-time weight limit validation
- Automatic CG envelope boundary checking
- Fuel capacity overflow detection
- Weather minimums enforcement
- Aviation-specific error messages

#### Documentation
- Complete language reference
- User guide with examples
- Token and keyword reference
- VM architecture documentation
- Sample flight planning programs

### 🏗️ Architecture
- 7-stage compilation pipeline
- Finite State Automata for token recognition
- Custom mathematical library (no external math.h dependency)
- Type-safe variable system
- Memory-safe string operations

### 📝 File Support
- `.air` file extension for AirLang programs
- Multi-line comment support (`^^ comment`)
- Block comment support (`% comment %`)

---

## Version Numbering

AirLang follows **Semantic Versioning**:
- **MAJOR** (2.x.x) - Breaking changes to language syntax
- **MINOR** (x.2.x) - New features, backward compatible
- **PATCH** (x.x.2) - Bug fixes, no new features

---

## Links

- [GitHub Repository](https://github.com/tishap27/AirLang)
- [Documentation](https://tishap27.github.io/AirLang/)
- [Report Issues](https://github.com/tishap27/AirLang/issues)
- [MIT License](LICENSE)

---

**Built by a pilot who codes** ✈️💻