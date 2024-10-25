### Why MinGW Instead of WSL?
WSL (Windows Subsystem for Linux) provides a Linux environment running on Windows. While it is a powerful tool, using it introduces some complexity, as the WSL environment must interface with Windows components to access features such as networking, GPU, and window management. For example, to use the internet, WSL must interface with the Windows network adapter, and external programs like XLaunch are required to display graphical applications. Similarly, to access OpenGL features, `mesa-utils` needs to be installed and updated, and there may be issues related to VPN or network configurations. 

On the other hand, **MinGW** compiles 42 as a native Windows executable, which means that you can directly use Windows components for window management, networking, and GPU access without extra dependencies or workarounds. MinGW targets native Windows APIs, so no additional software like XLaunch is needed.

NOTE: NOS3 requires linux. If you need NOS3 support, than WSL will be the answer.

### Steps to Get 42 Working on Windows Using MinGW:

1. **Install MSYS2**:
   - We use the MSYS2 version of MinGW rather than the standalone versions like on SourceForge.net
   - MSYS2 version will come with a package manager (pacman) that is useful for keeping our dependencies up to date, and removes the requirement for the historical 42ExternalSupport folder where dependencies were manually installed and updated
   - Download and install MSYS2 from the official website https://www.msys2.org/ using the `.exe` installer.
   - The **MSYS2 MinGW 64-bit** comes with the MSYS2 installation.

2. **Open an MSYS2 Prompt**:
   - MSYS2 should start automatically after installation. If not, open the **MSYS2 MSYS** terminal from the Start menu.
   - **Note**: There is a distinction between MSYS2 and MinGW:
     - **MSYS2** is a POSIX-like environment that provides a Unix-style shell.
     - **MinGW** (within MSYS2) is used for building **Windows-native executables**.

3. **Update MSYS2**:
   - You need to update MSYS2 and MinGW. The terminal may need to restart during this process. Run these commands in sequence:
     ```bash
     pacman -Syu
     pacman -Su
     ```

4. **Open an MSYS2 MinGW Terminal**:
   - Once updated, open the **MSYS2 MinGW 64-bit** terminal for compiling 64-bit Windows executables (or **MSYS2 MinGW 32-bit** for 32-bit executables, depending on your target).

5. **Install FreeGLUT and GLEW**:
   - Run the following command to install FreeGLUT and GLEW:
     ```bash
     pacman -S mingw-w64-x86_64-freeglut mingw-w64-x86_64-glew
     ```
   - **Note**: By using **MinGW**, you target Windows binaries, not Unix-like systems.

6. **Install the GCC Toolchain**:
   - To compile 42, you'll need the MinGW GCC toolchain:
     ```bash
     pacman -S mingw-w64-x86_64-toolchain
     ```
   - Verify that you have access to gcc from the MinGW terminal:
      ```bash
      gcc --version
      ```
   - If not, add MinGW64 to your path from the MinGW terminal and verify that you have access to gcc again:
      ```bash
         export PATH="/mingw64/bin:$PATH"
      ```
7. **Install `make`**:
   - You'll also need the `make` utility to run the Makefile:
     ```bash
     pacman -S make
     ```

8. **Get the 42 Repository**:
   - You can clone the 42 repository using **Git**. You can either use a Git client in Windows or install Git in MSYS2:
     - If using a Windows Git client:
       - Clone the repository into a standard Windows directory (e.g., `C:/Users/jpdavis6/42`).
       - From the MinGW terminal, you can access this directory as `/c/Users/jpdavis6/42`.
     - Alternatively, you could install Git in MSYS2 and clone the repository from there, but using a Windows Git client simplifies access.

9. **Navigate to the 42 Root Directory in the MinGW Terminal**:
   - Use the MinGW terminal to navigate to the cloned repository:
     ```bash
     cd /c/Users/<user>/42
     ```

10. **Compile the Repository**:
    - Run the `make` command to compile the project:
      ```bash
      make
      ```
    - This will compile 42 as a native Windows executable (`42.exe`).

11. **Run 42**:
    - After compilation, you can run the executable:
      ```bash
      ./42.exe
      ```

12. **Check for Errors**:
    - If there are no errors, 42 should run successfully with native Windows integration.

### Additional Notes:
- Ensure you are always using the **MSYS2 MinGW terminal** for compiling and running 42, as this targets Windows-native executables. Do not use the **MSYS2 MSYS** terminal for compiling Windows applications.
- If the `42.exe` file is created without the `.exe` extension, you can manually rename it or run it by typing the exact name (`42.exe`).


### To open **MinGW** from **VS Code**, follow these steps to integrate it as your terminal:

### 1. **Install MSYS2/MinGW**
   Make sure you have **MSYS2** installed, which includes MinGW. If not, you can download and install it from [MSYS2.org](https://www.msys2.org/).

### 2. **Find Your MinGW Path**
   Locate the **MinGW** shell executable within the MSYS2 installation directory:
   - For **64-bit MinGW**, the path is typically:
     ```
     C:\msys64\mingw64.exe
     ```
   - For **32-bit MinGW**, the path is:
     ```
     C:\msys64\mingw32.exe
     ```

### 3. **Configure VS Code to Use MinGW Terminal**
   You can configure **VS Code** to use the **MinGW** shell as its integrated terminal.

   - Open **VS Code**.
   - Go to **File** > **Preferences** > **Settings** (or press `Ctrl + ,`).
   - In the search bar at the top of the Settings window, type `terminal integrated shell`.
   - Under **Terminal > Integrated > Shell: Windows**, click on **Edit in settings.json** to manually edit the configuration file.

   Add the following configuration to add **MinGW** to the list of terminals:
   ```json
   "terminal.integrated.profiles.windows": {
        "MinGW64": {
            "path": "C:\\msys64\\usr\\bin\\bash.exe",
            "args": ["-l"],
            "env": {
                "CHERE_INVOKING": "1",
                "MSYSTEM": "MINGW64"
            }
        }
    },
   ```

### 4. **Open VS Code Terminal**
   After configuring, you can open the terminal in **VS Code** by going to **Terminal** > **New Terminal** (or pressing `Ctrl + `).

The dropdown should contain an entry of MinGW64. That should open as an integrated terminal in VS Cdoe. You can now compile and run your projects using **gcc**, **make**, and other MinGW tools directly from VS Code.