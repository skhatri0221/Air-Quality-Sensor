#include <Arduino.h>
#include "SCD41.h"
#include <Wire.h>

SCD41::SCD41(int sdaPin, int sclPin)
    : _sdaPin(sdaPin), _sclPin(sclPin) 
{}

SCD41::~SCD41() {}

bool SCD41::begin() {
    Wire.begin(_sdaPin, _sclPin);  // Initialize I2C on ESP32 with provided pins

    int16_t error;

    // Initialize sensor (pass Wire instance for I2C)
    _sensor.begin(Wire, SCD41_I2C_ADDR_62);
    delay(30); // Ensure sensor in clean state

    // Wake up sensor
    error = _sensor.wakeUp();
    if (error != NO_ERROR) printError(error);

    // Stop any ongoing measurement
    error = _sensor.stopPeriodicMeasurement();
    if (error != NO_ERROR) printError(error);

    // Reinitialize
    error = _sensor.reinit();
    if (error != NO_ERROR) printError(error);

    // Start periodic measurement (5s interval)
    error = _sensor.startPeriodicMeasurement();
    if (error != NO_ERROR) {
        printError(error);
        return false;
    }

    return true;
}

bool SCD41::read(float &co2, float &temperature, float &humidity) {
    int16_t error;
    bool ready = false;

    error = _sensor.getDataReadyStatus(ready);
    if (error != NO_ERROR) {
        printError(error);
        return false;
    }

    if (!ready) return false;  // Non-blocking

    uint16_t co2Raw;
    float tempRaw, humRaw;

    error = _sensor.readMeasurement(co2Raw, tempRaw, humRaw);
    if (error != NO_ERROR) {
        printError(error);
        return false;
    }

    co2 = (float)co2Raw;
    temperature = tempRaw;
    humidity = humRaw;

    return true;
}

void SCD41::printError(int16_t error) {
    errorToString(error, _errorMessage, sizeof(_errorMessage));
    Serial.print("SCD41 Error: ");
    Serial.println(_errorMessage);
}
