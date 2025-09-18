#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <Wire.h>
#include <RTClib.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CS_PIN 10

MD_Parola display = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
RTC_DS3231 rtc;

bool colonVisible = true;
unsigned long lastUpdate = 0;

void setup() {
  display.begin();
  display.setIntensity(5);  // Brightness (0-15)
  display.displayClear();

  Wire.begin();
  if (!rtc.begin()) {
    Serial.begin(9600);
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // Uncomment to set RTC to current compilation time
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastUpdate >= 500) {  // Blink colon every 500ms
    lastUpdate = currentMillis;
    colonVisible = !colonVisible;

    DateTime now = rtc.now();

    int hour = now.hour();
    // Convert to 12-hour format
    if (hour == 0) hour = 12;
    else if (hour > 12) hour -= 12;

    int minute = now.minute();

    char timeStr[6];  // HH:MM
    if (colonVisible) {
      sprintf(timeStr, "%02d:%02d", hour, minute);
    } else {
      sprintf(timeStr, "%02d %02d", hour, minute);
    }

    display.displayClear();
    display.setFont(nullptr); // Default font
    display.setTextAlignment(PA_CENTER); // Center the text
    display.print(timeStr);
  }
}