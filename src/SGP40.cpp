#include "SGP40.h"
#include <Arduino.h>

SGP40_Driver::SGP40_Driver() {
    // Constructor
}

bool SGP40_Driver::begin() {
    if (!sgp40.begin()){
        Serial.println("SGP40 not found!");
        return false;
    }
    return true;
}

float SGP40_Driver::readVocIndex(float temp, float hum) {
    // read raw value later if desired
    int32_t voc_index = sgp40.measureVocIndex(temp, hum);
    if (voc_index >= 0) {
        return (float)voc_index;
    }
    else {
        Serial.println("Failed to read VOC index from SGP40");
        return -1.0; // Indicate error
    }
}