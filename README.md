# Ugly Duckling

## Overview
Ugly Duckling is a simple XML parser written in C++. It reads an XML file and prints the XML tree using BFS.

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
    sudo apt-get install build-essential cmake
    ```

#### Arch-based systems
1. Install the necessary development tools:
    ```sh
    sudo pacman -Syu
    sudo pacman -S base-devel cmake
    ```

### Windows
1. Open PowerShell and paste the following to install MSYS2:
    ```sh
    winget install MSYS2.MSYS2
    ```
2. Open the MSYS2 UCRT64 shell and install the necessary packages:
    ```sh
    pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain cmake
    ```

## How to Clone and Build
1. Clone the repository:
    ```sh
    git clone git@github.com:KhalidAlansary/ugly-duckling.git
    ```
2. In the local repository, run:
    ```sh
    cd ugly-duckling
    cmake -B build
    cd build
    ./bin/ugly-duckling
    ```