# Zydis Disassembler Frontend

A simple **interactive GUI frontend** built on top of the [Zydis](https://github.com/zyantific/zydis) disassembly library. The project focuses on fast navigation, clean presentation, and an extensible architecture suitable for reverse‑engineering tools.

This is intentionally a *viewer-first* disassembler: decode instructions, navigate control flow, and inspect details without heavy analysis.

---

## Features

* Zydis-powered x86/x64 instruction decoding
* Scrollable disassembly view
* Go‑to address navigation
* Follow branch / call targets
* Interactive command console
* Instruction details panel
* Modular architecture (UI, disassembly, commands)

---

## Project Structure

```
.
├── CMakeLists.txt
└── src
    ├── app        # Main window and application startup
    ├── console    # Interactive command system
    ├── dialogs    # Modal dialogs (e.g. Go To)
    ├── disasm     # Disassembly model and view
    ├── widgets    # Reusable UI widgets
    └── main.cpp
```

---

## Building

### Requirements

* C++17 compatible compiler
* CMake ≥ 3.16
* Zydis
* A supported GUI toolkit (currently using a widgets-based UI)

### Build Steps

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

---

## Usage

Launch the application and load a binary to begin disassembly. Navigation is primarily keyboard‑driven:

* **Go to address**: `Ctrl+G`
* **Follow branch/call**: `Enter`
* **Navigate back / forward**: `Alt+Left` / `Alt+Right`
* **Open command console**: `:` or via toolbar

---

## Status

Early development. APIs, UI, and file layout are expected to change.

