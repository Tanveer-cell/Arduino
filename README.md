---
title: Arduino CLI Complete Guide (Nano - New Bootloader)
description: End-to-end workflow for compiling and uploading Arduino projects using arduino-cli on Linux.
---

# 🚀 Arduino CLI Complete Workflow  
## Arduino Nano (New Bootloader) – Linux Setup

This repository documents a fully reproducible workflow for running Arduino scripts using `arduino-cli`, specifically for:

- ✅ Arduino Nano  
- ✅ New Bootloader  
- ✅ Linux (Arch-based example)  
- ✅ OLED (Adafruit SH1106) example project  

---

# 🧠 Overview

To run an Arduino script using `arduino-cli`, the process is:

1. Install arduino-cli  
2. Initialize configuration  
3. Install board core  
4. Install required libraries  
5. Create sketch  
6. Compile using correct FQBN  
7. Upload to board  
8. Use serial monitor (optional)  

For **Nano with NEW bootloader**, always use:

```
arduino:avr:nano
```

DO NOT use:

```
arduino:avr:nano:cpu=atmega328old
```

That is only for old bootloader boards.

---

# 🛠 1️⃣ Install Arduino CLI

Install via pacman:

```bash
sudo pacman -S arduino-cli
```

Initialize configuration:

```bash
arduino-cli config init
arduino-cli core update-index
```

---

# 🔧 2️⃣ Install Arduino Nano Core

```bash
arduino-cli core install arduino:avr
```

Verify installation:

```bash
arduino-cli core list
```

You should see:

```
arduino:avr
```

---

# 📚 3️⃣ Install Required Libraries

Example for OLED project (SH1106 display):

```bash
arduino-cli lib install "Adafruit GFX Library"
arduino-cli lib install "Adafruit SH110X"
```

Verify installed libraries:

```bash
arduino-cli lib list
```

---

# 📁 4️⃣ Create a New Sketch

```bash
arduino-cli sketch new my_project
cd my_project
```

Edit file:

```bash
nano my_project.ino
```

Paste your clean UTF-8 code (use LF line endings).

---

# 🔎 5️⃣ Detect Connected Board

Plug in your Nano.

```bash
arduino-cli board list
```

Example output:

```
/dev/ttyUSB0
```

Even if it shows `Unknown`, it is fine.

---

# ⚙️ 6️⃣ Compile (Nano – NEW Bootloader)

```bash
arduino-cli compile --fqbn arduino:avr:nano
```

If successful, you will see memory usage:

```
Sketch uses XXXX bytes
Global variables use XXX bytes
```

---

# ⬆️ 7️⃣ Upload to Arduino

```bash
arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:nano
```

If upload fails:

- Press reset just before upload
- Confirm correct port
- Verify bootloader type

---

# 🖥 8️⃣ Serial Monitor (Optional)

If using `Serial.begin()`:

```bash
arduino-cli monitor -p /dev/ttyUSB0 -c baudrate=9600
```

Change baudrate if needed.

---

# 🔥 Example Titanic Animation Code (SH1106)

```cpp
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <math.h>

Adafruit_SH1106G display(128, 64, &Wire, -1);

float phase = 0;
int shipX = 10;
int shipY = 30;
int stage = 0;

void setup() {
  display.begin(0x3C, true);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
}

void drawWaves() {
  for (int x = 0; x < 128; x++) {
    int y = 45 + 2 * sin(x * 0.2 + phase);
    display.drawPixel(x, y, SH110X_WHITE);
  }
}

void drawShip(int x, int y) {
  display.fillRect(x, y, 30, 5, SH110X_WHITE);
  display.fillRect(x + 8, y - 6, 12, 6, SH110X_WHITE);
  display.fillRect(x + 10, y - 10, 3, 4, SH110X_WHITE);
  display.fillRect(x + 15, y - 10, 3, 4, SH110X_WHITE);
}

void loop() {
  display.clearDisplay();
  drawWaves();

  display.fillTriangle(100, 45, 115, 25, 125, 45, SH110X_WHITE);

  if (stage == 0) {
    shipX += 1;
    drawShip(shipX, shipY);
    if (shipX > 65) stage = 1;
  }

  else if (stage == 1) {
    drawShip(shipX, shipY);
    display.setCursor(40, 10);
    display.print("CRASH!");
    display.display();
    delay(800);
    stage = 2;
  }

  else if (stage == 2) {
    shipY += 1;
    drawShip(shipX, shipY);

    for (int i = 0; i < 5; i++) {
      int bx = shipX + random(0, 30);
      int by = shipY + random(0, 10);
      display.drawPixel(bx, by, SH110X_WHITE);
    }

    if (shipY > 60) {
      display.clearDisplay();
      display.setCursor(30, 30);
      display.print("RIP TITANIC");
      display.display();
      while (1);
    }
  }

  display.display();
  phase += 0.1;
  delay(40);
}
```

---

# ⚠️ Common Issues & Fixes

## Permission Denied

```bash
sudo usermod -aG uucp $USER
```

Logout and login again.

---

## Bootloader Mismatch

If upload error:

```
avrdude: stk500_recv(): programmer is not responding
```

Try switching between:

```
arduino:avr:nano
```

and

```
arduino:avr:nano:cpu=atmega328old
```

---

## Windows Line Ending Errors

If you see:

```
stray '\342'
```

Fix using:

```bash
sudo pacman -S dos2unix
dos2unix your_file.ino
```

---

# ⚡ Daily Workflow

```bash
cd my_project
arduino-cli compile --fqbn arduino:avr:nano
arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:nano
```

---

# 🧩 Conceptual Model

| Term      | Meaning |
|-----------|---------|
| Core      | Board architecture support |
| Library   | External dependency |
| FQBN      | Board + processor configuration |
| Port      | USB device path |
| Compile   | Generate .hex file |
| Upload    | Flash microcontroller |

---

# 🎯 Conclusion

This setup provides:

- Reproducible builds  
- No GUI dependency  
- Scriptable workflow  
- Git-friendly structure  
- Professional embedded toolchain  

You now have a complete CLI-based Arduino development environment.
