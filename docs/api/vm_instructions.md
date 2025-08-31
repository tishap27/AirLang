# AirLang Virtual Machine Instructions Reference

## Overview

The AirLang Virtual Machine (VM) is a stack-based virtual machine designed specifically for aviation domain operations. It executes bytecode generated from AirLang source code and provides specialized instructions for flight planning, navigation calculations, and aviation data processing.

## VM Architecture

- **Stack-based architecture** - Operations work with a runtime stack
- **Bytecode execution** - Executes compiled .air bytecode files
- **Type safety** - Runtime type checking for aviation data
- **Magic number validation** - Ensures bytecode integrity
- **237+ instruction types** - Comprehensive instruction set

## Instruction Categories

### 1. Stack Operations

| Instruction | Opcode | Description | Stack Effect |
|-------------|--------|-------------|--------------|
| `PUSH_INT` | 0x01 | Push integer value onto stack | → value |
| `PUSH_FLOAT` | 0x02 | Push floating-point value onto stack | → value |
| `PUSH_STRING` | 0x03 | Push string value onto stack | → string |
| `PUSH_BOOL` | 0x04 | Push boolean value onto stack | → bool |
| `POP` | 0x05 | Remove top value from stack | value → |
| `DUP` | 0x06 | Duplicate top stack value | value → value, value |
| `SWAP` | 0x07 | Swap top two stack values | val1, val2 → val2, val1 |

### 2. Arithmetic Operations

| Instruction | Opcode | Description | Stack Effect |
|-------------|--------|-------------|--------------|
| `ADD` | 0x10 | Addition | val1, val2 → result |
| `SUB` | 0x11 | Subtraction | val1, val2 → result |
| `MUL` | 0x12 | Multiplication | val1, val2 → result |
| `DIV` | 0x13 | Division | val1, val2 → result |
| `MOD` | 0x14 | Modulo operation | val1, val2 → result |
| `NEG` | 0x15 | Negation | value → -value |
| `ABS` | 0x16 | Absolute value | value → |value| |

### 3. Comparison Operations

| Instruction | Opcode | Description | Stack Effect |
|-------------|--------|-------------|--------------|
| `EQ` | 0x20 | Equal comparison | val1, val2 → bool |
| `NE` | 0x21 | Not equal comparison | val1, val2 → bool |
| `LT` | 0x22 | Less than | val1, val2 → bool |
| `LE` | 0x23 | Less than or equal | val1, val2 → bool |
| `GT` | 0x24 | Greater than | val1, val2 → bool |
| `GE` | 0x25 | Greater than or equal | val1, val2 → bool |

### 4. Logical Operations

| Instruction | Opcode | Description | Stack Effect |
|-------------|--------|-------------|--------------|
| `AND` | 0x30 | Logical AND | bool1, bool2 → bool |
| `OR` | 0x31 | Logical OR | bool1, bool2 → bool |
| `NOT` | 0x32 | Logical NOT | bool → !bool |

### 5. Control Flow

| Instruction | Opcode | Description | Stack Effect |
|-------------|--------|-------------|--------------|
| `JMP` | 0x40 | Unconditional jump | |
| `JMP_IF_FALSE` | 0x41 | Jump if top of stack is false | bool → |
| `JMP_IF_TRUE` | 0x42 | Jump if top of stack is true | bool → |
| `CALL` | 0x43 | Function call | |
| `RET` | 0x44 | Return from function | |
| `HALT` | 0x45 | Stop VM execution | |

### 6. Variable Operations

| Instruction | Opcode | Description | Stack Effect |
|-------------|--------|-------------|--------------|
| `LOAD_VAR` | 0x50 | Load variable value | → value |
| `STORE_VAR` | 0x51 | Store value in variable | value → |
| `LOAD_GLOBAL` | 0x52 | Load global variable | → value |
| `STORE_GLOBAL` | 0x53 | Store global variable | value → |

### 7. Aviation-Specific Instructions

#### Navigation & Distance

| Instruction | Opcode | Description | Stack Effect |
|-------------|--------|-------------|--------------|
| `AIRPATH` | 0x60 | Calculate great circle distance | lat1, lon1, lat2, lon2 → distance |
| `COORD_PARSE` | 0x61 | Parse coordinate string | coord_string → lat, lon |
| `COORD_VALIDATE` | 0x62 | Validate coordinate format | lat, lon → bool |

#### Wind Calculations

| Instruction | Opcode | Description | Stack Effect |
|-------------|--------|-------------|--------------|
| `HEADWIND` | 0x63 | Calculate headwind component | wind_dir, wind_speed, track → headwind |
| `CROSSWIND` | 0x64 | Calculate crosswind component | wind_dir, wind_speed, runway_hdg → crosswind |
| `WIND_TRIANGLE` | 0x65 | Solve wind triangle | tas, wind_dir, wind_speed, track → gs |

#### Aircraft & Flight Data

| Instruction | Opcode | Description | Stack Effect |
|-------------|--------|-------------|--------------|
| `VALIDATE_ACFT_ID` | 0x70 | Validate aircraft registration | acft_id → bool |
| `VALIDATE_FLIGHT_NUM` | 0x71 | Validate IATA flight number | flight_num → bool |
| `VALIDATE_DATE` | 0x72 | Validate ISO date format | date_string → bool |
| `LOAD_ACFT_DATA` | 0x73 | Load aircraft specifications | acft_type → mtow, fuel_cap, etc. |

#### Weight & Balance

