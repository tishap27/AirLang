# AirLang Syntax Guide

## Overview
AirLang is a domain-specific language for aviation professionals, featuring structured blocks for flight planning, weight calculations, and dispatch operations.

## Program Structure

### Basic Template
```airlang
MAIN {
    BRIEFING {
        AIRCRAFT { /* aircraft details */ }
        FLIGHT { /* flight information */ }
        ROUTE { /* route planning */ }
    } ENDBRIEFING;
    
    LOADSHEET {
        /* weight and balance calculations */
    } ENDLOADSHEET;
    
    DISPATCH {
        /* final dispatch operations */
    } ENDDISPATCH;
} ENDMAIN;
```

## Block Types

### 1. BRIEFING Block
Contains flight planning information:
```airlang
BRIEFING {
    AIRCRAFT { 
        AircraftID: C-GNBL; 
        AircraftType: "Boeing 747-400";
        MaxTakeoffWeight: 833000;
    }
    FLIGHT { 
        FlightNumber: AL123; 
        FlightDate: '2025-05-15';
        DepartureTime: "14:30";
    }
    ROUTE { 
        DepartureCoords: 45.3225, -75.6692;  ^^ Ottawa
        ArrivalCoords: 40.6413, -73.7781;    ^^ New York
        CruiseAltitude: 37000;
    }
} ENDBRIEFING;
```

### 2. LOADSHEET Block
Handles weight and balance calculations:
```airlang
LOADSHEET {
    PassengerCount = 350;
    CargoWeight = 15000;
    PayloadWeight = PassengerCount * 200 + CargoWeight;
    FuelWeight = 180000;
    TotalWeight = PayloadWeight + FuelWeight + 412000;
    
    IF TotalWeight > MaxTakeoffWeight THEN
        PRINT {"WARNING: Exceeds MTOW"};
    ELSE
        PRINT {"Weight Check: WITHIN LIMITS"};
    ENDIF;
} ENDLOADSHEET;
```

### 3. DISPATCH Block
Final dispatch operations and reports:
```airlang
DISPATCH {
    REPORT {
        Distance = AIRPATH;
        PRINT {"Flight Distance: " + Distance + " nautical miles"};
        PRINT {"Flight " + FlightNumber + " cleared for dispatch"};
    } ENDREPORT;
} ENDDISPATCH;
```

## Data Types

### 1. Identifiers
- Must start with uppercase letter (A-Z)
- Can contain letters, numbers, underscores
- Examples: `Rain`, `Aircraft1`, `Flight_123`

### 2. String Literals
- Enclosed in double quotes
- Examples: `"Boeing 747-400"`, `"Clear skies"`

### 3. Numeric Literals
- **Integers:** `123`, `-77`, `0`
- **Floats:** `1.5`, `-3.14159`, `123.456`

### 4. Aviation-Specific Types

#### Aircraft Registration
- Format: `[Letter]-[4 Letters]`
- Examples: `C-GNBL`, `N747BA`, `G-ABCD`

#### Flight Numbers
- Format: `[2 Letters][3 Digits]`
- Examples: `AL123`, `WS456`, `AC001`

#### Dates
- Format: `'YYYY-MM-DD'` (single quotes)
- Examples: `'2025-06-24'`, `'2024-12-31'`

#### Coordinates
- Decimal degree pairs: `latitude, longitude`
- Examples: `45.3225, -75.6692`, `40.6413, -73.7781`

## Operators

### Arithmetic
```airlang
PayloadWeight = PassengerCount * 200 + CargoWeight;
GroundSpeed = CruiseSpeed + (-Headwind);
FlightTime = Distance / GroundSpeed;
FuelBurn = FlightTime * FuelFlow;
```

### Comparison
```airlang
IF TotalWeight > MaxTakeoffWeight THEN
IF Visibility < 1000 THEN
IF FuelRemaining >= MinimumFuel THEN
```

### String Concatenation
```airlang
PRINT {"Flight: " + FlightNumber + " ready for departure"};
PRINT {"Weight: " + TotalWeight + " lbs"};
```

## Control Structures

### Conditional Statements
```airlang
IF condition THEN
    /* statements */
ELSE
    /* alternative statements */
ENDIF;
```

### Examples
```airlang
IF WeatherCondition == "VFR" THEN
    PRINT {"Visual flight rules apply"};
ELSE
    PRINT {"Instrument flight rules required"};
ENDIF;

IF CrosswindComponent > 15 THEN
    PRINT {"CAUTION: High crosswind conditions"};
ENDIF;
```

## Aviation Functions

### Navigation Functions
```airlang
Distance = AIRPATH;  // Great circle distance between last two coordinates
```

### Wind Calculations
```airlang
HeadwindComponent = HEADWIND(WindDirection, WindSpeed, TrackAngle);
CrosswindComponent = CROSSWIND(WindDirection, WindSpeed, RunwayHeading);
```

### Weather Functions
```airlang
WeatherData = METAR("METAR CYOW 151800Z 27015KT 10SM FEW250 M02/M12 A3012");
Visibility = EXTRACT_VISIBILITY(WeatherData);
WindInfo = EXTRACT_WIND(WeatherData);
```

## Built-in Constants

### Earth Navigation
```airlang
EARTH_RADIUS_KM    // 6371.0 km
EARTH_RADIUS_NM    // 3440.065 nautical miles
NM_TO_KM           // 1.852
KM_TO_NM           // 0.539957
```

### Standard Atmosphere
```airlang
STANDARD_PRESSURE  // 29.92 inHg
STANDARD_TEMP      // 15.0 °C
```

## Comments

### Single Line Comments
```airlang
^^ This is a single line comment
PassengerCount = 350;  ^^ Maximum passenger capacity
```

