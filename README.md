# Mini Compiler Front-End

A console-based compiler front-end written in C that demonstrates core concepts from **Automata Theory**.

## Concepts Demonstrated

| Phase | Automata Concept | What It Does |
|---|---|---|
| **Lexical Analysis** | DFA (Deterministic Finite Automaton) | Scans input character-by-character and classifies lexemes into tokens |
| **Syntax Analysis** | CFG (Context-Free Grammar) | Validates the token stream against grammar productions |
| **Symbol Table** | — | Stores variable names, types, and values |

## Supported Grammar

```
Declaration  →  KEYWORD  IDENTIFIER  '='  VALUE  ';'
Assignment   →  IDENTIFIER  '='  EXPRESSION  ';'
Expression   →  VALUE  { OPERATOR  VALUE }
VALUE        →  NUMBER | IDENTIFIER
KEYWORD      →  'int' | 'float'
OPERATOR     →  '+' | '-' | '*' | '/'
```

## Token Types

- `KEYWORD` — `int`, `float`
- `IDENTIFIER` — variable names
- `NUMBER` — integer and floating-point literals
- `OPERATOR` — `=`, `+`, `-`, `*`, `/`
- `SEMICOLON` — `;`

## Build & Run

```bash
gcc -Wall -Wextra -o mini_compiler mini_compiler.c
./mini_compiler
```

Enter code line by line; press **Enter on a blank line** to finish input.

## Example Session

**Input:**
```
int a = 10;
float b = 2.5;
a = a + 5;
```

**Output:**
```
====== TOKEN LIST ======
  KEYWORD      : int
  IDENTIFIER   : a
  OPERATOR     : =
  NUMBER       : 10
  SEMICOLON    : ;
  KEYWORD      : float
  IDENTIFIER   : b
  OPERATOR     : =
  NUMBER       : 2.5
  SEMICOLON    : ;
  IDENTIFIER   : a
  OPERATOR     : =
  IDENTIFIER   : a
  OPERATOR     : +
  NUMBER       : 5
  SEMICOLON    : ;

====== SYNTAX ANALYSIS ======
  Line 1: Syntax Correct  (Declaration)
  Line 2: Syntax Correct  (Declaration)
  Line 3: Syntax Correct  (Assignment)

====== SYMBOL TABLE ======
  Name           Type           Value
  ----------------------------------------
  a              int            a + 5
  b              float          2.5
```

## Error Detection Examples

| Input | Error |
|---|---|
| `int x = 10` | Missing semicolon |
| `float = 3.14;` | Incomplete declaration |
| `a = b` | Incomplete assignment (missing `;`) |
| `42 bad;` | Unrecognised statement |

## Project Structure

```
mini-compiler/
├── mini_compiler.c   # All source code (lexer, parser, symbol table)
└── README.md
```

## Architecture

```
Source Code → [Lexer/DFA] → Token Stream → [Parser/CFG] → Syntax Result
                                     ↓
                              [Symbol Table Builder]
                                     ↓
                              Symbol Table Output
```