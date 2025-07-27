@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@                             @@
@@            __|__            @@
@@-------@--o--(_)--o--@-------@@
@@                             @@
@@        A I R L A N G        @@
@@                             @@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

Overview 
AirLang is a Domain Specific Language designed for flight dispatchers and aviation professionals. It provides native support for flight planning calculations, weight and balance computation, and route planning with aviation- specific datatypes. 

Key Features

- Aviation-Focused Syntax - Natural language constructs for flight operations
- Built-in Navigation - Coordinate parsing and great circle distance calculations
- Weight & Balance - Automatic load calculations and safety checks
- Weather Integration - Weather condition handling and dispatch decisions
- Aircraft Data - Support for IATA/ICAO identifiers and specifications
- Flight Mathematics - Fuel burn, flight time, and performance calculations


Compiler Architecture 

Source Code → Step1 → Step2 → Step3 → Step4 → Step5 → Output
              Coder   Reader  Scanner  Parser  Writer


Phase Breakdown 

1. Coder  : File preprocessing and cypher/decypher.
2. Reader : Managing buffer and character stream, validating input.
3. Scanner: Tokenizing source code via FSM and lexical analysis.
4. Parser : Applying BNF grammar rules to build syntax tree.
5. Writer : Generating dispatch reports and final output.


Language Syntax

Program Structure 

	MAIN {
    	    BRIEFING {
              AIRCRAFT { AircraftID: C-GNBL; AircraftType: "Boeing 747-400"; }
              FLIGHT { FlightNumber: AL123; FlightDate: '2025-05-15'; }
              ROUTE { DepartureCoords: 45.3225, -75.6692; }
            } ENDBRIEFING;
    
            LOADSHEET {
                ^^ Load calculations optional
                PayloadWeight = PassengerCount * 200 + CargoWeight;
                IF TotalWeight > MaxTakeoffWeight THEN
                   PRINT {"WARNING: Exceeds MTOW"};
                ENDIF;
    	    } ENDLOADSHEET;
	    DISPATCH {
		PRINT {"Flight log accepted"};
	    }ENDDISPATCH; 
        } ENDMAIN;


Variable Declaration

	AircraftID: C-GNBL;                    // Aircraft registration
	FlightNumber: AL123;                   // IATA flight code  
	CruiseSpeed: 493;                      // Numeric value
	AircraftType: "Boeing 747-400";        // String value
	DepartureCoords: 45.3225, -75.6692;   // Coordinate pair
	FlightDate: '2025-05-15';              // Date format


Arithmetic Operations

	PayloadWeight = PassengerCount * 200 + CargoWeight;
	GroundSpeed = CruiseSpeed + (-Headwind);
	FlightTime = Distance / GroundSpeed;
	TotalFuel = FuelBurn + ReserveFuel;

Conditional Logic & Safety Checks

	IF TotalWeight > MaxTakeoffWeight THEN
   	 PRINT{"WARNING: Exceeds MTOW"};
	 PRINT {"Current Weight: " + TotalWeight + " lbs"};
	ELSE
   	 PRINT{"Weight Check: WITHIN LIMITS"};
	ENDIF;

Aviation-Specific Functions

	Distance = AIRPATH;  // Calculates great circle distance between last two coordinates
	CrossWind(270, 15, 70);      // calculates crosswind , wind_dir, speed, runway_heading
	HeadWind(...); 		     

Weather Integration

	Handles METAR strings, wind/visibility calculations, trigger warnings

Output Statements
	
	PRINT{"Flight: " + FlightNumber + " ready for departure"};
	PRINT{"Distance: " + Distance + " nautical miles"};

Comments

	^^ This is a single-line comment
	% This is a multi-line comment %


Supported Data Types:

- Numeric: 450, 123.5

- String: "Boeing 747-400"

- Coordinate: 45.32, -75.67

- Aircraft ID: C-GNBL		//ICAO aircraft registration

- Flight Code: AL123		//IATA airline and flight number

- Date: '2025-05-15'

- Boolean: true, false

- Character: 'A'


Language Features

Aviation Data Validation

Aircraft Registration: Validates ICAO format (e.g., C-GNBL)
Flight Numbers: Validates IATA format (e.g., AL123)
Coordinates: Validates decimal degree format
Dates: Validates ISO date format

Built-in Constants & Conversions

Earth radius for navigation calculations (6371 km)
Nautical mile conversions (1 nm = 1.852 km)
Standard atmospheric conditions
Aviation unit conversions


Error Handling & Safety Warnings 

AirLang provides clear error messages for common issues:

Syntax Errors

Clear error messages with context
Line-by-line validation
Missing bracket/brace detection

Aviation Validation Errors

Invalid aircraft registration formats
Out-of-range coordinates
Invalid flight number patterns
Malformed date/time formats

Flight Safety Checks

Weight and balance limit violations
Fuel capacity exceedances
Weather condition warnings
Performance limit notifications






