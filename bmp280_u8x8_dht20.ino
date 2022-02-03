#include <Arduino.h>
#include <U8x8lib.h>
#include <Wire.h>
#include "Seeed_BMP280.h"
#include "DHT.h"

#define DHTTYPE DHT20
DHT dht(DHTTYPE);

#if defined(ARDUINO_ARCH_AVR)
#define debug Serial

#elif defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_SAM)
#define debug SerialUSB
#else
#define debug Serial
#endif

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/U8X8_PIN_NONE);
BMP280 bmp280;
  int LED = 6;

void setup(void) {
  Serial.begin(9600);
  Serial.println("DHTxx test!");

  if (!bmp280.init()) {
    Serial.println("Device not connected or broken!");
  }

  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFlipMode(1);

  Wire.begin();
  dht.begin();

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void loop(void) {
  float temp_hum_val[2] = { 0 };
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float pressure = bmp280.getPressure();
  float temp = bmp280.getTemperature();
  float alti = bmp280.calcAltitude(pressure);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 0);
  u8x8.print("Tempi: ");
  u8x8.print(temp);
  u8x8.print(" C");
  u8x8.setCursor(0, 1);
  u8x8.print("Pa: ");
  u8x8.print(pressure);
  u8x8.setCursor(0, 2);
  u8x8.print("Alt: ");
  u8x8.print(alti);

  if (!dht.readTempAndHumidity(temp_hum_val)) {
    u8x8.setCursor(0, 3);
    u8x8.print("Humi: ");
    u8x8.print(temp_hum_val[0]);
    u8x8.print(" %\t");
    u8x8.setCursor(0, 4);
    u8x8.print("Temp: ");
    u8x8.print(temp_hum_val[1]);
    u8x8.println(" C");
  } else {
    u8x8.println("Failed to get temprature and humidity value.");
  }
  Serial.println("serial test");

  if (temp_hum_val[0] > 60.00) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }

  u8x8.refreshDisplay();
  delay(1000);
}