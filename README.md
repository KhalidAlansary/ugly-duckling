# Ugly Duckling

## Overview

Ugly Duckling is a simple yet powerful multipurpose XML utility written in C++.

## Features

- Parses and validates XML files
- Fixes misplaced tags
- Formats XML
- Converts XML to JSON
- Minifies XML by removing whitespace
- Compresses and decompresses XML

## Table of Contents

- [Setup Development Environment](#setup-development-environment)
  - [Linux](#linux)
  - [Windows](#windows)
- [How to Clone and Build](#how-to-clone-and-build)

## Setup Development Environment

### Linux

#### Ubuntu and Debian-based systems

1. Install the necessary development tools:
   ```sh
   sudo apt-get update
   sudo apt-get install build-essential cmake libboost-all-dev
   ```

#### Arch-based systems

1. Install the necessary development tools:
   ```sh
   sudo pacman -Syu base-devel cmake boost
   ```

### Windows

1. Open PowerShell and paste the following to install MSYS2:
   ```sh
   winget install MSYS2.MSYS2 Kitware.CMake
   ```
2. Open the MSYS2 UCRT64 shell and install the necessary packages:
   ```sh
   pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-boost
   ```
3. Open PowerShell as adminstrator and add the MSYS2 UCRT64 bin directory to the system PATH (This may vary depending on where you installed MSYS2):
   ```powershell
   [System.Environment]::SetEnvironmentVariable("Path", $env:Path + ";C:\msys64\mingw64\bin", [System.EnvironmentVariableTarget]::Machine)
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
   make -C build
   cd build/bin
   ./xml_editor <command> [options]
   ```
