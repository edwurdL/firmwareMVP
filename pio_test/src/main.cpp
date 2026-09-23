#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h> // screeen driver
#include "Adafruit_VL53L1X.h" // laser sensor driver

// OLED screen info
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64   
#define OLED_RESET -1      // no dedicated reset pin on most I2C boards
#define SCREEN_ADDRESS 0x3C

// i2c bus
#define SDA 21
#define SCL 22

// VL53L1X laser sensor 
#define VL53_XSHUT_PIN 4

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); 
Adafruit_VL53L1X vl53 = Adafruit_VL53L1X(VL53_XSHUT_PIN);


void setup() {
  Serial.begin(115200);
  delay(1000); // give serial monitor time to connect
  Serial.println("ESP32 boot OK - starting blink test");

  Wire.begin(SDA, SCL);
  
  pinMode(VL5_XSHUT_PIN, OUTPUT);
  digitalWrite(VL5_XSHUT_PIN, HIGH); // enable sensor before begin()
  delay(10); // brief settle time after power-up

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("SSD1306 allocation failed - check wiring/address");
    while (true) delay(10);
  }

  if(!vl53.begin(0x29, &Wire)){
    Serial.print("Error initializing VL53L1X: ");
    Serial.println(vl53.vl_status);
    while (1) delay(10);
  }

  Serial.println("VL52L1X initialized.");

  if (!vl53.startRanging()) {
    Serial.print("Couldn't start ranging: ");
    Serial.println(vl53.vl_status);
    while (1) delay(10);
  }
  Serial.println("Ranging started");
  
  vl53.setTimingBudget(50); // updates every 50ms
}

void loop() {
  int16_t distance; // distance for VL53 sensor

  if (vl53.dataReady()) {
    distance = vl53.distance();

    if (distance == -1) {
      // something went wrong, print error and keep going
      Serial.print("Couldn't get distance: ");
      Serial.println(vl53.vl_status);
      return;
    }

    vl53.clearInterrupt(); // required before next reading is available

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" mm");

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Distance:");

    display.setTextSize(2);
    display.setCursor(0, 20);
    display.print(distance);
    display.println(" mm");

    display.display();
  }
}