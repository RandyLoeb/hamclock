#include <Arduino.h>
#include <TM1637Display.h>
#include <NTPClient.h>
// change next line to use with another board/shield
#include <ESP8266WiFi.h>
//#include <WiFi.h> // for WiFi shield
//#include <WiFi101.h> // for WiFi 101 shield or MKR1000
#include <WiFiUdp.h>
#include <time.h>
#include "mywifi.h"
#include "wifiConfigWebServer.h"
#include "wifiUtils.h"

// Define the connections pins:
#define CLK D1
#define DIO D2
// Create display object of type TM1637Display:
TM1637Display display = TM1637Display(CLK, DIO);
// Create array that turns all segments on:
const uint8_t data[] = {0xff, 0xff, 0xff, 0xff};
// Create array that turns all segments off:
const uint8_t blank[] = {0x00, 0x00, 0x00, 0x00};

WiFiUDP ntpUDP;

// By default 'pool.ntp.org' is used with 60 seconds update interval and
// no offset
NTPClient timeClient(ntpUDP);

// You can specify the time server pool and the offset, (in seconds)
// additionaly you can specify the update interval (in milliseconds).
// NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

int millisInAMinute = (1000 * 60);
int checkPointIntervalMinutes = 10 * millisInAMinute;
int displayHours;
int displayMinutes;
int displaySeconds;
int displayInt;
int lastCheckPointMillis;

int checkPointHours;
int checkPointMinutes;
int checkPointSeconds;

time_t checkPointTime_t;
int lastDisplayInt;

int getDisplayInt()
{
  return displayHours * 100 + displayMinutes;
}

void timeCheckPoint()
{

  Serial.print("Display time is:");
  Serial.print(displayHours);
  Serial.print(":");
  Serial.print(displayMinutes);
  Serial.print(":");
  Serial.println(displaySeconds);
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());
  checkPointHours = timeClient.getHours();
  checkPointMinutes = timeClient.getMinutes();
  checkPointSeconds = timeClient.getSeconds();
  lastCheckPointMillis = millis();
  displayHours = checkPointHours;
  displayMinutes = checkPointMinutes;
  /*
  tm checkPointtm;
  checkPointtm.tm_hour = checkPointHours;
  checkPointtm.tm_min = checkPointMinutes;
  checkPointtm.tm_sec = checkPointSeconds;
  timeClient.getEpochTime
  checkPointTime_t = mktime(&checkPointtm);
  char buffer[80];

  tm *gmtm = localtime(&checkPointTime_t);
  strftime(buffer, 80, "Now it's %I:%M%p.", gmtm);

  Serial.println(buffer);
  */

  checkPointTime_t = timeClient.getEpochTime();
}

bool GLOBAL_AP_MODE = false;
WifiUtils _wifiUtils;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  // Set the display brightness (0-7):
  display.setBrightness(5);
  // Clear the display:
  display.clear();
  _wifiUtils.initialize();
  //WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  timeClient.begin();
  timeCheckPoint();
}

void loop()
{
  // put your main code here, to run repeatedly:
  int now = millis();
  int nowDiff = now - lastCheckPointMillis;
  int secondsPassed = (int)floor(nowDiff / 1000);
  time_t movementTime = checkPointTime_t + secondsPassed;
  tm gmtm = *gmtime(&movementTime);

  displayHours = gmtm.tm_hour;
  displayMinutes = gmtm.tm_min;
  displaySeconds = gmtm.tm_sec;
  if (nowDiff > checkPointIntervalMinutes)
  {
    timeCheckPoint();
  }
  int displayInt = getDisplayInt();
  if (displayInt != lastDisplayInt)
  {
    display.showNumberDecEx(displayInt, 0b11100000, true);
  }
  lastDisplayInt = displayInt;
}