#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <math.h>

Adafruit_SH1106G display(128, 64, &Wire, -1);

float phase = 0;
int shipX = 10;
int shipY = 30;
int stage = 0;   // 0 = sailing, 1 = hit, 2 = sinking

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
  // Ship body
  display.fillRect(x, y, 30, 5, SH110X_WHITE);

  // Cabin
  display.fillRect(x + 8, y - 6, 12, 6, SH110X_WHITE);

  // Chimneys
  display.fillRect(x + 10, y - 10, 3, 4, SH110X_WHITE);
  display.fillRect(x + 15, y - 10, 3, 4, SH110X_WHITE);
}

void loop() {

  display.clearDisplay();
  drawWaves();

  // Iceberg
  display.fillTriangle(100, 45, 115, 25, 125, 45, SH110X_WHITE);

  if (stage == 0) {
    shipX += 1;
    drawShip(shipX, shipY);

    if (shipX > 65) {
      stage = 1;
    }
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

    // Bubbles
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
