#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BNO08x.h>

// OLED Display settings (0.96 inch is usually 128x64)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// BNO08x Sensor instance
Adafruit_BNO08x bno08x;
sh2_SensorValue_t sensorValue;

void setup() {
  Serial.begin(115200);
  
  // Initialize I2C for ESP32 (SDA = 21, SCL = 22)
  Wire.begin(21, 22);

  // 1. Initialize OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while(true); 
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Starting BNO08x...");
  display.display();

  // 2. Initialize BNO08x
  if (!bno08x.begin_I2C(0x4B)) {
    Serial.println("Failed to find BNO08x chip");
    display.println("BNO08x Init Failed!");
    display.display();
    while (true); 
  }
  Serial.println("BNO08x Found!");

  // Enable the Game Rotation Vector report at 50,000 microseconds (50ms / 20Hz)
  if (!bno08x.enableReport(SH2_GAME_ROTATION_VECTOR, 50000)) {
    Serial.println("Could not enable rotation vector");
  }
}

void loop() {
  // Check if a new sensor event is available
  if (bno08x.wasReset()) {
    Serial.print("Sensor was reset ");
    bno08x.enableReport(SH2_GAME_ROTATION_VECTOR, 50000);
  }

  if (bno08x.getSensorEvent(&sensorValue)) {
    // Only process Game Rotation Vector reports
    if (sensorValue.sensorId == SH2_GAME_ROTATION_VECTOR) {
      
      // Extract Quaternion values (w = real, x = i, y = j, z = k)
      float w = sensorValue.un.gameRotationVector.real;
      float x = sensorValue.un.gameRotationVector.i;
      float y = sensorValue.un.gameRotationVector.j;
      float z = sensorValue.un.gameRotationVector.k;

      // Convert quaternions to Euler angles (Roll, Pitch, Yaw) in radians, then to degrees
      float roll = atan2(2.0 * (w * x + y * z), 1.0 - 2.0 * (x * x + y * y)) * 180.0 / PI;
      float pitch = asin(2.0 * (w * y - z * x)) * 180.0 / PI;
      float yaw = atan2(2.0 * (w * z + x * y), 1.0 - 2.0 * (y * y + z * z)) * 180.0 / PI;

      // Clear the display for the new frame
      display.clearDisplay();
      display.setCursor(0, 0);
      
      // Make the text slightly larger for easier reading
      display.setTextSize(2);
      
      // Print the Euler angles in degrees
      display.print("R: "); display.println(roll, 2);
      display.print("P: "); display.println(pitch, 2);
      display.print("Y: "); display.println(yaw, 2);

      // Push the buffer to the screen
      display.display();
    }
  }
  
  delay(10); 
}