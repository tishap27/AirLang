# AirLang DSL - BNF Grammar


### Core Program Structure
```bnf
<program> ::= <comment>* <request_statement>* <comment>* <main_block>

<main_block> ::= "MAIN" "{" <briefing_block> <weather_block>? <loadsheet_block>? <dispatch_block> "}" "ENDMAIN" ";"
```

### Comments and Preprocessing
```bnf
<comment> ::= COMMENT_LITERAL

<request_statement> ::= "REQUEST" <request_list> "FROM" STRING_LITERAL ";"

<request_list> ::= "METAR" | "NOTAM"
```

### Briefing Block
```bnf
<briefing_block> ::= "BRIEFING" "{" <briefing_content> "}" "ENDBRIEFING" ";"

<briefing_content> ::= <aircraft_record>? <flight_record>? <route_record>? <output_statement>*

<aircraft_record> ::= "AIRCRAFT" "{" <aircraft_structure>* "}"

<aircraft_structure> ::= <field_identifier> ":" <aircraft_value> ";"

<aircraft_value> ::= AIRCRAFT_ID | ID | STRING_LITERAL | INTEGER | FLOAT | BOOLEAN

<flight_record> ::= "FLIGHT" "{" <flight_data> "}"

<flight_data> ::= <flight_structure>* <output_statement>* <comment>*

<flight_structure> ::= ID ":" <flight_value> ";"

<flight_value> ::= FLIGHT_ID | DATE_LITERAL | STRING_LITERAL | INTEGER | FLOAT | BOOLEAN

<route_record> ::= "ROUTE" "{" <route_data> "}"

<route_data> ::= <route_structure>* <output_statement>* <comment>*

<route_structure> ::= ID ":" <route_value> ";"

<route_value> ::= STRING_LITERAL | INTEGER | FLOAT | BOOLEAN | <coordinate_pair>

<coordinate_pair> ::= FLOAT "," FLOAT | INTEGER "," INTEGER | FLOAT "," INTEGER | INTEGER "," FLOAT
```

### Weather Block
```bnf
<weather_block> ::= "WEATHER" "{" <weather_content> "}" "ENDWEATHER" ";"

<weather_content> ::= <received_data_block>? <runway_data_block>? <wind_analysis_block>? <safety_alert_block>?

<received_data_block> ::= "RECEIVEDDATA" "{" <weather_data_assignments> "}" "ENDRECEIVEDDATA" ";"

<weather_data_assignments> ::= <received_data_assignment>* <output_statement>* <comment>*

<received_data_assignment> ::= "METAR" ":" STRING_LITERAL ";"

<runway_data_block> ::= "RUNWAYDATA" "{" <runway_content> "}" "ENDRUNWAYDATA" ";"

<runway_content> ::= <method_call>* <comment>*

<wind_analysis_block> ::= "WINDANALYSIS" "{" <wind_content> "}" "ENDWINDANALYSIS" ";"

<wind_content> ::= <method_call>* <comment>*

<safety_alert_block> ::= "SAFETYALERT" "{" <safety_content> "}" "ENDSAFETYALERT" ";"

<safety_content> ::= <if_statement>* <output_statement>* <comment>*
```

### Load Sheet Block
```bnf
<loadsheet_block> ::= "LOADSHEET" "{" <performance_content> "}" "ENDLOADSHEET" ";"

<performance_content> ::= <performance_assignment>* <if_statement>* <output_statement>* <comment>*

<performance_assignment> ::= ID <assignment_operator> <performance_expression> ";"

<assignment_operator> ::= "=" | ":"

<performance_expression> ::= <expression> | "AIRPATH"

<expression> ::= <term> ("+" <term>)*

<term> ::= <factor> ("*" <factor> | "/" <factor>)*

<factor> ::= ID | INTEGER | FLOAT | "(" INTEGER ")" | "(" FLOAT ")" | "AIRPATH"
```

