#include <Arduino.h>
#include "PMS.h"
#include "HardwareSerial.h"
#include <PMS5003.h>
#include <LCD_DISPLAY.h>
#include <SCD41.h>
#include <SGP40.h>

// SCD41 object
SCD41 scd41(21, 22); // SDA, SCL pins
SGP40_Driver sgp40; 
LCD_DISPLAY lcd;

void setup()
{
  Serial.begin(9600);  // For debugging via USB
  Wire.begin(21, 22);

  initPMS();

  lcd.init();

   //Initialize SCD41
    if (!scd41.begin()) {
        Serial.println("SCD41 init failed!");
        // optionally show error on LCD
    } else {
        Serial.println("SCD41 initialized");
    }

    // Initialize SGP40
    if (!sgp40.begin()) {
        Serial.println("SGP40 init failed!");
    } else {
        Serial.println("SGP40 initialized");
    }
}

void loop()
{
    static unsigned long lastRead = 0;
    unsigned long now = millis();

    // Read all sensors every 5 seconds
    if (now - lastRead >= 5000) {
        lastRead = now;

        //Read PMS5003
        PMData reading = readPM();
        lcd.showPMS(reading);
        Serial.print("PM 1.0 (ug/m3): ");
        Serial.println(reading.pm1_0);

        Serial.print("PM 2.5 (ug/m3): ");
        Serial.println(reading.pm2_5);

        Serial.print("PM 10.0 (ug/m3): ");
        Serial.println(reading.pm10_0);

        Serial.println();

        // Read SCD41
        float co2, temp, hum;
        if (scd41.read(co2, temp, hum)) {
          lcd.showSCD(co2, temp, hum);
            Serial.printf("CO2: %.0f ppm, Temp: %.2f C, RH: %.2f %%\n",
                          co2, temp, hum);
            // showSCD(co2, temp, hum);  // optional LCD display
        } else {
            Serial.println("SCD41 data not ready");
        }

        // Read SGP40
        float vocIndex = sgp40.readVocIndex(temp, hum);
        if (vocIndex >= 0) {
            lcd.showSGP40(vocIndex);
            Serial.printf("VOC Index: %.2f\n", vocIndex);
        } else {
            Serial.println("Failed to read VOC index from SGP40");
        }
    }
}