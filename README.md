<div align="center">

# 📁 TIniFile for C++

### Lightweight INI Configuration File Parser and Writer

![Cps://img.shields.io/badge/Language-C%2B%2B-blue.svg
![Platform](https://imgbadge/Platform-Windows%20%7C%20Linux-green.svg
![Licensemg.shields.io/badge/License-MIT-yellow.svg
![Status](https://img.shields.io/badge/Status-Stablevg

Simple, lightweight and dependency-free library for reading and writing INI configuration files in C++.

⭐ If you find this project useful, consider giving it a star!

</div>

---

## ✨ Features

- ✅ Read strings from INI files
- ✅ Read integer values
- ✅ Read hexadecimal values
- ✅ Write strings
- ✅ Write integers
- ✅ Automatically create sections and keys
- ✅ Automatic persistence on file update
- ✅ No external dependencies
- ✅ Lightweight and easy to integrate
- ✅ Educational implementation using linked lists

---

## 📂 Project Structure

```text
.
├── inifiles.h
├── teste.ini
└── teste_inifiles.c
```

### Files

| File | Description |
|--------|-------------|
| `inifiles.h` | Implementation of the `TIniFile` class |
| `teste.ini` | Example configuration file |
| `teste_inifiles.c` | Usage and testing example |

---

## 📖 Example INI File

```ini
[Geral]
nome=teste-inifiles

[Rede]
porta=8080
mascara=0xFF00FF00
```

---

## 🚀 Getting Started

### Linux

```bash
g++ teste_inifiles.c -o teste
./teste
```

### Windows (MinGW)

```bash
g++ teste_inifiles.c -o teste.exe
teste.exe
```

---

## 💻 Basic Usage

```cpp
#include "inifiles.h"

int main()
{
    TIniFile ini("teste.ini");

    char valor[TAM_STRING];

    if (ini.ReadString("Geral", "nome", valor))
    {
        printf("Nome: %s\n", valor);
    }

    int porta = ini.ReadInteger(
        "Rede",
        "porta",
        -1
    );

    printf("Porta: %d\n", porta);

    ini.WriteString(
        "Rede",
        "endereco",
        (char*)"127.0.0.1"
    );

    ini.Write*nteger(
        "Rede",
        "p*rta",
        9090
    );

    ret*rn 0;
}
```

---

## 📸 Example Ou*put

```text
nome = teste-inifiles*porta = 8080
mascara*= 0xFF00FF00

Conteudo carregado:
*[Geral]
    nome=teste-inifiles

[*ede]
    porta=*090
    mascara=0xFF00FF00
    end*reco=127.0.0.1

O arquivo*sera atualizado ao finalizar o tes*e.
```

---

## 🖼 Screenshot

Cri* um diretório:

```text
assets/
``*

Salve uma captura da execução co*o:

```text
assets/demo.png
```

*epois adicione ao README:

```mark*own
## Screenshot

assets/demo.png*```

*--

## 🔧 API Reference

### Read*tring

*``cpp
bool ReadString(
    char* s*cao,
    char* variavel,
    char**valor
);
```

Reads a string value*from a specified section.

---

##* ReadInteger

```cpp
int ReadInteg*r(
    char* secao,
    char* vari*vel,
    int defaultValue
);
```

*eads an integer value.

---

### R*adHexadecimal

```cpp
int ReadHexa*ecimal(
    char* secao,
    char**variavel,
    int defaultValue
);
*``

Reads a hexadecimal value.

--*

### WriteString

```cpp
bool WriteString(
    char* secao,
    char* variavel,
    char* valor
);
```

Creates or updates a string entry.

---

### WriteInteger

```cpp
bool WriteInteger(
    char* secao,
    char* variavel,
    int valor
);
```

Creates or updates an integer entry.

---

## 🏗 Internal Architecture

The library is built around:

- Object-Oriented Programming
- Dynamic Memory Allocation
- Singly Linked Lists
- Text File Parsing
- Runtime Configuration Management

Structure hierarchy:

```text
TIniFile
└── SECTION
    └── PROFILE
```

---

## 📚 Educational Value

This project can be used as a learning resource for:

- C/C++
- Object-Oriented Programming
- Linked Lists
- Dynamic Memory Management
- File Manipulation
- Configuration Management
- Data Structures

---

## 🔮 Possible Improvements

Future enhancements may include:

- UTF-8 support
- Comment preservation
- Section deletion
- Variable deletion
- Case-insensitive search
- Cross-platform unit tests
- CMake build support

---

## 🤝 Contributing

Contributions are welcome.

```bash
git fork
git checkout -b feature/new-feature
git commit -m "Add new feature"
git push origin feature/new-feature
```

Then open a Pull Request.

---

## 👨‍🏫 Author

### Paulo Cesar Furlanetto Marques

Professor DNS I

GitHub:

🔗 https://github.com/paulocfmarques-collab

---

## ⭐ Support

If you found this project useful:

- Give it a Star ⭐
- Fork it 🍴
- Share it 🚀

---

## 📄 License

This project is distributed under the MIT License.

Feel free to use, modify and distribute it.
