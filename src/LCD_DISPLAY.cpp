#include "LCD_DISPLAY.h"
#include <Arduino.h>

LCD_DISPLAY::LCD_DISPLAY() {
    // Constructor, nothing to init here
}

void LCD_DISPLAY::init() {
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    drawHeader();
}

void LCD_DISPLAY::drawHeader() {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(0, 0);
    tft.println("Air Quality Monitor");
    tft.drawLine(0, 20, tft.width(), 20, TFT_WHITE);
}

// Display PMS5003 readings
void LCD_DISPLAY::showPMS(const PMData &pmData) {
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(0, 30);
    tft.printf("PM1.0 : %d ug/m3\n", pmData.pm1_0);
    tft.setCursor(0, 50);
    tft.printf("PM2.5 : %d ug/m3\n", pmData.pm2_5);
    tft.setCursor(0, 70);
    tft.printf("PM10  : %d ug/m3\n", pmData.pm10_0);
}

// Display SCD41 readings
void LCD_DISPLAY::showSCD(float co2, float temperature, float humidity) {
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(0, 100);
    tft.printf("CO2  : %.0f ppm\n", co2);
    tft.setCursor(0, 120);
    tft.printf("Temp : %.1f C\n", temperature);
    tft.setCursor(0, 140);
    tft.printf("RH   : %.1f %%\n", humidity);
}

// Display SGP40 VOC index
void LCD_DISPLAY::showSGP40(float vocIndex) {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(0, 170);
    tft.printf("VOC Index: %.2f\n", vocIndex);
}

void LCD_DISPLAY::clearScreen() {
    tft.fillScreen(TFT_BLACK);
    drawHeader();
}