| Instruction | Opcode | Description | Stack Effect |
|-------------|--------|-------------|--------------|
| `CALC_PAYLOAD` | 0x80 | Calculate payload weight | pax_count, cargo_weight → payload |
| `CHECK_MTOW` | 0x81 | Check maximum takeoff weight | total_weight, mtow → bool |
| `CALC_CG` | 0x82 | Calculate center of gravity | weights, arms → cg_position |
| `CHECK_CG_LIMITS` | 0x83 | Check CG within limits | cg_pos, fwd_limit, aft_limit → bool |

#### Weather Operations

| Instruction | Opcode | Description | Stack Effect |
|-------------|--------|-------------|--------------|
| `PARSE_METAR` | 0x90 | Parse METAR weather string | metar_string → weather_data |
| `EXTRACT_WIND` | 0x91 | Extract wind from METAR | weather_data → wind_dir, wind_speed |
| `EXTRACT_VISIBILITY` | 0x92 | Extract visibility from METAR | weather_data → visibility |
| `CHECK_WEATHER_MINS` | 0x93 | Check weather minimums | conditions, minimums → bool |

### 8. I/O Operations

| Instruction | Opcode | Description | Stack Effect |
|-------------|--------|-------------|--------------|
| `PRINT` | 0xA0 | Print value to output | value → |
| `PRINT_LN` | 0xA1 | Print value with newline | value → |
| `READ_INPUT` | 0xA2 | Read input from user | → input_string |
| `FORMAT_OUTPUT` | 0xA3 | Format string with values | format, ...values → formatted_string |

### 9. Type Operations

| Instruction | Opcode | Description | Stack Effect |
|-------------|--------|-------------|--------------|
| `TYPE_CHECK` | 0xB0 | Check value type | value, expected_type → bool |
| `TYPE_CONVERT` | 0xB1 | Convert value type | value, target_type → converted_value |
| `IS_NUMERIC` | 0xB2 | Check if value is numeric | value → bool |
| `IS_STRING` | 0xB3 | Check if value is string | value → bool |

### 10. Error Handling

| Instruction | Opcode | Description | Stack Effect |
|-------------|--------|-------------|--------------|
| `THROW_ERROR` | 0xC0 | Throw runtime error | error_message → |
| `TRY_BEGIN` | 0xC1 | Begin try block | |
| `TRY_END` | 0xC2 | End try block | |
| `CATCH` | 0xC3 | Catch exception | |

## Bytecode File Format

```
Magic Number: 0xAIRL (0x41494C52)
Version: 1 byte
Instruction Count: 4 bytes (big-endian)
String Table Size: 4 bytes (big-endian)
String Table: variable length
Instructions: variable length
```

## Example Bytecode Sequence

```assembly
; Simple AirLang program: PRINT{"Hello AirLang"};
PUSH_STRING "Hello AirLang"    ; 0x03 [string_index]
PRINT_LN                       ; 0xA1
HALT                           ; 0x45
```

## Aviation Constants

The VM includes built-in constants for aviation calculations:

| Constant | Value | Description |
|----------|-------|-------------|
| `EARTH_RADIUS_KM` | 6371.0 | Earth radius in kilometers |
| `EARTH_RADIUS_NM` | 3440.065 | Earth radius in nautical miles |
| `NM_TO_KM` | 1.852 | Nautical miles to kilometers |
| `KM_TO_NM` | 0.539957 | Kilometers to nautical miles |
| `STANDARD_PRESSURE` | 29.92 | Standard atmospheric pressure (inHg) |
| `STANDARD_TEMP` | 15.0 | Standard temperature (°C) |

## Error Codes

| Code | Name | Description |
|------|------|-------------|
| 0x00 | SUCCESS | Operation completed successfully |
| 0x01 | STACK_UNDERFLOW | Attempted to pop from empty stack |
| 0x02 | STACK_OVERFLOW | Stack exceeded maximum size |
| 0x03 | INVALID_INSTRUCTION | Unknown instruction opcode |
| 0x04 | TYPE_ERROR | Type mismatch in operation |
| 0x05 | DIVISION_BY_ZERO | Division by zero attempted |
| 0x06 | INVALID_AIRCRAFT_ID | Aircraft registration format invalid |
| 0x07 | INVALID_FLIGHT_NUM | Flight number format invalid |
| 0x08 | WEIGHT_EXCEEDS_MTOW | Weight exceeds maximum takeoff weight |
| 0x09 | CG_OUT_OF_LIMITS | Center of gravity outside limits |
| 0x0A | INVALID_COORDINATES | Coordinate values out of range |
| 0x0B | WEATHER_PARSE_ERROR | Failed to parse METAR string |

## Usage Examples

### Basic Stack Operations
```assembly
PUSH_INT 100        ; Stack: [100]
PUSH_INT 50         ; Stack: [100, 50]
ADD                 ; Stack: [150]
PRINT_LN           ; Output: 150
```

### Aviation Calculation
```assembly
PUSH_FLOAT 45.3225  ; Departure latitude
PUSH_FLOAT -75.6692 ; Departure longitude
PUSH_FLOAT 40.6413  ; Arrival latitude
PUSH_FLOAT -73.7781 ; Arrival longitude
AIRPATH            ; Calculate great circle distance
PRINT_LN           ; Output: distance in nautical miles
```

### Conditional Logic
```assembly
LOAD_VAR total_weight
LOAD_VAR mtow
GT                 ; Check if weight > MTOW
JMP_IF_TRUE warning_label
PUSH_STRING "Weight OK"
JMP end_label
warning_label:
PUSH_STRING "WARNING: Exceeds MTOW"
end_label:
PRINT_LN
```

This instruction set provides comprehensive support for both general programming constructs and specialized aviation operations, making the AirLang VM suitable for flight planning and dispatch applications.