### Dispatch Block
```bnf
<dispatch_block> ::= "DISPATCH" "{" <dispatch_content> "}" "ENDDISPATCH" ";"

<dispatch_content> ::= <if_statement>* <report_statement>* <output_statement>* <comment>*

<if_statement> ::= "IF" ID <comparison_operator> <comparison_value> "THEN" <print_statements> <else_clause>? "ENDIF" ";"

<comparison_operator> ::= "!=" | "==" | ">"

<comparison_value> ::= ID | STRING_LITERAL | BOOLEAN | INTEGER | FLOAT

<print_statements> ::= <output_statement>*

<else_clause> ::= "ELSE" <output_statement>*

<report_statement> ::= "REPORT" "{" <report_content> "}" "ENDREPORT" ";"

<report_content> ::= <method_call>* <output_statement>* <comment>*
```

### Method Calls and Configuration
```bnf
<method_call> ::= METHOD_IDENTIFIER <with_config_block>? ";"

<with_config_block> ::= "WITHCONFIG" "{" <config_assignments> "}"

<config_assignments> ::= <config_assignment>*

<config_assignment> ::= ID <assignment_operator> <config_value> ";"

<config_value> ::= ID ("." ID)? | METHOD_IDENTIFIER "(" ")" | INTEGER | FLOAT | STRING_LITERAL | DECIMAL
```

### Output Statements
```bnf
<output_statement> ::= "PRINT" "{" <output_list> "}" ";"

<output_list> ::= <print_element> ("+" <print_element>)*

<print_element> ::= STRING_LITERAL | ID | INTEGER | FLOAT | AIRCRAFT_ID | FLIGHT_ID
```

### Field Identifiers and Values
```bnf
<field_identifier> ::= ID | AIRCRAFT_ID
```

### Terminal Symbols 
```
ID               : Identifier tokens
METHOD_IDENTIFIER: Method names (like HEADWIND, CROSSWIND, RUNWAYHEADING, WIND)
AIRCRAFT_ID      : Aircraft registration (like C-GNBL)
FLIGHT_ID        : Flight numbers (like AL123)
INTEGER          : Integer literals
FLOAT            : Float literals  
DECIMAL          : Decimal point token
STRING_LITERAL   : String literals in quotes
DATE_LITERAL     : Date format '2025-05-15'
BOOLEAN          : TRUE/FALSE values
COMMENT_LITERAL  : Comments starting with ^^

Operators:
EQL_T           : =
COLON_T         : :
NOT_EQ_T        : !=
GT_T            : >
PLUS_T          : +
MULTI_T         : *
DIV_T           : /
COMMA_T         : ,
DEC_T           : .

Delimiters:
LBR_T           : {
RBR_T           : }
LPR_T           : (
RPR_T           : )
EOS_T           : ; (end of statement)

Keywords (from your KW_ tokens):
MAIN, ENDMAIN, BRIEFING, ENDBRIEFING, AIRCRAFT, FLIGHT, ROUTE
WEATHER, ENDWEATHER, RECEIVEDDATA, ENDRECEIVEDDATA, RUNWAYDATA, ENDRUNWAYDATA
WINDANALYSIS, ENDWINDANALYSIS, SAFETYALERT, ENDSAFETYALERT
LOADSHEET, ENDLOADSHEET, DISPATCH, ENDDISPATCH
REPORT, ENDREPORT, WITHCONFIG
IF, THEN, ELSE, ENDIF, PRINT
REQUEST, FROM, METAR, NOTAM, AIRPATH
```

## Key Features Actually Implemented:

### 1. **Flight Dispatch Structure**
- MAIN block containing briefing, weather, loadsheet, and dispatch
- Proper block termination with END keywords

### 2. **Aircraft Configuration**
- Aircraft records with fields like AircraftID, AircraftType, etc.
- Type checking for aircraft values
- Safety warnings for minimum requirements

### 3. **Flight Planning**
- Flight records with flight numbers, dates, times
- Route records with coordinates and airport codes
- Weather data integration (METAR only)

### 4. **Performance Calculations**
- Load sheet with weight and fuel calculations
- Mathematical expressions with +, -, *, /
- Special AIRPATH keyword for distance calculations

### 5. **Safety Logic**
- IF-THEN-ELSE conditional statements
- Safety alerts with threshold checking
- Go/No-go decision making

### 6. **Professional Output**
- Structured PRINT statements with concatenation
- Report generation blocks
- Method calls with configuration blocks

### 7. **Data Sources**
- REQUEST statements for METAR and NOTAM data
- External URL integration

