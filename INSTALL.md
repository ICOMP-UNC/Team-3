
# 🚀 Installation and Configuration Guide

This guide provides detailed instructions on how to compile, flash, and run the firmware on the STM32F1 microcontroller using PlatformIO in Visual Studio Code.

## 1. 🛠️ Prerequisites

#### Required Tools
- **Visual Studio Code (VSC)** - Development environment.
- **PlatformIO** - Extension for VSC that enables firmware compilation and flashing.
- **ST-Link Driver** - Necessary for the system to recognize the STM32.

#### Platform Compatibility
This project is compatible with the following operating systems:
- 🖥️ **Windows**
- 🍏 **macOS**
- 🐧 **Linux**

---

## 2. ⚙️ Setting Up the Development Environment

1. **Install Visual Studio Code** from its [official website](https://code.visualstudio.com/).
2. **Install PlatformIO**:
   - Open Visual Studio Code.
   - Go to the Extensions tab (`Ctrl+Shift+X`).
   - Search for "PlatformIO IDE" and install it.
3. **Install the ST-Link driver** (Windows only):
   - Download the driver from the official STMicroelectronics website: [STSW-LINK009](https://www.st.com/en/development-tools/stsw-link009.html).
   - Follow the driver installation instructions provided on the website.

---

## 3. 🧩 Compiling the Code

1. Open the project in Visual Studio Code.
2. Ensure the PlatformIO extension is enabled.
3. Download or clone this repository with:

   ```sh
   git clone https://github.com/ICOMP-UNC/Team-3.git
   cd Team-3
   ```

4. To compile the code, click on the **Build** option in the PlatformIO menu in the taskbar (🛠️ checkmark icon).
   - Alternatively, you can use the shortcut `Ctrl+Alt+B` to start the build.
   - PlatformIO will automatically download and configure the project-specific dependencies during the compilation process.

---

## 4. 🔌 Flashing the Firmware on the STM32F1

1. Connect the STM32F1 board to the computer using an ST-Link debugger.
2. Ensure PlatformIO is set up to use ST-Link as the programmer. This can be verified in the project's `platformio.ini` file:
   ```ini
   upload_protocol = stlink
   ```
3. To flash the firmware, select the **Upload** option in the PlatformIO menu (➡️ right arrow icon).
    - Alternatively, you can use the shortcut `Ctrl+Alt+U` to start the upload or write this in your VSCode terminal:
      ```sh
      pio run --target upload
      ```
4. Wait for the process to complete. Once finished, the firmware will be flashed onto the STM32F1 board.

---

## 5. 🐛 Common Troubleshooting

### 🛑 Issue: Board Not Detected
   - **Windows**: Verify that the ST-Link driver is correctly installed.
   - **macOS/Linux**: Ensure USB permissions are set correctly and that PlatformIO is authorized to access the device.

### 🛑 Issue: ST-Link Communication Error
   - Check that the `upload_protocol = stlink` option is correctly set in `platformio.ini`.
   - Ensure the ST-Link cable is securely connected and that the board is powered on.

---
