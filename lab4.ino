#include <TM1637Display.h>

#include <MD_MAX72xx.h>
#include <SPI.h>

#include <MD_MAX72xx.h>
#include <SPI.h>

#include <WiFi.h>
#include "time.h"
#include "esp_sntp.h"
#include <FirebaseESP32.h>
#include <addons/RTDBHelper.h>
#include <addons/TokenHelper.h>
#include <MD_MAX72XX.h>
#include <TM1637Display.h>

#define USER_EMAIL "testuser@gmail.com"
#define USER_PASSWORD "abc123"
#define API_KEY "AIzaSyB8jTIK02q5S_0xgqmRHAyeMm29zhkoKM8"
#define DATABASE_URL "https://trackpup-default-rtdb.asia-southeast1.firebasedatabase.app/"

// pakilagyan ng pins

#define MAX7219_DATA_PIN 19
#define MAX7219_CS_PIN 18
#define MAX7219_CLK_PIN 5
#define CLK 22
#define DIO 23
#define BTN 21

MD_MAX72XX mx = MD_MAX72XX(MAX7219_CS_PIN, 1, MAX7219_CLK_PIN);
TM1637Display display = TM1637Display(CLK, DIO);

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

const char *ssid = "Computer Engineering";
const char *password = "CPELABwifi3";

const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = 28800;  // GMT+8 (example)
const int daylightOffset_sec = 0;

// Timezone offsets
const long offset_TZ1 = 28800;   // Pinas
const long offset_TZ2 = -14400;  // US


String printLocalTime(struct tm *timeinfo, const char *timezone) {
  char buffer[64];
  strftime(buffer, sizeof(buffer), "%A, %B %d %Y %H:%M:%S", timeinfo);
  Serial.printf("Time in %s: %s\n", timezone, buffer);

  // Convert the buffer to a String and return it
  return String(buffer);
}

void timeavailable(struct timeval *t) {
  Serial.println("Got time adjustment from NTP!");
}

// Pattern for "U"
const uint8_t U[] = {
  B00111100,
  B01100110,
  B01100110,
  B01100110,
  B01100110,
  B01100110,
  B00111100,
  B00000000
};

// Pattern for "S"
const uint8_t S[] = {
  B00111100,
  B01000010,
  B01000000,
  B00111100,
  B00000010,
  B01000010,
  B00111100,
  B00000000
};

// Pattern for "P"
const uint8_t P[] = {
  B01111100,
  B01000110,
  B01000110,
  B01111100,
  B01000000,
  B01000000,
  B01000000,
  B00000000
};

// Pattern for "H"
const uint8_t H[] = {
  B01000010,
  B01000010,
  B01000010,
  B01111110,
  B01000010,
  B01000010,
  B01000010,
  B00000000
};

void displayPH() {
  // Display "P"
  mx.setColumn(0, P);  // Display "P" at column 0

  // Display "H"
  mx.setColumn(2, H);  // Display "H" at column 2
}

void displayUS() {
  // Display "U"
  mx.setColumn(0, U);  // Display "U" at column 0

  // Display "S"
  mx.setColumn(2, S);  // Display "S" at column 2
}

void setup() {
  Serial.begin(115200);

  // First step is to configure WiFi STA and connect in order to get the current time and date.
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");

  pinMode(BTN, INPUT_PULLUP);
  // digitalRead(BTN) = BTNSTATE;  //FLAG: QA
  mx.begin();
  mx.clear();
  delay(500);
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  sntp_set_time_sync_notification_cb(timeavailable);
  esp_sntp_servermode_dhcp(1);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

  display.clear();
  display.setBrightness(7);
}

void displayPHTime() {
  // Display Pinas time on 7-segment display (HHMM format)
  int hours = stoi(pinasTime.substring(11, 13));    // Extract hours
  int minutes = stoi(pinasTime.substring(14, 16));  // Extract minutes
  int displayTime = hours * 100 + minutes;          // Combine hours and minutes

  // Display on TM1637 7-segment display
  display.showNumberDecEx(displayTime, 0b11100000, true);
}

void displayUSTime() {
  int hours = stoi(usTime.substring(11, 13));    // Extract hours
  int minutes = stoi(usTime.substring(14, 16));  // Extract minutes
  int displayTime = hours * 100 + minutes;       // Combine hours and minutes

  // Display on TM1637 7-segment display
  display.showNumberDecEx(displayTime, 0b11100000, true);
}

void updatekomunatime() {
  delay(5000);

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("No time available (yet)");
    return;
  }

  // Print time in default timezone
  String defaultTime = printLocalTime(&timeinfo, "Default Timezone");

  // Convert and print time in first timezone (Pinas, GMT+8)
  time_t rawtime;
  time(&rawtime);
  rawtime += offset_TZ1 - gmtOffset_sec;
  struct tm *timeinfo_TZ1 = localtime(&rawtime);
  String pinasTime = printLocalTime(timeinfo_TZ1, "Pinas");

  // Write Pinas time to Firebase
  if (Firebase.set(fbdo, "Rooms/Room300/PHTime", pinasTime)) {
    Serial.println("Pinas Time Updated: " + pinasTime);
  } else {
    Serial.println("Failed: " + fbdo.errorReason());
  }

  // Convert and print time in second timezone (US, UTC-4)
  rawtime = time(NULL);
  rawtime += offset_TZ2 - gmtOffset_sec;
  struct tm *timeinfo_TZ2 = localtime(&rawtime);
  String usTime = printLocalTime(timeinfo_TZ2, "US");

  // Write US time to Firebase
  if (Firebase.set(fbdo, "Rooms/Room300/USTime", usTime)) {
    Serial.println("US Time Updated: " + usTime);
  } else {
    Serial.println("Failed: " + fbdo.errorReason());
  }
}
void loop() {
  updatekomunatime();
  // BTNSTATE = digitalRead(BTN);
  if (digitalRead(BTN) == LOW) {
    displayUSTime();
    displayUS();
  } else {
    displayPHTime();
    displayPH();
  }
}
