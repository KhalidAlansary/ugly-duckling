# Ugly Duckling

## Overview

Ugly Duckling is a simple yet powerful multipurpose XML utility written in C++.

## Table of Contents

- [Features](#features)
- [Limitations](#limitations)
- [Setup Development Environment](#setup-development-environment)
  - [Linux](#linux)
  - [Windows](#windows)
- [How to Clone and Build](#how-to-clone-and-build)

## Features

- Parses and validates XML files
- Fixes misplaced tags
- Formats XML
- Converts XML to JSON
- Minifies XML by removing whitespace
- Compresses and decompresses XML

## Limitations

Does not support the following:

- XML namespaces
- XML attributes
- prolog and doctype
- CDATA sections
- self-closing tags
- DTDs

## Setup Development Environment

### Linux

#### Ubuntu and Debian-based systems

1. Install the necessary development tools:
   ```bash
   sudo apt-get update
   sudo apt-get install build-essential cmake libboost-all-dev graphviz libqt6-dev
   ```

#### Arch-based systems

1. Install the necessary development tools:
   ```bash
   sudo pacman -Syu --needed base-devel cmake boost graphviz qt6
   ```

### Windows

1. Open PowerShell and paste the following to install MSYS2:
   ```powershell
   winget install MSYS2.MSYS2 Kitware.CMake Graphviz.Graphviz
   ```
2. Open the MSYS2 UCRT64 shell and install the necessary packages:
   ```bash
   pacman -Syu --needed base-devel mingw-w64-ucrt-x86_64-{toolchain,boost,graphviz,qt6}
   ```
3. Open PowerShell as adminstrator and add the MSYS2 UCRT64 bin directory to the system PATH (This may vary depending on where you installed MSYS2):
   ```powershell
   [System.Environment]::SetEnvironmentVariable("Path", $env:Path + ";C:\msys64\ucrt64\bin", [System.EnvironmentVariableTarget]::Machine)
   ```

## How to Clone and Build

1. Clone the repository:
   ```sh
   git clone https://github.com/KhalidAlansary/ugly-duckling.git
   ```
2. In the local repository, run:
   ```sh
   cd ugly-duckling
   cmake -B build
   cmake --build build
   cd build/bin
   ./xml_editor <command> [options]
   ```
