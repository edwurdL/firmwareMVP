#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64   // change to 32 if yours is a 128x32 panel
#define OLED_RESET -1      // no dedicated reset pin on most I2C boards
#define SCREEN_ADDRESS 0x3C // common default; 0x3D on some boards

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); 


void setup() {
  Serial.begin(115200);
  delay(1000); // give serial monitor time to connect
  Serial.println("ESP32 boot OK - starting blink test");

  Wire.begin(21, 22); // SDA = D21, SCL = D22
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("SSD1306 allocation failed - check wiring/address");
    while (true) delay(10); // halt
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Hello, ESP32!");
  display.println("999999999999999");
  display.display(); // must call this to actually push buffer to screen
}

void loop() {
  
}