### Multi-line Comments
```airlang
% 
This is a multi-line comment
Used for detailed explanations
of complex flight calculations
%
```

## Output Statements

### Basic Output
```airlang
PRINT {"Simple message"};
PRINT {"Value: " + VariableName};
```

### Formatted Output
```airlang
PRINT {"Flight: " + FlightNumber + " on " + FlightDate};
PRINT {"Distance: " + Distance + " nautical miles"};
PRINT {"Fuel Required: " + FuelRequired + " gallons"};
```

## Error Handling

### Common Syntax Errors
```airlang
// ❌ Wrong - lowercase identifier
flight_number = AL123;

// ✅ Correct - uppercase identifier
FlightNumber = AL123;

// ❌ Wrong - missing quotes
AircraftType = Boeing 747;

// ✅ Correct - proper string
AircraftType = "Boeing 747-400";

// ❌ Wrong - invalid aircraft ID format
AircraftID = C123;

// ✅ Correct - proper format
AircraftID: C-GNBL;
```

### Aviation Validation Errors
```airlang
// ❌ Invalid coordinate range
DepartureCoords: 91.0, 181.0;  // Latitude > 90, Longitude > 180

// ✅ Valid coordinates
DepartureCoords: 45.3225, -75.6692;

// ❌ Invalid date format
FlightDate: 2025-05-15;  // Missing quotes

// ✅ Correct date format
FlightDate: '2025-05-15';
```

## Safety Checks and Warnings

AirLang includes built-in safety validations:

### Weight Limits
```airlang
IF TotalWeight > MaxTakeoffWeight THEN
    PRINT {"*** WARNING: Exceeds MTOW ***"};
    PRINT {"Current: " + TotalWeight + " lbs"};
    PRINT {"Maximum: " + MaxTakeoffWeight + " lbs"};
ENDIF;
```

### Weather Minimums
```airlang
IF Visibility < WeatherMinimums THEN
    PRINT {"*** WEATHER BELOW MINIMUMS ***"};
ENDIF;
```

### Fuel Requirements
```airlang
IF TotalFuel < (FlightFuel + ReserveFuel) THEN
    PRINT {"*** INSUFFICIENT FUEL ***"};
ENDIF;
```

## Complete Example Program

```airlang
MAIN {
    BRIEFING {
        AIRCRAFT { 
            AircraftID: C-GNBL; 
            AircraftType: "Boeing 747-400";
            MaxTakeoffWeight: 833000;
            EmptyWeight: 412000;
        }
        FLIGHT { 
            FlightNumber: AL123; 
            FlightDate: '2025-05-15';
            DepartureTime: "14:30";
        }
        ROUTE { 
            DepartureCoords: 45.3225, -75.6692;  ^^ CYOW Ottawa
            ArrivalCoords: 40.6413, -73.7781;    ^^ KJFK New York
            CruiseAltitude: 37000;
        }
    } ENDBRIEFING;

    LOADSHEET {
        ^^ Passenger and cargo calculations
        PassengerCount = 350;
        BaggageWeight = PassengerCount * 50;
        CargoWeight = 15000;
        PayloadWeight = PassengerCount * 200 + BaggageWeight + CargoWeight;
        
        ^^ Fuel calculations
        FuelWeight = 180000;
        TotalWeight = PayloadWeight + FuelWeight + EmptyWeight;
        
        ^^ Safety checks
        PRINT {"=== WEIGHT AND BALANCE ==="};
        PRINT {"Passengers: " + PassengerCount};
        PRINT {"Payload: " + PayloadWeight + " lbs"};
        PRINT {"Total Weight: " + TotalWeight + " lbs"};
        
        IF TotalWeight > MaxTakeoffWeight THEN
            PRINT {"*** WARNING: Exceeds MTOW ***"};
            PRINT {"Overweight by: " + (TotalWeight - MaxTakeoffWeight) + " lbs"};
        ELSE
            PRINT {"Weight Check: WITHIN LIMITS"};
            PRINT {"Margin: " + (MaxTakeoffWeight - TotalWeight) + " lbs"};
        ENDIF;
    } ENDLOADSHEET;

    DISPATCH {
        REPORT {
            Distance = AIRPATH;
            EstimatedFlightTime = Distance / 483;  ^^ Cruise speed
            
            PRINT {""};
            PRINT {"=== FLIGHT DISPATCH REPORT ==="};
            PRINT {"Aircraft: " + AircraftType + " (" + AircraftID + ")"};
            PRINT {"Flight: " + FlightNumber + " on " + FlightDate};
            PRINT {"Route Distance: " + Distance + " nautical miles"};
            PRINT {"Estimated Flight Time: " + EstimatedFlightTime + " hours"};
            
            IF TotalWeight <= MaxTakeoffWeight THEN
                PRINT {"*** FLIGHT CLEARED FOR DISPATCH ***"};
            ELSE
                PRINT {"*** FLIGHT NOT CLEARED - WEIGHT ISSUE ***"};
            ENDIF;
            
            PRINT {"=== END OF REPORT ==="};
        } ENDREPORT;
    } ENDDISPATCH;
} ENDMAIN;
```

## Best Practices

1. **Always validate aircraft registrations** using proper ICAO format
2. **Include safety checks** for weight, fuel, and weather
3. **Use meaningful variable names** that reflect aviation terminology
4. **Comment complex calculations** for clarity
5. **Structure programs** using the standard BRIEFING → LOADSHEET → DISPATCH flow
6. **Validate coordinates** are within realistic ranges
7. **Include error handling** for edge cases

## File Extensions
- **Source files:** `.txt` (AirLang source code)
- **Compiled bytecode:** `.air` (compiled and ready for execution)
- **Encrypted intermediate:** `.enc` (internal compiler use)