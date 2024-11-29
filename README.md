# Setup Development Environment

## Linux

Instructions for setting up the development environment on Linux.

### Ubuntu

1. Update the package list and install the required packages:
   ```sh
   sudo apt update && sudo apt install --needed build-essential gcc g++ libgtk-4-dev
   ```

### Arch

1. Update the package list and install the required packages:
   ```sh
   sudo pacman -Syu --needed base-devel gcc gtk4
   ```

## Windows

To set up the development environment on Windows, follow these steps:

1. Open PowerShell and install MSYS2:

   ```powershell
   winget install MSYS2.MSYS2
   ```

2. Open MSYS2 UCRT and run the following commands:

   ```sh
   pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-gtk4 mingw-w64-x86_64-boost
   ```

3. Add the MSYS2 UCRT64 bin directory to the system's PATH environment variable:
   - Open the Start Menu, search for "Environment Variables", and select "Edit the system environment variables".
   - In the System Properties window, click on the "Environment Variables" button.
   - In the Environment Variables window, find the "Path" variable in the "System variables" section and click "Edit".
   - Add `C:\msys64\ucrt64\bin` to the list of paths and click "OK".

