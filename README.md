<div align="center">

# TIniFile

### A lightweight, dependency-free INI configuration reader and writer for C++

[![Language: C++](https://img.shields.io/badge/language-C%2B%2B-00599C?logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![Standard: C++11](https://img.shields.io/badge/standard-C%2B%2B11-00599C)](https://en.cppreference.com/w/cpp/11)
[![Build: Make](https://img.shields.io/badge/build-Make-427819?logo=gnu&logoColor=white)](Makefile)
[![Dependencies: none](https://img.shields.io/badge/dependencies-none-2ea44f)](#design-goals)
[![Repository](https://img.shields.io/badge/repository-GitHub-181717?logo=github)](https://github.com/paulocfmarques-collab/inifiles)

Read, query, update, and persist classic `section/key=value` configuration files with a small single-header implementation.

</div>

---

## Contents

- [Why TIniFile](#why-tinifile)
- [Capabilities](#capabilities)
- [Architecture](#architecture)
- [Data flow](#data-flow)
- [Quick start](#quick-start)
- [Usage](#usage)
- [API reference](#api-reference)
- [INI format](#ini-format)
- [Build targets](#build-targets)
- [Behavior and limitations](#behavior-and-limitations)
- [Project layout](#project-layout)
- [Contributing](#contributing)

## Why TIniFile

`TIniFile` is designed for small C++ programs that need human-readable configuration without pulling in a framework or external parser. The implementation lives in [`inifiles.h`](inifiles.h), so integrating it into an existing project requires only one repository file and a compatible C++ compiler.

### Design goals

- **Small surface area:** one public class and five read/write operations.
- **No runtime dependencies:** uses the C/C++ standard library headers available on the host.
- **Readable storage:** preserves the familiar INI mental model of sections and key/value records.
- **Predictable defaults:** numeric reads return the caller-provided fallback when a section/key is not found.
- **Educational internals:** uses explicit linked-list structures, file I/O, parsing, and ownership cleanup.

## Capabilities

| Capability | Details |
| --- | --- |
| String reads | `ReadString(section, key, buffer)` copies a value into a caller-provided buffer. |
| Integer reads | `ReadInteger(section, key, defaultValue)` parses base-10 integers. |
| Hexadecimal reads | `ReadHexadecimal(section, key, defaultValue)` accepts formats understood by `strtol(..., 0)`, including `0x...`. |
| String writes | `WriteString(section, key, value)` creates or replaces an entry. |
| Integer writes | `WriteInteger(section, key, value)` creates or replaces an entry. |
| Section/key creation | Missing sections and keys are created when writing. |
| Automatic persistence | Changes are written when the `TIniFile` object is destroyed. |
| Inspection | `PrintIniFile()` prints the in-memory representation. |

## Architecture

The parser loads the file into a two-level singly linked list. `TIniFile` owns the section list; each `SECTION` owns a list of raw `PROFILE` records such as `porta=8080`.

```mermaid
classDiagram
    class TIniFile {
      -SECTION* pLstArquivo
      -SECTION* lsttmpSecao
      -PROFILE* lsttmpProfile
      -bool bAlterado
      -char* pstNomeArquivo
      +TIniFile(char* arquivo)
      +~TIniFile()
      +bool ReadString(char*, char*, char*)
      +int ReadInteger(char*, char*, int)
      +int ReadHexadecimal(char*, char*, int)
      +bool WriteString(char*, char*, char*)
      +bool WriteInteger(char*, char*, int)
      +void PrintIniFile()
    }
    class SECTION {
      +char* pstNome
      +PROFILE* VarValores
      +SECTION* prox
    }
    class PROFILE {
      +char* pstVarValor
      +PROFILE* prox
    }
    TIniFile "1" o-- "0..*" SECTION : owns
    SECTION "1" o-- "0..*" PROFILE : owns
```

### In-memory representation

```text
TIniFile
└── pLstArquivo ──► SECTION("Geral") ──► SECTION("Rede") ──► null
                    │                      │
                    ▼                      ▼
              PROFILE("nome=...")   PROFILE("porta=9090")
                    │                      └──► PROFILE("mascara=0xFF00FF00")
                    ▼
                  null
```

## Data flow

### Load and read

```mermaid
flowchart LR
    A[INI file on disk] --> B[TIniFile constructor]
    B --> C[Read lines with fgets]
    C --> D{Line type}
    D -->|[Section]| E[Create SECTION node]
    D -->|key=value| F[Create PROFILE node]
    D -->|blank or unsupported| G[Ignore]
    E --> H[In-memory linked lists]
    F --> H
    H --> I[FindSection]
    I --> J[FindVariavel]
    J --> K{Typed read}
    K -->|string| L[Copy value to caller buffer]
    K -->|integer| M[strtol base 10]
    K -->|hexadecimal| N[strtol base 0]
```

### Write and persist

```mermaid
flowchart LR
    A[WriteString / WriteInteger] --> B[Find section]
    B -->|found| C[Find key]
    B -->|missing| D[Create SECTION]
    C -->|found| E[Replace raw key=value text]
    C -->|missing| F[Append PROFILE]
    D --> F
    E --> G[Set bAlterado = true]
    F --> G
    G --> H[Object lifetime ends]
    H --> I{Changed?}
    I -->|yes| J[Open file with wt]
    J --> K[Write sections and records]
    K --> L[Free nodes]
    I -->|no| L
```

> **Important:** persistence is destructor-based. Keep the `TIniFile` object alive while performing all reads and writes, and allow it to leave scope normally so changes are flushed.

## Quick start

### Requirements

- A C++ compiler exposed as `g++`.
- GNU Make for the provided `Makefile`.
- C++11 support or newer.

### Build and run the example

```bash
git clone https://github.com/paulocfmarques-collab/inifiles.git
cd inifiles
make
make run
```

The example program recreates `teste.ini`, reads values, writes `endereco` and updates `porta`, prints the loaded structure, and persists the changes when `TIniFile` is destroyed.

### Direct compilation

Although the example file uses a `.c` suffix, it is compiled as C++ by `g++`:

```bash
g++ -Wall -Wextra -O2 -std=c++11 teste_inifiles.c -o teste_inifiles
./teste_inifiles
```

On Windows with MinGW:

```bash
g++ -Wall -Wextra -O2 -std=c++11 teste_inifiles.c -o teste_inifiles.exe
./teste_inifiles.exe
```

## Usage

```cpp
#include "inifiles.h"

int main()
{
    char fileName[] = "app.ini";
    char section[] = "Network";
    char hostKey[] = "host";
    char portKey[] = "port";
    char host[ TAM_STRING ] = { 0 };

    TIniFile ini(fileName);

    if (ini.ReadString(section, hostKey, host))
        printf("Host: %s\n", host);

    int port = ini.ReadInteger(section, portKey, 8080);
    printf("Port: %d\n", port);

    ini.WriteString(section, hostKey, (char*)"127.0.0.1");
    ini.WriteInteger(section, portKey, 9090);

    // Changes are written when `ini` is destroyed at the end of this scope.
    return 0;
}
```

### Example configuration

```ini
[General]
name=demo

[Network]
host=127.0.0.1
port=9090
mask=0xFF00FF00
```

### Example output

```text
Host: 127.0.0.1
Port: 8080
```

## API reference

| Method | Return value | Behavior |
| --- | --- | --- |
| `bool ReadString(char* section, char* key, char* value)` | `true` when found | Copies the text after `=` into `value`; returns `false` if the section or key is absent. |
| `int ReadInteger(char* section, char* key, int defaultValue)` | Parsed integer or default | Parses the value after `=` in base 10. |
| `int ReadHexadecimal(char* section, char* key, int defaultValue)` | Parsed integer or default | Parses using base `0`, supporting prefixes such as `0x`. |
| `bool WriteString(char* section, char* key, char* value)` | `true` | Creates or replaces `key=value` and marks the document as changed. |
| `bool WriteInteger(char* section, char* key, int value)` | `true` | Creates or replaces a decimal `key=value` record and marks the document as changed. |
| `void PrintIniFile()` | — | Prints sections and their raw records to standard output. |

`TAM_STRING` is defined as `255` and is the recommended size for buffers passed to `ReadString`.

## INI format

The parser recognizes:

- Section headers beginning with `[` and ending at the last `]`, for example `[Network]`.
- Records stored as `key=value` under the most recently parsed section.
- Empty lines, which are effectively ignored during loading.
- Semicolon-prefixed records, which are not returned by key lookup.

The current implementation stores records as raw strings and does not normalize whitespace or preserve comments as structured metadata.

## Build targets

The included `Makefile` exposes three targets:

```text
make          # Build teste_inifiles
make run      # Build, then execute teste_inifiles
make clean    # Remove the generated executable
```

Build flags are `-Wall -Wextra -O2 -std=c++11`.

## Behavior and limitations

This is intentionally a compact implementation. Before using it in a production configuration path, account for these characteristics:

- **Single-header implementation:** implementation and declarations are both in `inifiles.h`.
- **Fixed read buffer:** input lines are read into a `TAM_STRING`-sized buffer (`255` bytes).
- **C-style API:** public parameters use mutable `char*` buffers rather than `const char*` or `std::string`.
- **Destructor flush:** writes happen when the object is destroyed, not through an explicit `Save()` method.
- **Formatting is regenerated:** persistence writes the linked-list contents and does not preserve original formatting or comments as separate objects.
- **Simple lookup:** section and key matching is case-sensitive and uses linear traversal.
- **Not thread-safe:** an instance should not be concurrently mutated from multiple threads.
- **No exception-based error reporting:** file and parse failures are represented through return values or defaults where the API provides them.

These constraints keep the code approachable and small, while also defining where a more feature-rich parser may be appropriate.

## Project layout

```text
.
├── inifiles.h          # TIniFile declarations and implementation
├── teste.ini           # Example configuration generated/used by the test program
├── teste_inifiles.c    # Assert-based usage and smoke test
├── Makefile            # Build, run, and clean targets
└── README.md           # Project documentation
```

## Contributing

1. Fork the repository.
2. Create a focused branch:

   ```bash
   git checkout -b feature/your-change
   ```

3. Build and run the example:

   ```bash
   make clean
   make
   make run
   ```

4. Keep changes focused, document user-visible behavior, and open a pull request with a clear description.

Useful contribution areas include safer string handling, explicit save/error reporting, UTF-8 support, comment preservation, deletion APIs, broader tests, and a modern CMake build.

## Author

**Paulo Cesar Furlanetto Marques**  
[github.com/paulocfmarques-collab](https://github.com/paulocfmarques-collab)

---

<div align="center">

Made for small, readable C++ configuration workflows.

</div>
