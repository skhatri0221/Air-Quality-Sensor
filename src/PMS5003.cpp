#include <Arduino.h>
#include "PMS.h"           // pick the single PMS library you're using
// #include <PMS5003.h>    // REMOVE this if you're using PMS.h

// Use Serial2 for PMS sensor, specify RX/TX pins
HardwareSerial PMSserial(2);  // UART2 (Serial2)
PMS *pms = nullptr;          // will construct after begin()
PMS::DATA data;

// Example PMData struct (copy or use your existing one)
struct PMData {
  float pm1_0;
  float pm2_5;
  float pm10_0;
  bool valid;
};

void initPMS()
{
    // Serial2 on GPIO16 (RX2), GPIO17 (TX2)
    // RX is the ESP pin that listens to sensor TX
    const int RX_PIN = 16;
    const int TX_PIN = 17;

    // Start UART2 with explicit pins BEFORE creating PMS object
    PMSserial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
    Serial.println("PMS5003 sensor starting...");
    delay(200); // small settle time for UART

    // Construct the PMS object now that the Serial is initialized
    if (pms) { delete pms; pms = nullptr; }      // safety if reinit
    pms = new PMS(PMSserial);

    // Optional warm-up delay (sensor datasheet may recommend ~30s)
    // but keep it reasonable while debugging. Increase if sensor needs it.
    delay(2000);

    // Put sensor into passive mode (or active if you prefer)
    if (pms) {
      pms->passiveMode();   // use pointer call
      Serial.println("PMS set to passive mode");
    } else {
      Serial.println("PMS object not created!");
    }
}

PMData readPM()
{
  PMData result;
  result.valid = false;

  if (!pms) {
    Serial.println("PMS not initialized!");
    return result;
  }

  // Request a measurement read (passive mode)
  pms->requestRead();

  // readUntil returns true if a valid frame was parsed
  // it may block for a short time depending on library implementation —
  // consider using a timeout wrapper if necessary.
  if (pms->readUntil(data))
  {
    result.pm1_0 = data.PM_AE_UG_1_0;
    result.pm2_5 = data.PM_AE_UG_2_5;
    result.pm10_0 = data.PM_AE_UG_10_0;
    result.valid = true;
  }
  else
  {
    Serial.println("No data from sensor.");
  }
  return result;
}
