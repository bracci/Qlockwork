//*****************************************************************************
// zytQuadrat
// An advanced firmware for a DIY "word-clock"
//
// @mc ESP8266
// @created 12.03.2017
//
// This source file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This source file is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//*****************************************************************************

#define FIRMWARE_VERSION 20240214

#include <Arduino.h>
#include <Arduino_JSON.h>
#include <ArduinoHttpClient.h>
#include <ArduinoOTA.h>

#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <TimeLib.h>
#include <LittleFS.h>

#include "Colors.h"
#include "Configuration.h"
#include "Event.h"
#include "LedDriver.h"
#include "Modes.h"
#include "Ntp.h"
#include "OpenWeather.h"
#include "Renderer.h"
#include "Settings.h"
#include "Syslog.h"
#include "Timezone.h"
#include "Timezones.h"
#include "Animation.h"
#include "html_content.h"

#ifdef IR_RECEIVER
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#endif

#ifdef RTC_BACKUP
#include <DS3232RTC.h>
#endif

#ifdef SENSOR_DHT22
#include <DHT.h>
#endif

#ifdef SENSOR_BME280
#include <Adafruit_BME280.h>
#endif

#ifdef SENSOR_MCP9808
#include <Adafruit_MCP9808.h>
#endif

#define MILLIS_2_HZ 500

//*****************************************************************************
// Init
//*****************************************************************************
char HostName[32];
char HostNameAp[32];

// Servers
ESP8266WebServer webServer(80);
ESP8266HTTPUpdateServer httpUpdater;

// DHT22
#ifdef SENSOR_DHT22
DHT dht(PIN_DHT22, DHT22);
#endif

// BME280
#ifdef SENSOR_BME280
Adafruit_BME280 bme;
#endif

// MCP9808
#ifdef SENSOR_MCP9808
Adafruit_MCP9808 mcp;
#endif

// IR receiver
#ifdef IR_RECEIVER
IRrecv irrecv(PIN_IR_RECEIVER);
decode_results irDecodeResult;
unsigned long last_ir_millis = 0;
#endif

// Syslog
#ifdef SYSLOGSERVER_SERVER
WiFiUDP wifiUdp;
Syslog syslog(wifiUdp, SYSLOGSERVER_SERVER, SYSLOGSERVER_PORT, HostName, "zytQuadrat", LOG_INFO);
#endif

// RTC
#ifdef RTC_BACKUP
DS3232RTC RTC;
#endif

// LED driver 
LedDriver ledDriver;

// Renderer
Renderer renderer;

// Settings
Settings settings;

// NTP
Ntp ntp;
char ntpServer[] = NTP_DEFAULT_SERVER;
uint8_t errorCounterNTP = 0;

// Screenbuffer
uint16_t matrix[NUMPIXELS_Y] = {};
uint16_t matrixOld[NUMPIXELS_Y] = {};
bool screenBufferNeedsUpdate = true;
bool colorNeedsChange = false;
uint8_t colorOld;

// Mode
Mode mode = MODE_TIME;
Mode lastMode = mode;
uint32_t modeTimeout = 0;
uint32_t autoModeChangeTimer = AUTO_MODECHANGE_TIME;
bool runTransitionOnce = false;
bool runTransitionDemo = false;
uint8_t autoMode = 0;

// Time
uint8_t lastDay = 0;
uint8_t lastMinute = 0;
uint8_t lastHour = 0;
uint8_t lastSecond = 0;
time_t upTime = 0;
uint8_t randomHour = 0;
uint8_t randomMinute = 0;
uint8_t randomSecond = 0;
uint8_t moonphase = 0;

// Feed
String feedText = "";
uint8_t feedColor = WHITE;
uint8_t feedPosition = 0;

// OpenWeather
#ifdef APIKEY
OpenWeather outdoorWeather;
uint8_t errorCounterOutdoorWeather = 0;
#endif

// Temperature Sensor
float roomTemperature = 0;
uint8_t errorCounterMCP = 0;

// Humidity Sensor
float roomHumidity = 0;
uint8_t errorCounterDHT = 0;
uint8_t errorCounterBME = 0;

// Brightness and LDR
uint8_t maxBrightness = map(settings.mySettings.brightness, 0, 100, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
uint8_t brightness = maxBrightness;
#ifdef LDR
uint16_t ldrValue = 0;
uint16_t lastLdrValue = 0;
uint16_t minLdrValue = LDR_MIN_DEFAULT;
uint16_t maxLdrValue = LDR_MAX_DEFAULT;
#endif

// Alarm
#ifdef BUZZER
bool alarmTimerSet = false;
uint8_t alarmTimer = 0;
uint8_t alarmTimerSecond = 0;
uint8_t alarmOn = false;
#endif

// Events
#if defined(SHOW_MODE_SUNRISE_SUNSET) && defined(APIKEY)
bool sunrise_started = false;
unsigned long sunrise_millis = 0;
bool sunset_started = false;
unsigned long sunset_millis = 0;
time_t sunset_unix = 0;
time_t sunrise_unix = 0;
int save_color_sunrise_sunset = 0;
#endif

// Misc
uint8_t testColumn = 0;
int updateInfo = 0;
IPAddress myIP = { 0,0,0,0 };
uint32_t lastButtonPress = 0;
uint32_t lastModePress = 0;
uint8_t modeButtonStage = 0;
bool testFlag = false;
unsigned long mood_millis = 0;
uint32_t lastMillis2Hz = 0;

uint32_t connectMillis = 0;
bool connecting = false;
bool startWps = false;

//Animationen
s_myanimation myanimation;
s_frame copyframe;
uint32_t anipalette[] = { 0xFF0000, 0xFFAA00, 0xFFFF00, 0x00FF00, 0x00FFFF, 0x0000FF, 0xAA00FF, 0xFF00FF, 0x000000, 0xFFFFFF };
String myanimationslist[MAXANIMATION];
uint8_t akt_aniframe = 0;
uint8_t akt_aniloop = 0;
uint8_t frame_fak = 1;        // Animationsrichtung
String animation;
bool playanimation = false;

uint16_t minFreeBlockSize = 10000;
uint16_t codeline = 0;
String codetab;

struct rst_info *rtc_info;

void (*gpio16SoftInterrupt)() = null;
bool gpio16pressed = false;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println("Connected to Wi-Fi sucessfully.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Disconnected from Wi-Fi.");
}

//*****************************************************************************
// Setup()
//*****************************************************************************

void setup()
{
    // init serial port
    Serial.begin(SERIAL_SPEED);
    while (!Serial);
    delay(1000);

    rtc_info = system_get_rst_info();

    // And the monkey flips the switch. (Akiva Goldsman)
    Serial.println();
    Serial.println("*** zytQuadrat ***");
    Serial.println("Firmware: " + String(FIRMWARE_VERSION));
  memset(HostName, 0, sizeof(HostName));
  sprintf(HostName, "%s-%06X", PRODUCT_NAME, ESP.getChipId());
  memcpy(HostNameAp, HostName, sizeof(HostName));
  strcat(HostNameAp, "-AP");

#ifdef POWERON_SELFTEST
    renderer.setAllScreenBuffer(matrix);
    Serial.println("Set all LEDs to red.");
    writeScreenBuffer(matrix, RED, TEST_BRIGHTNESS);
    delay(2500);
    Serial.println("Set all LEDs to green.");
    writeScreenBuffer(matrix, GREEN, TEST_BRIGHTNESS);
    delay(2500);
    Serial.println("Set all LEDs to blue.");
    writeScreenBuffer(matrix, BLUE, TEST_BRIGHTNESS);
    delay(2500);
    Serial.println("Set all LEDs to white.");
    writeScreenBuffer(matrix, WHITE, TEST_BRIGHTNESS);
    delay(2500);
#endif

#ifdef ESP_LED
    Serial.println("Setting up ESP-LED.");
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, HIGH);
#endif

#ifdef MODE_BUTTON
    Serial.println("Setting up Mode-Button.");
    if(PIN_MODE_BUTTON == 16){
      pinMode(PIN_MODE_BUTTON, INPUT_PULLDOWN_16);
      gpio16SoftInterrupt = buttonModeInterrupt;
    } else {
      pinMode(PIN_MODE_BUTTON, INPUT_PULLUP);
      attachInterrupt(digitalPinToInterrupt(PIN_MODE_BUTTON), buttonModeInterrupt, FALLING);
    }
#endif

#ifdef ONOFF_BUTTON
    Serial.println("Setting up Back-Button.");
    pinMode(PIN_ONOFF_BUTTON, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_ONOFF_BUTTON), buttonOnOffInterrupt, FALLING);
#endif

#ifdef TIME_BUTTON
    Serial.println("Setting up Time-Button.");
    pinMode(PIN_TIME_BUTTON, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_TIME_BUTTON), buttonTimeInterrupt, FALLING);
#endif

#ifdef PLUS_BUTTON
    Serial.println("Setting up Plus-Button.");
    pinMode(PIN_PLUS_BUTTON, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_PLUS_BUTTON), buttonPlusInterrupt, FALLING);
#endif

#ifdef MINUS_BUTTON
    Serial.println("Setting up Minus-Button.");
    pinMode(PIN_MINUS_BUTTON, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_MINUS_BUTTON), buttonMinusInterrupt, FALLING);
#endif

#ifdef BUZZER
    Serial.println("Setting up Buzzer.");
    pinMode(PIN_BUZZER, OUTPUT);
#endif

#ifdef SENSOR_DHT22
    Serial.println("Setting up DHT22.");
    dht.begin();
#endif

#ifdef SENSOR_BME280
    Serial.println(F("Setting up BME280."));
    bool bme_status;
    bme_status = bme.begin();
    if (!bme_status) {
      bme.begin(BME280_ADDRESS_ALTERNATE);
      if (!bme_status) {
        Serial.println(F("Could not find a valid BME280 sensor, check wiring, address!"));
      }
    }
#endif

#ifdef SENSOR_MCP9808
    Serial.println(F("Setting up MCP9808."));
    bool mcp_status;
    mcp_status = mcp.begin();
    if (!mcp_status) Serial.println(F("Could not find a valid MCP9808 sensor, check wiring, address!"));
#endif

#ifdef LDR
    Serial.print("Setting up LDR. ABC: ");
    settings.mySettings.useAbc ? Serial.println("enabled") : Serial.println("disabled");
    pinMode(PIN_LDR, INPUT);
#endif

#ifdef IR_RECEIVER
    Serial.println("Setting up IR-Receiver.");
    irrecv.enableIRIn();
#endif

    // Start WiFi and services
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

    renderer.clearScreenBuffer(matrix);
    renderer.setSmallText("WI", TEXT_POS_TOP, matrix);
    renderer.setSmallText("FI", TEXT_POS_BOTTOM, matrix);
    writeScreenBuffer(matrix, WHITE, brightness);

    WiFi.setAutoConnect(true);
  WiFi.hostname(HostName);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  setupWiFi();
  postWiFiSetup();

  Serial.println("Starting webserver.");
  setupWebServer();

  Serial.println("Starting updateserver.");
  httpUpdater.setup(&webServer);
  
#ifdef SYSLOGSERVER_SERVER
    Serial.println("Starting syslog.");
#ifdef APIKEY
    syslog.log(LOG_INFO, ";#;dateTime;roomTemperature;roomHumidity;outdoorTemperature;outdoorHumidity;sunriseTime;sunsetTime;ldrValue;errorCounterNTP;errorCounterDHT;errorCounterMCP;errorCounterBME;errorCounterOutdoorWeather;freeHeapSize;upTime");
#else
    syslog.log(LOG_INFO, ";#;dateTime;roomTemperature;roomHumidity;ldrValue;errorCounterNTP;errorCounterDHT;errorCounterMCP;errorCounterBME;freeHeapSize;upTime");
#endif
#endif

        Serial.println("Starting OTA service.");
#ifdef DEBUG
        ArduinoOTA.onStart([]()
        {
            Serial.println("Start OTA update.");
        });
        ArduinoOTA.onError([](ota_error_t error)
        {
            Serial.println("OTA Error: " + String(error));
            if (error == OTA_AUTH_ERROR) Serial.println("Auth failed.");
            else if (error == OTA_BEGIN_ERROR) Serial.println("Begin failed.");
            else if (error == OTA_CONNECT_ERROR) Serial.println("Connect failed.");
            else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive failed.");
            else if (error == OTA_END_ERROR) Serial.println("End failed.");
        });
        ArduinoOTA.onEnd([]()
        {
            Serial.println("End OTA update.");
        });
#endif
        ArduinoOTA.setPassword(OTA_PASS);
        ArduinoOTA.begin();

#ifdef RTC_BACKUP
    RTC.begin();
    setSyncProvider(RTC.get);
    Serial.print("RTC Sync.");
    if (timeStatus() != timeSet) Serial.print(" FAIL!");
    Serial.println();
#ifdef DEBUG
    time_t tempRtcTime = RTC.get();
    Serial.printf("Time (RTC): %02u:%02u:%02u %02u.%02u.%04u\r\n", hour(tempRtcTime), minute(tempRtcTime), second(tempRtcTime), day(tempRtcTime), month(tempRtcTime), year(tempRtcTime));
#endif
#endif

    // Get the time!
    if (WiFi.isConnected())
    {
        // Set ESP (and RTC) time from NTP
        time_t tempNtpTime = ntp.getTime(ntpServer);
        if (tempNtpTime)
        {
            errorCounterNTP = 0;
            setTime(timeZone.toLocal(tempNtpTime));
#ifdef DEBUG
            Serial.printf("Time (NTP): %02u:%02u:%02u %02u.%02u.%04u (UTC)\r\n", hour(tempNtpTime), minute(tempNtpTime), second(tempNtpTime), day(tempNtpTime), month(tempNtpTime), year(tempNtpTime));
            Serial.printf("Drift (ESP): %d sec.\r\n", tempNtpTime - timeZone.toUTC(now()));
#endif
#ifdef RTC_BACKUP
            RTC.set(timeZone.toLocal(tempNtpTime));
#ifdef DEBUG
            Serial.printf("Drift (RTC): %d sec.\r\n", tempNtpTime - timeZone.toUTC(RTC.get()));
#endif
#endif
        }
        else
        {
            if (errorCounterNTP < 255)
                errorCounterNTP++;
#ifdef DEBUG
            Serial.printf("Error (NTP): %u\r\n", errorCounterNTP);
#endif
        }
    }
    else
    {
    }

    // Define a random time
    randomSeed(analogRead(A0));
    randomHour = random(0, 24);
    randomMinute = random(5, 56);
    randomSecond = random(5, 56);

    // Update room conditions
#if defined(RTC_BACKUP) || defined(SENSOR_DHT22) || defined(SENSOR_MCP9808) || defined(SENSOR_BME280)
    getRoomConditions();
#endif

    // print some infos
#ifdef DEBUG
    Serial.printf("Day on: %02u:%02u:00\r\n", hour(settings.mySettings.dayOnTime), minute(settings.mySettings.dayOnTime));
    Serial.printf("Night off: %02u:%02u:00\r\n", hour(settings.mySettings.nightOffTime), minute(settings.mySettings.nightOffTime));
    Serial.printf("Alarm1: %02u:%02u:00 ", hour(settings.mySettings.alarm1Time), minute(settings.mySettings.alarm1Time));
    settings.mySettings.alarm1 ? Serial.print("on ") : Serial.print("off ");
    Serial.println(settings.mySettings.alarm1Weekdays, BIN);
    Serial.printf("Alarm2: %02u:%02u:00 ", hour(settings.mySettings.alarm2Time), minute(settings.mySettings.alarm2Time));
    settings.mySettings.alarm2 ? Serial.print("on ") : Serial.print("off ");
    Serial.println(settings.mySettings.alarm2Weekdays, BIN);
    Serial.printf("Random time: %02u:%02u:%02u\r\n", randomHour, randomMinute, randomSecond);
    Serial.println("DEBUG is on.");
#else
    Serial.println("DEBUG is off.");
#endif

    lastDay = day() - 1;
    lastHour = hour();
    lastMinute = minute();
    lastSecond = second();

    setupFS();
    // Load Animationsliste
    getAnimationList();
    updateBrightness(true);
} // setup()

//*****************************************************************************
// Loop()
//*****************************************************************************

void loop()
{
    //*************************************************************************
    // Run once a day
    //*************************************************************************

    if (day() != lastDay)
    {
        lastDay = day();
        screenBufferNeedsUpdate = true;

#ifdef SHOW_MODE_MOONPHASE
        moonphase = getMoonphase(year(), month(), day());
#endif

#ifdef DEBUG
        Serial.printf("Uptime: %u days, %02u:%02u\r\n", int(upTime / 86400), hour(upTime), minute(upTime));
        Serial.printf("Free RAM: %u bytes\r\n", system_get_free_heap_size());
        Serial.printf("Moonphase: %u\r\n", moonphase);
#endif

        // Change color
        if (settings.mySettings.colorChange == COLORCHANGE_DAY)
        {
          colorNeedsChange = true;
        }
    }

    //*************************************************************************
    // Run once every hour
    //*************************************************************************

    if (hour() != lastHour)
    {
        lastHour = hour();
        screenBufferNeedsUpdate = true;

        // Change color
        if (settings.mySettings.colorChange == COLORCHANGE_HOUR)
        {
            colorNeedsChange = true;
        }

        // Hourly beep
#ifdef BUZZER
        if ((settings.mySettings.hourBeep == true) && (mode == MODE_TIME))
        {
            digitalWrite(PIN_BUZZER, HIGH);
            delay(25);
            digitalWrite(PIN_BUZZER, LOW);
#ifdef DEBUG
            Serial.println("Beep!");
#endif
        }
#endif

        //*********************************************************************
        // Run once every random hour (once a day)
        //*********************************************************************

        if (hour() == randomHour)
        {
            // Get updateinfo
#ifdef UPDATE_INFOSERVER
            if (WiFi.isConnected())
                getUpdateInfo();
#endif
        }

    }

    //*************************************************************************
    // Run once every minute
    //*************************************************************************

    if (minute() != lastMinute)
    {
        lastMinute = minute();
        screenBufferNeedsUpdate = true;

#if defined(RTC_BACKUP) || defined(SENSOR_DHT22) || defined(SENSOR_MCP9808) || defined(SENSOR_BME280)
        // Update room conditions
        getRoomConditions();
#endif

#ifdef BUZZER
        // Switch on buzzer for alarm 1
        if (settings.mySettings.alarm1 && (hour() == hour(settings.mySettings.alarm1Time)) && (minute() == minute(settings.mySettings.alarm1Time)) && bitRead(settings.mySettings.alarm1Weekdays, weekday()))
        {
            alarmOn = BUZZTIME_ALARM_1;
#ifdef DEBUG
            Serial.println("Alarm1 on.");
#endif
        }

        // Switch on buzzer for alarm 2
        if (settings.mySettings.alarm2 && (hour() == hour(settings.mySettings.alarm2Time)) && (minute() == minute(settings.mySettings.alarm2Time)) && bitRead(settings.mySettings.alarm2Weekdays, weekday()))
        {
            alarmOn = BUZZTIME_ALARM_2;
#ifdef DEBUG
            Serial.println("Alarm2 on.");
#endif
        }
#endif

#ifdef DEBUG
        time_t tempEspTime = now();
        Serial.printf("Time (ESP): %02u:%02u:%02u %02u.%02u.%04u\r\n", hour(tempEspTime), minute(tempEspTime), second(tempEspTime), day(tempEspTime), month(tempEspTime), year(tempEspTime));
#ifdef RTC_BACKUP
        time_t tempRtcTime = RTC.get();
        Serial.printf("Time (RTC): %02u:%02u:%02u %02u.%02u.%04u\r\n", hour(tempRtcTime), minute(tempRtcTime), second(tempRtcTime), day(tempRtcTime), month(tempRtcTime), year(tempRtcTime));
#endif
#endif

        // Set night- and daymode
        if ((hour() == hour(settings.mySettings.nightOffTime)) && (minute() == minute(settings.mySettings.nightOffTime)))
        {
#ifdef DEBUG
            Serial.println("Night off.");
#endif
            setMode(MODE_BLANK);
        }

        if ((hour() == hour(settings.mySettings.dayOnTime)) && (minute() == minute(settings.mySettings.dayOnTime)))
        {
#ifdef DEBUG
            Serial.println("Day on.");
#endif
            setMode(lastMode);

        }

        //*********************************************************************
        // Run once every random minute (once an hour) or if NTP has an error
        //*********************************************************************

        if ((minute() == randomMinute) || ((errorCounterNTP > 0) && (errorCounterNTP < 10)))
        {
            if (WiFi.isConnected())
            {
                // Set ESP (and RTC) time from NTP
                time_t tempNtpTime = ntp.getTime(ntpServer);
                if (tempNtpTime)
                {
                    errorCounterNTP = 0;
                    setTime(timeZone.toLocal(tempNtpTime));
#ifdef DEBUG
                    Serial.printf("Time (NTP): %02u:%02u:%02u %02u.%02u.%04u (UTC)\r\n", hour(tempNtpTime), minute(tempNtpTime), second(tempNtpTime), day(tempNtpTime), month(tempNtpTime), year(tempNtpTime));
                    Serial.printf("Drift (ESP): %d sec.\r\n", tempNtpTime - timeZone.toUTC(now()));
#endif
#ifdef RTC_BACKUP
                    RTC.set(timeZone.toLocal(tempNtpTime));
#ifdef DEBUG
                    Serial.printf("Drift (RTC): %d sec.\r\n", tempNtpTime - timeZone.toUTC(RTC.get()));
#endif
#endif
                }
                else
                {
                    if (errorCounterNTP < 255) errorCounterNTP++;
#ifdef DEBUG
                    Serial.printf("Error (NTP): %u\r\n", errorCounterNTP);
#endif
                }
            }
        }

        if (minute() == randomMinute)
        {
          updateOutdoorWeather();
        }

        //*********************************************************************
        // Run once every 5 minutes
        //*********************************************************************

        if (minute() % 5 == 0)
        {
#ifdef SYSLOGSERVER_SERVER
            // Write some data to syslog
            if (WiFi.isConnected())
            {
                time_t tempEspTime = now();
#ifdef APIKEY
                syslog.log(LOG_INFO, ";D;" + String(tempEspTime) + ";" + String(roomTemperature) + ";" + String(roomHumidity) + ";" + String(outdoorWeather.temperature) + ";" + String(outdoorWeather.humidity) + ";" \
                +String(hour(timeZone.toLocal(outdoorWeather.sunrise))) + ":" + String(minute(timeZone.toLocal(outdoorWeather.sunrise))) + ";" \
                + String(hour(timeZone.toLocal(outdoorWeather.sunset))) + ":" + String(minute(timeZone.toLocal(outdoorWeather.sunset))) + ";" + String(ldrValue)\
                    + ";" + String(errorCounterNTP) + ";" + String(errorCounterDHT) + ";" + String(errorCounterMCP) + ";" + String(errorCounterBME) + ";" + String(errorCounterOutdoorWeather) + ";" + String(ESP.getFreeHeap()) + ";" + String(upTime));
#else
                syslog.log(LOG_INFO, ";D;" + String(tempEspTime) + ";" + String(roomTemperature) + ";" + String(roomHumidity) + ";" + String(ldrValue)\
                    + ";" + String(errorCounterNTP) + ";" + String(errorCounterDHT) + ";" + String(errorCounterMCP) + ";" + String(errorCounterBME) + ";" + String(ESP.getFreeHeap()) + ";" + String(upTime));
#endif
#ifdef DEBUG
                Serial.println("Data written to syslog.");
#endif
            }
#endif
            // Change color
            if (settings.mySettings.colorChange == COLORCHANGE_FIVE)
            {
                colorNeedsChange = true;
            }
        }
    }

    //*************************************************************************
    // Run once every half second
    //*************************************************************************
    
    // millis overflow
    if (lastMillis2Hz > millis()){
      lastMillis2Hz = millis();
    }
  
    // execute every 0.5 second
    if ((lastMillis2Hz + MILLIS_2_HZ) < millis()){
      lastMillis2Hz = millis();
      switch (mode) {
#ifdef LDR
        case MODE_SET_LDR:
#endif      
#ifdef BUZZER
        case MODE_SET_TIMER:
        case MODE_SET_ALARM_1:
        case MODE_SET_ALARM_2:
#endif
        case MODE_SET_COLOR:
        case MODE_SET_COLORCHANGE:
        case MODE_SET_TIMEOUT:
        case MODE_SET_TRANSITION:
        case MODE_SET_TIME:
        case MODE_SET_IT_IS:
        case MODE_SET_GSI:
        case MODE_SET_DAY:
        case MODE_SET_MONTH:
        case MODE_SET_YEAR:
        case MODE_SET_NIGHTOFF:
        case MODE_SET_DAYON:
          screenBufferNeedsUpdate = true;
        break;
          default:
        break;
      }
    }

    //*************************************************************************
    // Run once every second
    //*************************************************************************

    if (second() != lastSecond)
    {
        lastSecond = second();
        upTime++;

#ifdef BUZZER
        // Make some noise
        if (alarmOn)
        {
            alarmOn--;
            digitalRead(PIN_BUZZER) ? digitalWrite(PIN_BUZZER, LOW) : digitalWrite(PIN_BUZZER, HIGH);
            if (!alarmOn)
            {
#ifdef DEBUG
                Serial.println("Alarm: off");
#endif
                digitalWrite(PIN_BUZZER, LOW);
                screenBufferNeedsUpdate = true;
            }
        }
#endif

        if (settings.mySettings.frontCover == FRONTCOVER_BINARY) {
          if (mode != MODE_BLANK)
              screenBufferNeedsUpdate = true;
        } else {
        // General Screenbuffer-Update every second.
        // (not in MODE_TIME or MODE_BLANK because it will lock the ESP due to TRANSITION_FADE)
          if ((mode != MODE_TIME) && (mode != MODE_BLANK))
              screenBufferNeedsUpdate = true;
        }

        // Flash ESP LED
#ifdef ESP_LED
        digitalWrite(PIN_LED, !digitalRead(PIN_LED));
#endif

        // Countdown timeralarm by one minute in the second it was activated
#ifdef BUZZER
        if (alarmTimer && alarmTimerSet && (alarmTimerSecond == second()))
        {
            alarmTimer--;
#ifdef DEBUG
            if (alarmTimer)
                Serial.printf("Timeralarm in %u min.\r\n", alarmTimer);
#endif
        }
        // Switch on buzzer for timer
        if (!alarmTimer && alarmTimerSet)
        {
            alarmTimerSet = false;
            alarmOn = BUZZTIME_TIMER;
#ifdef DEBUG
            Serial.println("Timeralarm: on");
#endif
        }
#endif

        // Auto switch modes
        if (settings.mySettings.modeChange && (mode == MODE_TIME))
        {
            autoModeChangeTimer--;
            if (!autoModeChangeTimer)
            {
#ifdef DEBUG
                Serial.println("Auto modechange. (" + String(autoMode) + ")");
#endif
                autoModeChangeTimer = AUTO_MODECHANGE_TIME;
                switch (autoMode)
                {
                case 0:
#ifdef APIKEY
                    if (String(settings.mySettings.owApiKey) != "") {
                      setMode(MODE_EXT_TEMP);
                    }
#else
#if defined(RTC_BACKUP) || defined(SENSOR_DHT22) || defined(SENSOR_MCP9808) || defined(SENSOR_BME280)
                    setMode(MODE_TEMP);
#endif
#endif
                    autoMode = 1;
                    break;
                case 1:
#if defined(RTC_BACKUP) || defined(SENSOR_DHT22) || defined(SENSOR_MCP9808) || defined(SENSOR_BME280)
                    setMode(MODE_TEMP);
#else
#ifdef APIKEY
                    if (String(settings.mySettings.owApiKey) != "") {
                        setMode(MODE_EXT_TEMP);
                    }
#endif
#endif
                    autoMode = 0;
                    break;
                }
            }
        }

    // Set brightness from LDR and update display.
#ifdef LDR
      if (settings.mySettings.useAbc)
      {
        updateBrightness(false);
      }
#endif

      if (mode == MODE_TIME)
      {
          for (uint8_t i = 0; i < NUM_EVTS; i++)
          {
              uint8_t rep_rate = evtRepRate[settings.mySettings.events[i].repRate];
              if (settings.mySettings.events[i].enabled && (day() == day(settings.mySettings.events[i].time)) && (month() == month(settings.mySettings.events[i].time)) && !(minute() % rep_rate) && (second() < 30))
              {
                  feedText = "  " + String(settings.mySettings.events[i].txt) + "   ";
                  feedPosition = 0;
                  feedColor = settings.mySettings.events[i].color;
#ifdef DEBUG
                  Serial.println("Event Text: \"" + feedText + "\"");
                  Serial.println("Event Animation: " + String(settings.mySettings.events[i].animation));
#endif
                  if (loadAnimation(String(settings.mySettings.events[i].animation)))
                  {
                    akt_aniframe = 0;
                    akt_aniloop = 0;
                    frame_fak = 0;
#ifdef DEBUG
                    Serial.println("Starte Event Animation: " + String(myanimation.name) );
#endif
                    uint32_t animationMillis = millis();
                    while( millis() < (animationMillis + MIN_ANI_DURATION)){ while ( showAnimation(brightness) ) {yield();}; yield();};
                    setMode(MODE_FEED);
                  }
              }
          }
      }
    if (connecting && (WiFi.isConnected() || (millis() > connectMillis + WIFI_CONNECT_TIMEOUT))) {
      connecting = false;
      postWiFiSetup();
    }
    }

    //*************************************************************************
    // Run always
    //*************************************************************************
  if(gpio16pressed && !digitalRead(16)){
    gpio16pressed = false;
    lastModePress = 0;
  }
  if (gpio16SoftInterrupt != null){
    if (digitalRead(16) && !gpio16pressed)
    {
      gpio16pressed = true;
      gpio16SoftInterrupt();
    }
  }

  if (mode == MODE_WPS){
    if (startWps) {
      startWps = false;
      if(!WiFi.isConnected()){
        WiFi.beginWPSConfig();
        connecting = true;
        connectMillis = millis();
      } else {
        postWiFiSetup();
      }
    }
  }
  
    if (mode == MODE_FEED)
        screenBufferNeedsUpdate = true;

    // Call HTTP- and OTA-handle
    webServer.handleClient();
    ArduinoOTA.handle();

#ifdef IR_RECEIVER
    // Look for IR commands
    if (irrecv.decode(&irDecodeResult))
    {
      if ((last_ir_millis + IR_DEBOUNCE_T) < millis()){
        last_ir_millis = millis();
#ifdef DEBUG_IR
        Serial.print("IR signal: 0x");
        serialPrintUint64(irDecodeResult.value, HEX);
        Serial.println();
#endif
      bool codeValid = true;
        switch (irDecodeResult.value)
        {
        case IR_CODE_ONOFF:
            buttonOnOffPressed();
            break;
        case IR_CODE_TIME:
            buttonTimePressed();
            break;
        case IR_CODE_MODE:
            buttonModePressed();
            break;
        case IR_CODE_SETTINGS:
            if ((mode < MODE_SET_1ST) || (mode > MODE_COUNT)) setMode(MODE_SET_1ST);
            else setMode(mode++);
          break;
        case IR_CODE_PLUS:
          buttonPlusPressed();
          break;
        case IR_CODE_MINUS:
          buttonMinusPressed();
          break;
        case IR_CODE_SECONDS:
          setMode(MODE_SECONDS);
          break;
        case IR_CODE_DATE:
          setMode(MODE_DATE);
          break;
        case IR_CODE_RED:
          settings.mySettings.color = RED;
          settings.mySettings.colorChange = COLORCHANGE_NO;
          break;
        case IR_CODE_GREEN:
          settings.mySettings.color = GREEN;
          settings.mySettings.colorChange = COLORCHANGE_NO;
          break;
        case IR_CODE_DARKBLUE:
          settings.mySettings.color = BLUE;
          settings.mySettings.colorChange = COLORCHANGE_NO;
          break;
        case IR_CODE_PINK:
          settings.mySettings.color = PINK;
          settings.mySettings.colorChange = COLORCHANGE_NO;
          break;
        case IR_CODE_WHITE:
          settings.mySettings.color = WHITE;
          settings.mySettings.colorChange = COLORCHANGE_NO;
          break;
        case IR_CODE_BRIGHTBLUE:
          settings.mySettings.color = CYAN;
          settings.mySettings.colorChange = COLORCHANGE_NO;
          break;
        case IR_CODE_YELLOW:
          settings.mySettings.color = YELLOW;
          settings.mySettings.colorChange = COLORCHANGE_NO;
          break;
        case IR_CODE_ORANGE:
          settings.mySettings.color = ORANGE;
          settings.mySettings.colorChange = COLORCHANGE_NO;
          break;
        case IR_CODE_NORMAL:
          settings.mySettings.transition = TRANSITION_NORMAL;
          runTransitionDemo = true;
          break;
        case IR_CODE_FADE:
          settings.mySettings.transition = TRANSITION_FADE;
          runTransitionDemo = true;
          break;
        case IR_CODE_MATRIX:
          settings.mySettings.transition = TRANSITION_MATRIX;
          runTransitionDemo = true;
          break;
        case IR_CODE_SLIDE:
          settings.mySettings.transition = TRANSITION_MOVEUP;
          runTransitionDemo = true;
          break;
        case IR_CODE_MOOD:
          settings.mySettings.colorChange = COLORCHANGE_MOOD;
          settings.mySettings.color = MOOD;
          break;
        case IR_CODE_5MIN:
          settings.mySettings.colorChange = COLORCHANGE_FIVE;
          colorNeedsChange = true;
          break;
        case IR_CODE_1H:
          settings.mySettings.colorChange = COLORCHANGE_HOUR;
          colorNeedsChange = true;
          break;
        case IR_CODE_24H:
          settings.mySettings.colorChange = COLORCHANGE_DAY;
          colorNeedsChange = true;
          break;
        default:
          codeValid = false;
        }
      if (codeValid) {
        settings.saveToEEPROM();
        screenBufferNeedsUpdate = true;
      }
    }
      irrecv.resume();
    }
#endif

#ifdef MODE_BUTTON
#ifdef SHOW_MODE_SETTINGS
  if ((lastModePress != 0) && (!digitalRead(PIN_MODE_BUTTON)^(PIN_MODE_BUTTON == 16)) && (millis() > (lastModePress + 2000)) && modeButtonStage < 1) {
    modeButtonStage = 1;
    if (mode < MODE_SET_1ST) {
      setMode(MODE_SET_1ST);
    } else {
      setMode(MODE_TIME);
    }
  }
#endif
  if ((lastModePress != 0) && (!digitalRead(PIN_MODE_BUTTON)^(PIN_MODE_BUTTON == 16)) && (millis() > (lastModePress + 5000)) && modeButtonStage < 2) {
    modeButtonStage = 2;
    setupWPS();
  }
#endif


    if (settings.mySettings.colorChange == COLORCHANGE_MOOD){
      if((mood_millis + MOOD_INTERVAL_MIN + ((MOOD_INTERVAL_MAX - MOOD_INTERVAL_MIN) * (MOOD_LEVEL_MAX - settings.mySettings.moodRate) / MOOD_LEVEL_MAX)) < millis()){
        mood_millis = millis();
        ledDriver.updateColorWheel();
        screenBufferNeedsUpdate = true;
      }
    }

    // Render a new screenbuffer if needed
    if (screenBufferNeedsUpdate)
    {
        screenBufferNeedsUpdate = false;
        bool goBackToTime = false;

#if defined(SHOW_MODE_SUNRISE_SUNSET) && defined(APIKEY)
        if (sunrise_started && (mode != MODE_SUNRISE)) {
          sunrise_started = false; 
          settings.mySettings.color = save_color_sunrise_sunset;
        }
        if (sunset_started && (mode != MODE_SUNSET)) {
          sunset_started = false;
          settings.mySettings.color = save_color_sunrise_sunset;
        }
#endif
        colorOld = settings.mySettings.color;
        if (colorNeedsChange) {
          updateColor();
          colorNeedsChange = false;
        }

        // Save old screenbuffer (or not if it's the test pattern)
        if (testFlag)
        {
            for (uint8_t i = 0; i <= 9; i++) matrixOld[i] = 0;
            testFlag = false;
        }
        else
            for (uint8_t i = 0; i <= 9; i++) matrixOld[i] = matrix[i];

#ifdef SHOW_MODE_TEST
        if ( mode != MODE_TEST) testColumn = 0;
#endif

        renderer.clearScreenBuffer(matrix);

        switch (mode)
        {
        case MODE_TIME:
            if (settings.mySettings.frontCover == FRONTCOVER_BINARY) {
              matrix[0] = 0b1111000000000000;
              matrix[1] = hour() << 5;
              matrix[2] = minute() << 5;
              matrix[3] = second() << 5;
              matrix[5] = 0b1111000000000000;
              matrix[6] = day() << 5;
              matrix[7] = month() << 5;
              matrix[8] = year() - 2000 << 5;
            } else {
              if (runTransitionDemo) {
                runTransitionOnce = true;
                uint8_t simHour = hour();
                uint8_t simMinute = 0;
                if (minute() < 55) {
                  simMinute = (minute() / 5 + 1) * 5;
                } else {
                  if (hour() < 23){
                    simHour++;
                  } else {
                    simHour = 0;
                  }
                }
                renderer.setTime(simHour, simMinute, settings.mySettings.frontCover, settings.mySettings.chGsi, matrix);
                renderer.setCorners(simMinute, matrix);
                if (settings.mySettings.purist && ((simMinute / 5) % 6)) renderer.clearEntryWords(settings.mySettings.frontCover, matrix);
              }
              else {
                renderer.setTime(hour(), minute(), settings.mySettings.frontCover, settings.mySettings.chGsi, matrix);
                renderer.setCorners(minute(), matrix);
                if (settings.mySettings.purist && ((minute() / 5) % 6)) renderer.clearEntryWords(settings.mySettings.frontCover, matrix);
              }
            }
#ifdef BUZZER
            if (settings.mySettings.alarm1 || settings.mySettings.alarm2 || alarmTimerSet) renderer.setAlarmLed(matrix);
#endif
            break;
#ifdef SHOW_MODE_AMPM
        case MODE_AMPM:
            isAM() ? renderer.setSmallText("AM", TEXT_POS_MIDDLE, matrix) : renderer.setSmallText("PM", TEXT_POS_MIDDLE, matrix);
            break;
#endif
#ifdef SHOW_MODE_SECONDS
        case MODE_SECONDS:
            renderer.setCorners(minute(), matrix);
            for (uint8_t i = 0; i <= 6; i++)
            {
                matrix[1 + i] |= numbersBig[second() / 10][i] << 11;
                matrix[1 + i] |= numbersBig[second() % 10][i] << 5;
            }
            break;
#endif
#ifdef SHOW_MODE_WEEKDAY
        case MODE_WEEKDAY:
            renderer.setSmallText(String(sWeekday[weekday()][0]) + String(sWeekday[weekday()][1]), TEXT_POS_MIDDLE, matrix);
            break;
#endif
#ifdef SHOW_MODE_DATE
        case MODE_DATE:
            if (day() < 10)
                renderer.setSmallText(("0" + String(day())), TEXT_POS_TOP, matrix);
            else
                renderer.setSmallText(String(day()), TEXT_POS_TOP, matrix);
            if (month() < 10)
                renderer.setSmallText(("0" + String(month())), TEXT_POS_BOTTOM, matrix);
            else
                renderer.setSmallText(String(month()), TEXT_POS_BOTTOM, matrix);
            break;
#endif
#if defined(SHOW_MODE_SUNRISE_SUNSET) && defined(APIKEY)
        case MODE_SUNRISE:
            if (!sunrise_started)
            {
                sunrise_unix = timeZone.toLocal(outdoorWeather.sunrise);
                sunrise_started = true;
                sunrise_millis = millis();
                save_color_sunrise_sunset = settings.mySettings.color;
            }
            // if (millis() < sunrise_millis + SUNSET_SUNRISE_SPEED * 0.5 * 0.333)
            if (millis() < sunrise_millis + 1000)
            {
                settings.mySettings.color = YELLOW;
                // Sunrise screen
                matrix[0] = 0b0000000000000000;
                matrix[1] = 0b0000000000000000;
                matrix[2] = 0b0000000000000000;
                matrix[3] = 0b0000000000000000;
                matrix[4] = 0b0000000000000000;
                matrix[5] = 0b0000111000000000;
                matrix[6] = 0b0011111110000000;
                matrix[7] = 0b0111111111000000;
                matrix[8] = 0b0111111111000000;
                matrix[9] = 0b1111111111100000;
            }
            // else if (millis() < sunrise_millis + SUNSET_SUNRISE_SPEED * 0.5 * 0.666)
            else if (millis() < sunrise_millis + 2000)
            {
                // Sunrise screen
                matrix[0] = 0b0000000000000000;
                matrix[1] = 0b0000000000000000;
                matrix[2] = 0b0000111000000000;
                matrix[3] = 0b0011111110000000;
                matrix[4] = 0b0111111111000000;
                matrix[5] = 0b0111111111000000;
                matrix[6] = 0b1111111111100000;
                matrix[7] = 0b1111111111100000;
                matrix[8] = 0b0111111111000000;
                matrix[9] = 0b0111111111000000;
            }
            // else if (millis() < sunrise_millis + SUNSET_SUNRISE_SPEED * 0.5)
            else if (millis() < sunrise_millis + 3000)
            {
                // Sunrise screen
                matrix[0] = 0b0000111000000000;
                matrix[1] = 0b0011111110000000;
                matrix[2] = 0b0111111111000000;
                matrix[3] = 0b0111111111000000;
                matrix[4] = 0b1111111111100000;
                matrix[5] = 0b1111111111100000;
                matrix[6] = 0b0111111111000000;
                matrix[7] = 0b0111111111000000;
                matrix[8] = 0b0011111110000000;
                matrix[9] = 0b0000111000000000;
            }
            //else if (millis() < sunrise_millis + SUNSET_SUNRISE_SPEED * 1.5)
            else if (millis() < sunrise_millis + 4500 + settings.mySettings.timeout * 1000)
            {
                renderer.setTime(hour(sunrise_unix), minute(sunrise_unix), settings.mySettings.frontCover, false, matrix);
                renderer.setCorners(minute(sunrise_unix), matrix);
                renderer.clearEntryWords(settings.mySettings.frontCover, matrix);
            }
            else
            {
                sunrise_started = false;
                settings.mySettings.color = save_color_sunrise_sunset;
                if (settings.mySettings.timeout != 0) goBackToTime = true;
            }
            break;
            
        case MODE_SUNSET:
            if (!sunset_started)
            {
                sunset_unix = timeZone.toLocal(outdoorWeather.sunset);
                sunset_started = true;
                sunset_millis = millis();
            }

            // Matrix is only updatet every second.
            // if (millis() < sunset_millis + SUNSET_SUNRISE_SPEED * 0.5 * 0.333)
            if (millis() < sunset_millis + 1000)
            {
                settings.mySettings.color = ORANGE;
                // Sunset screen
                matrix[0] = 0b0000111000000000;
                matrix[1] = 0b0011111110000000;
                matrix[2] = 0b0111111111000000;
                matrix[3] = 0b0111111111000000;
                matrix[4] = 0b1111111111100000;
                matrix[5] = 0b1111111111100000;
                matrix[6] = 0b0111111111000000;
                matrix[7] = 0b0111111111000000;
                matrix[8] = 0b0011111110000000;
                matrix[9] = 0b0000111000000000;
            }
            // else if (millis() < sunset_millis + SUNSET_SUNRISE_SPEED * 0.5 * 0.666)
            else if (millis() < sunset_millis + 2000)
            {
                // Sunset screen
                matrix[0] = 0b0000000000000000;
                matrix[1] = 0b0000000000000000;
                matrix[2] = 0b0000111000000000;
                matrix[3] = 0b0011111110000000;
                matrix[4] = 0b0111111111000000;
                matrix[5] = 0b0111111111000000;
                matrix[6] = 0b1111111111100000;
                matrix[7] = 0b1111111111100000;
                matrix[8] = 0b0111111111000000;
                matrix[9] = 0b0111111111000000;
            }
            // else if (millis() < sunset_millis + SUNSET_SUNRISE_SPEED * 0.5)
            else if (millis() < sunset_millis + 3000)
            {
                // Sunset screen
                matrix[0] = 0b0000000000000000;
                matrix[1] = 0b0000000000000000;
                matrix[2] = 0b0000000000000000;
                matrix[3] = 0b0000000000000000;
                matrix[4] = 0b0000000000000000;
                matrix[5] = 0b0000111000000000;
                matrix[6] = 0b0011111110000000;
                matrix[7] = 0b0111111111000000;
                matrix[8] = 0b0111111111000000;
                matrix[9] = 0b1111111111100000;
            }
            // else if (millis() < sunset_millis + SUNSET_SUNRISE_SPEED * 1.5)
            else if (millis() < sunset_millis + 4500 + settings.mySettings.timeout * 1000)
            {
                renderer.setTime(hour(sunset_unix), minute(sunset_unix), settings.mySettings.frontCover, false, matrix);
                renderer.setCorners(minute(sunset_unix), matrix);
                renderer.clearEntryWords(settings.mySettings.frontCover, matrix);
            }
            else
            {
                sunset_started = false;
                settings.mySettings.color = save_color_sunrise_sunset;
                if (settings.mySettings.timeout != 0) goBackToTime = true;
            }
            break;
#endif
#ifdef SHOW_MODE_MOONPHASE
        case MODE_MOONPHASE:
            switch (moonphase)
            {
            case 0:
                matrix[0] = 0b0000111000000000;
                matrix[1] = 0b0011000110000000;
                matrix[2] = 0b0100000001000000;
                matrix[3] = 0b0100000001000000;
                matrix[4] = 0b1000000000100000;
                matrix[5] = 0b1000000000100000;
                matrix[6] = 0b0100000001000000;
                matrix[7] = 0b0100000001000000;
                matrix[8] = 0b0011000110000000;
                matrix[9] = 0b0000111000000000;
                break;
            case 1:
                matrix[0] = 0b0000111000000000;
                matrix[1] = 0b0000001110000000;
                matrix[2] = 0b0000000111000000;
                matrix[3] = 0b0000000111000000;
                matrix[4] = 0b0000000111100000;
                matrix[5] = 0b0000000111100000;
                matrix[6] = 0b0000000111000000;
                matrix[7] = 0b0000000111000000;
                matrix[8] = 0b0000001110000000;
                matrix[9] = 0b0000111000000000;
                break;
            case 2:
                matrix[0] = 0b0000011000000000;
                matrix[1] = 0b0000011110000000;
                matrix[2] = 0b0000011111000000;
                matrix[3] = 0b0000011111000000;
                matrix[4] = 0b0000011111100000;
                matrix[5] = 0b0000011111100000;
                matrix[6] = 0b0000011111000000;
                matrix[7] = 0b0000011111000000;
                matrix[8] = 0b0000011110000000;
                matrix[9] = 0b0000011000000000;
                break;
            case 3:
                matrix[0] = 0b0000111000000000;
                matrix[1] = 0b0001111110000000;
                matrix[2] = 0b0001111111000000;
                matrix[3] = 0b0001111111000000;
                matrix[4] = 0b0001111111100000;
                matrix[5] = 0b0001111111100000;
                matrix[6] = 0b0001111111000000;
                matrix[7] = 0b0001111111000000;
                matrix[8] = 0b0001111110000000;
                matrix[9] = 0b0000111000000000;
                break;
            case 4:
                matrix[0] = 0b0000111000000000;
                matrix[1] = 0b0011111110000000;
                matrix[2] = 0b0111111111000000;
                matrix[3] = 0b0111111111000000;
                matrix[4] = 0b1111111111100000;
                matrix[5] = 0b1111111111100000;
                matrix[6] = 0b0111111111000000;
                matrix[7] = 0b0111111111000000;
                matrix[8] = 0b0011111110000000;
                matrix[9] = 0b0000111000000000;
                break;
            case 5:
                matrix[0] = 0b0000111000000000;
                matrix[1] = 0b0011111100000000;
                matrix[2] = 0b0111111100000000;
                matrix[3] = 0b0111111100000000;
                matrix[4] = 0b1111111100000000;
                matrix[5] = 0b1111111100000000;
                matrix[6] = 0b0111111100000000;
                matrix[7] = 0b0111111100000000;
                matrix[8] = 0b0011111100000000;
                matrix[9] = 0b0000111000000000;
                break;
            case 6:
                matrix[0] = 0b0000110000000000;
                matrix[1] = 0b0011110000000000;
                matrix[2] = 0b0111110000000000;
                matrix[3] = 0b0111110000000000;
                matrix[4] = 0b1111110000000000;
                matrix[5] = 0b1111110000000000;
                matrix[6] = 0b0111110000000000;
                matrix[7] = 0b0111110000000000;
                matrix[8] = 0b0011110000000000;
                matrix[9] = 0b0000110000000000;
                break;
            case 7:
                matrix[0] = 0b0000111000000000;
                matrix[1] = 0b0011100000000000;
                matrix[2] = 0b0111000000000000;
                matrix[3] = 0b0111000000000000;
                matrix[4] = 0b1111000000000000;
                matrix[5] = 0b1111000000000000;
                matrix[6] = 0b0111000000000000;
                matrix[7] = 0b0111000000000000;
                matrix[8] = 0b0011100000000000;
                matrix[9] = 0b0000111000000000;
                break;
            }
            break;
#endif
#if defined(RTC_BACKUP) || defined(SENSOR_DHT22) || defined(SENSOR_MCP9808) || defined(SENSOR_BME280)
        case MODE_TEMP:
#ifdef DEBUG
            Serial.println("Room Temperature: " + String(roomTemperature));
#endif
            if (roomTemperature == 0)
            {
                matrix[0] = 0b0000000001000000;
                matrix[1] = 0b0000000010100000;
                matrix[2] = 0b0000000010100000;
                matrix[3] = 0b0000000011100000;
            }
            if (roomTemperature > 0)
            {
                matrix[0] = 0b0000000001000000;
                matrix[1] = 0b0100000010100000;
                matrix[2] = 0b1110000010100000;
                matrix[3] = 0b0100000011100000;
            }
            if (roomTemperature < 0)
            {
                matrix[0] = 0b0000000001000000;
                matrix[1] = 0b0000000010100000;
                matrix[2] = 0b1110000010100000;
                matrix[3] = 0b0000000011100000;
            }
            renderer.setSmallText(String(int(abs(roomTemperature))), TEXT_POS_BOTTOM, matrix);
            break;
#if defined(SENSOR_DHT22) || defined(SENSOR_BME280)
        case MODE_HUMIDITY:
#ifdef DEBUG
            Serial.println("Room Humidity: " + String(roomHumidity));
#endif
            renderer.setSmallText(String(int(roomHumidity + 0.5)), TEXT_POS_TOP, matrix);
            matrix[6] = 0b0100100001000000;
            matrix[7] = 0b0001000010100000;
            matrix[8] = 0b0010000010100000;
            matrix[9] = 0b0100100011100000;
            break;
#endif
#endif
#ifdef APIKEY
        case MODE_EXT_TEMP:
#ifdef DEBUG
            Serial.println("Outdoor temperature: " + String(outdoorWeather.temperature));
#endif
            if (outdoorWeather.temperature > 0)
            {
                matrix[1] = 0b0100000000000000;
                matrix[2] = 0b1110000000000000;
                matrix[3] = 0b0100000000000000;
            }
            if (outdoorWeather.temperature < 0)
                matrix[2] = 0b1110000000000000;
            renderer.setSmallText(String(int(abs(outdoorWeather.temperature) + 0.5)), TEXT_POS_BOTTOM, matrix);
            break;
        case MODE_EXT_HUMIDITY:
#ifdef DEBUG
            Serial.println("Outdoor humidity: " + String(outdoorWeather.humidity));
#endif
            if (outdoorWeather.humidity < 100)
                renderer.setSmallText(String(outdoorWeather.humidity), TEXT_POS_TOP, matrix);
            else
            {
                matrix[0] = 0b0010111011100000;
                matrix[1] = 0b0110101010100000;
                matrix[2] = 0b0010101010100000;
                matrix[3] = 0b0010101010100000;
                matrix[4] = 0b0010111011100000;
            }
            matrix[6] = 0b0100100000000000;
            matrix[7] = 0b0001000000000000;
            matrix[8] = 0b0010000000000000;
            matrix[9] = 0b0100100000000000;
            break;
#endif
#ifdef BUZZER
        case MODE_TIMER:
            renderer.setSmallText("TI", TEXT_POS_TOP, matrix);
            renderer.setSmallText(String(alarmTimer), TEXT_POS_BOTTOM, matrix);
            break;
#endif
#ifdef SHOW_MODE_TEST
        case MODE_TEST:
            if (testColumn == 10)
                testColumn = 0;
            matrix[testColumn] = 0b1111111111110000;
            testColumn++;
            break;
        case MODE_RED:
        case MODE_GREEN:
        case MODE_BLUE:
        case MODE_WHITE:
            renderer.setAllScreenBuffer(matrix);
            testFlag = true;
            break;
#endif
#ifdef SHOW_MODE_SETTINGS
#ifdef LDR
        case MODE_SET_LDR:
          renderer.setSmallText("AB", TEXT_POS_TOP, matrix);
          if ((lastMillis2Hz/MILLIS_2_HZ) % 2 == 0) for (uint8_t i = 5; i <= 9; i++) matrix[i] = 0;
          else
          {
            if (settings.mySettings.useAbc)
            {
              renderer.setSmallText("EN", TEXT_POS_BOTTOM, matrix);
            }
            else
            {
              renderer.setSmallText("DA", TEXT_POS_BOTTOM, matrix);
            }
          }
        break;
#endif
        case MODE_SET_COLOR:
          renderer.setSmallText("CO", TEXT_POS_TOP, matrix);
          if ((lastMillis2Hz/MILLIS_2_HZ) % 2 == 0) for (uint8_t i = 5; i <= 9; i++) matrix[i] = 0;
          else
          {
            renderer.setSmallText(String(settings.mySettings.color), TEXT_POS_BOTTOM, matrix);
          }
        break;  
        case MODE_SET_COLORCHANGE:
          renderer.setSmallText("CC", TEXT_POS_TOP, matrix);
          if ((lastMillis2Hz/MILLIS_2_HZ) % 2 == 0) for (uint8_t i = 5; i <= 9; i++) matrix[i] = 0;
          else {
            switch (settings.mySettings.colorChange) {
              case COLORCHANGE_NO:
                renderer.setSmallText("NO", TEXT_POS_BOTTOM, matrix);
              break;
              case COLORCHANGE_MOOD:
                renderer.setSmallText("MD", TEXT_POS_BOTTOM, matrix);
              break;
              case COLORCHANGE_FIVE:
                renderer.setSmallText("5", TEXT_POS_BOTTOM, matrix);
              break;
              case COLORCHANGE_HOUR:
                renderer.setSmallText("60", TEXT_POS_BOTTOM, matrix);
              break;
              case COLORCHANGE_DAY:
                renderer.setSmallText("24", TEXT_POS_BOTTOM, matrix);
              break;
            }
          }
          break;
        case MODE_SET_TRANSITION:
          renderer.setSmallText("TR", TEXT_POS_TOP, matrix);
          if ((lastMillis2Hz/MILLIS_2_HZ) % 2 == 0) for (uint8_t i = 5; i <= 9; i++) matrix[i] = 0;
          else
          {
            switch (settings.mySettings.transition) {
              case TRANSITION_NORMAL:
                renderer.setSmallText("NO", TEXT_POS_BOTTOM, matrix);
              break;
              case TRANSITION_FADE:
                renderer.setSmallText("FD", TEXT_POS_BOTTOM, matrix);
              break;
              case TRANSITION_MATRIX:
                renderer.setSmallText("MX", TEXT_POS_BOTTOM, matrix);
              break;
              case TRANSITION_MOVEUP:
                renderer.setSmallText("UP", TEXT_POS_BOTTOM, matrix);
              break;
            }
          }
          break;
        case MODE_SET_IT_IS:
          renderer.setSmallText("PT", TEXT_POS_TOP, matrix);
          if ((lastMillis2Hz/MILLIS_2_HZ) % 2 == 0) for (uint8_t i = 5; i <= 9; i++) matrix[i] = 0;
          else
          {
            if (settings.mySettings.purist)
            {
              renderer.setSmallText("EN", TEXT_POS_BOTTOM, matrix);
            }
            else
            {
              renderer.setSmallText("DA", TEXT_POS_BOTTOM, matrix);
            }
          }
          break;
        case MODE_SET_GSI:
          renderer.setSmallText("GS", TEXT_POS_TOP, matrix);
          if ((lastMillis2Hz/MILLIS_2_HZ) % 2 == 0) for (uint8_t i = 5; i <= 9; i++) matrix[i] = 0;
          else
          {
            if (settings.mySettings.chGsi)
            {
              renderer.setSmallText("EN", TEXT_POS_BOTTOM, matrix);
            }
            else
            {
              renderer.setSmallText("DA", TEXT_POS_BOTTOM, matrix);
            }
          }
          
          break;
        case MODE_SET_TIME:
          if (millis() < (modeTimeout + SETTINGS_TITLE_TIMEOUT)){
            renderer.setSmallText("TI", TEXT_POS_TOP, matrix);
            renderer.setSmallText("ME", TEXT_POS_BOTTOM, matrix);
          }
          else if ((lastMillis2Hz/MILLIS_2_HZ) % 2 == 0)
          {
            renderer.setTime(hour(), minute(), settings.mySettings.frontCover, settings.mySettings.chGsi, matrix);
            renderer.setCorners(minute(), matrix);
            renderer.clearEntryWords(settings.mySettings.frontCover, matrix);
            renderer.setAMPM(hour(), settings.mySettings.frontCover, matrix);
          }
          break;
        case MODE_SET_DAY:
          renderer.setSmallText("DD", TEXT_POS_TOP, matrix);
          if ((lastMillis2Hz/MILLIS_2_HZ) % 2 == 0) for (uint8_t i = 5; i <= 9; i++) matrix[i] = 0;
          else
          {
            renderer.setSmallText(String(day()), TEXT_POS_BOTTOM, matrix);
          }
          break;
        case MODE_SET_MONTH:
          renderer.setSmallText("MM", TEXT_POS_TOP, matrix);
          if ((lastMillis2Hz/MILLIS_2_HZ) % 2 == 0) for (uint8_t i = 5; i <= 9; i++) matrix[i] = 0;
          else
          {
            renderer.setSmallText(String(month()), TEXT_POS_BOTTOM, matrix);
          }
          break;
        case MODE_SET_YEAR:
          renderer.setSmallText("YY", TEXT_POS_TOP, matrix);
          if ((lastMillis2Hz/MILLIS_2_HZ) % 2 == 0) for (uint8_t i = 5; i <= 9; i++) matrix[i] = 0;
          else
          {
            renderer.setSmallText(String(year() % 100), TEXT_POS_BOTTOM, matrix);
          }
          break;
        case MODE_SET_NIGHTOFF:
          if (millis() < (modeTimeout + SETTINGS_TITLE_TIMEOUT)){
            renderer.setSmallText("DO", TEXT_POS_TOP, matrix);
            renderer.setSmallText("FF", TEXT_POS_BOTTOM, matrix);
          }
          else if ((lastMillis2Hz/MILLIS_2_HZ) % 2 == 0)
          {
            renderer.setTime(hour(settings.mySettings.nightOffTime), minute(settings.mySettings.nightOffTime), settings.mySettings.frontCover, false, matrix);
            renderer.clearEntryWords(settings.mySettings.frontCover, matrix);
            renderer.setAMPM(hour(settings.mySettings.nightOffTime), settings.mySettings.frontCover, matrix);
          }
          break;
        case MODE_SET_DAYON:
          if (millis() < (modeTimeout + SETTINGS_TITLE_TIMEOUT)){
            renderer.setSmallText("D", TEXT_POS_TOP, matrix);
            renderer.setSmallText("ON", TEXT_POS_BOTTOM, matrix);
          }
          else if ((lastMillis2Hz/MILLIS_2_HZ) % 2 == 0)
          {
            renderer.setTime(hour(settings.mySettings.dayOnTime), minute(settings.mySettings.dayOnTime), settings.mySettings.frontCover, false, matrix);
            renderer.clearEntryWords(settings.mySettings.frontCover, matrix);
            renderer.setAMPM(hour(settings.mySettings.dayOnTime), settings.mySettings.frontCover, matrix);
          }
          break;
        case MODE_SET_TIMEOUT:
          renderer.setSmallText("FB", TEXT_POS_TOP, matrix);
          if ((lastMillis2Hz/MILLIS_2_HZ) % 2 == 0) for (uint8_t i = 5; i <= 9; i++) matrix[i] = 0;
          else
          {
            renderer.setSmallText(String(settings.mySettings.timeout), TEXT_POS_BOTTOM, matrix);
          }
          break;
      case MODE_SET_BRIGHTNESS:
        for (uint8_t x = 0; x < map(settings.mySettings.brightness, 10, 100, 1, 10); x++)
        {
          for (uint8_t y = 0; y <= x; y++)
          {
            matrix[9 - y] |= 1 << (14 - x);
          }
        }
        break;
#ifdef BUZZER
//        case MODE_SET_TIMER:
//        case MODE_SET_ALARM_1:
//        case MODE_SET_ALARM_2:
#endif
#endif
        case MODE_FEED:
            for (uint8_t y = 0; y <= 5; y++)
            {
                renderer.clearScreenBuffer(matrix);
                for (uint8_t z = 0; z <= 6; z++)
                {
                    matrix[2 + z] |= (lettersBig[feedText[feedPosition] - 32][z] << (11 + y)) & 0b1111111111100000;
                    matrix[2 + z] |= (lettersBig[feedText[feedPosition + 1] - 32][z] << (5 + y)) & 0b1111111111100000;
                    matrix[2 + z] |= (lettersBig[feedText[feedPosition + 2] - 32][z] << (y - 1)) & 0b1111111111100000;
                }
                writeScreenBuffer(matrix, feedColor, brightness);
                delay(120);
            }
            feedPosition++;
            if (feedPosition == feedText.length() - 2)
            {
                feedPosition = 0;
                goBackToTime = true;
            }
            break;
      case MODE_WPS:
        renderer.setSmallText("WP", TEXT_POS_TOP, matrix);
        renderer.setSmallText("S", TEXT_POS_BOTTOM, matrix);
        break;
        }

        // turn off LED behind IR-sensor
#ifdef IR_LETTER_OFF
        renderer.unsetPixelInScreenBuffer(IR_LETTER_X, IR_LETTER_Y, matrix);
#endif

        //debugScreenBuffer(matrixOld);
#ifdef DEBUG_MATRIX
        debugScreenBuffer(matrix);
#endif

        // write screenbuffer to display
        switch (mode)
        {
        case MODE_TIME:
        case MODE_BLANK:
            if (settings.mySettings.transition == TRANSITION_NORMAL) {
                writeScreenBuffer(matrix, settings.mySettings.color, brightness);
                if (runTransitionOnce) delay(500);
            }
            else if (settings.mySettings.transition == TRANSITION_FADE)
                writeScreenBufferFade(matrixOld, matrix, settings.mySettings.color, brightness);
            else if (settings.mySettings.transition == TRANSITION_MOVEUP)
            {
                if (((minute() % 5 == 0) && (second() == 0)) || runTransitionOnce)
                    moveScreenBufferUp(matrixOld, matrix, settings.mySettings.color, brightness);
                else
                    writeScreenBuffer(matrix, settings.mySettings.color, brightness);
            }
            else if (settings.mySettings.transition == TRANSITION_MATRIX)
                if (((minute() % 5 == 0) && (second() == 0)) || runTransitionOnce)
                    writeScreenBufferMatrix(matrixOld, matrix, settings.mySettings.color, brightness);
                else
                    writeScreenBuffer(matrix, settings.mySettings.color, brightness);
            break;
#ifdef SHOW_MODE_TEST
        case MODE_RED:
            writeScreenBuffer(matrix, RED, TEST_BRIGHTNESS);
            break;
        case MODE_GREEN:
            writeScreenBuffer(matrix, GREEN, TEST_BRIGHTNESS);
            break;
        case MODE_BLUE:
            writeScreenBuffer(matrix, BLUE, TEST_BRIGHTNESS);
            break;
        case MODE_WHITE:
            writeScreenBuffer(matrix, WHITE, TEST_BRIGHTNESS);
            break;
#endif
        case MODE_FEED:
            writeScreenBuffer(matrix, feedColor, brightness);
            break;
      case MODE_WPS:
        writeScreenBuffer(matrix, LIGHTBLUE, brightness);
        break;
        default:
//            if (runTransitionOnce)
//            {
//                moveScreenBufferUp(matrixOld, matrix, settings.mySettings.color, brightness);
//                testColumn = 0;
//            }
//            else
                writeScreenBuffer(matrix, settings.mySettings.color, brightness);
            break;
        }
        
        if (runTransitionDemo) {
          screenBufferNeedsUpdate = true;
        }
        runTransitionDemo = false;
        runTransitionOnce = false;
        if (goBackToTime) {
          setMode(MODE_TIME);
          goBackToTime = false;
        }
    }

    // Wait for mode timeout then switch back to time
    if ((settings.mySettings.timeout != 0) && (millis() > (modeTimeout + settings.mySettings.timeout * 1000)) && modeTimeout && (mode < MODE_SET_1ST))
    {
        setMode(MODE_TIME);
    }
  
  // Wait for brightness timeout then switch back to time
  if ((millis() > (modeTimeout + BRIGHTNESS_TIMEOUT)) && modeTimeout && (mode == MODE_SET_BRIGHTNESS))
  {
    setMode(MODE_TIME);
  }

#ifdef DEBUG_FPS
    debugFps();
#endif
} // loop()

//*****************************************************************************
// Transitions
//*****************************************************************************

void writeScreenBuffer(uint16_t screenBuffer[], uint8_t color, uint8_t brightness)
{
    ledDriver.clear();
    for (uint8_t y = 0; y < NUMPIXELS_Y; y++)
    {
        for (uint8_t x = 0; x < NUMPIXELS_X; x++)
        {
            if (bitRead(screenBuffer[y], 15 - x))
                ledDriver.setPixel(x, y, color, brightness);
        }
    }

    // Corner LEDs
    for (uint8_t y = 0; y < NUMPIXELS_CORNERS; y++)
    {
        if (bitRead(screenBuffer[y], 4))
            ledDriver.setPixel(PIXEL_NO_CORNER_1 + y, color, brightness);
    }

    // Alarm LED
#ifdef BUZZER
    if (bitRead(screenBuffer[NUMPIXELS_CORNERS], 4))
    {
#ifdef ALARM_LED_COLOR
#ifdef ABUSE_CORNER_LED_FOR_ALARM
        if (settings.mySettings.alarm1 || settings.mySettings.alarm2 || alarmTimerSet)
            ledDriver.setPixel(PIXEL_NO_CORNER_2, ALARM_LED_COLOR, brightness);
        else
            if (bitRead(screenBuffer[1], 4))
                ledDriver.setPixel(PIXEL_NO_CORNER_2, color, brightness);
#else
        ledDriver.setPixel(PIXEL_NO_ALARM, ALARM_LED_COLOR, brightness);
#endif
#else
        ledDriver.setPixel(PIXEL_NO_ALARM, color, brightness);
#endif
    }
#endif

    ledDriver.show();
} // writeScreenBuffer

void moveScreenBufferUp(uint16_t screenBufferOld[], uint16_t screenBufferNew[], uint8_t color, uint8_t brightness)
{
    for (uint8_t z = 0; z < NUMPIXELS_Y; z++)
    {
        for (uint8_t i = 0; i < (NUMPIXELS_Y - 1); i++)
            screenBufferOld[i] = screenBufferOld[i + 1];
        screenBufferOld[NUMPIXELS_Y - 1] = screenBufferNew[z];
        writeScreenBuffer(screenBufferOld, color, brightness);
        webServer.handleClient();
        delay(50);
    }
} // moveScreenBufferUp

void writeScreenBufferFade(uint16_t screenBufferOld[], uint16_t screenBufferNew[], uint8_t color, uint8_t brightness)
{
    ledDriver.clear();
    uint8_t brightnessBuffer[NUMPIXELS_Y][NUMPIXELS_X+1] = {};

    // Copy old matrix to buffer
    for (uint8_t y = 0; y < NUMPIXELS_Y; y++)
    {
        for (uint8_t x = 0; x < (NUMPIXELS_X + 1); x++)
        {
            if (bitRead(screenBufferOld[y], 15 - x))
                brightnessBuffer[y][x] = brightness;
        }
    }

    // Fade old to new matrix
    for (uint8_t i = 0; i < brightness; i++)
    {
        for (uint8_t y = 0; y < NUMPIXELS_Y; y++)
        {
            for (uint8_t x = 0; x < (NUMPIXELS_X + 1); x++)
            {
                ESP.wdtFeed();
                if (!(bitRead(screenBufferOld[y], 15 - x)) && (bitRead(screenBufferNew[y], 15 - x)))
                    brightnessBuffer[y][x]++;
                if ((bitRead(screenBufferOld[y], 15 - x)) && !(bitRead(screenBufferNew[y], 15 - x)))
                    brightnessBuffer[y][x]--;
                ledDriver.setPixel(x, y, color, brightnessBuffer[y][x]);
            }
        }

        // Corner LEDs
        for (uint8_t y = 0; y < NUMPIXELS_CORNERS; y++)
            ledDriver.setPixel(PIXEL_NO_CORNER_1 + y, color, brightnessBuffer[y][11]);

        // Alarm LED
#ifdef BUZZER
#ifdef ALARM_LED_COLOR
#ifdef ABUSE_CORNER_LED_FOR_ALARM
        if (settings.mySettings.alarm1 || settings.mySettings.alarm2 || alarmTimerSet)
            ledDriver.setPixel(PIXEL_NO_CORNER_2, ALARM_LED_COLOR, brightnessBuffer[4][11]);
        else
            ledDriver.setPixel(PIXEL_NO_CORNER_2, color, brightnessBuffer[1][11]);
#else
        ledDriver.setPixel(PIXEL_NO_ALARM, ALARM_LED_COLOR, brightnessBuffer[4][11]);
#endif
#else
        ledDriver.setPixel(PIXEL_NO_ALARM, color, brightnessBuffer[4][11]);
#endif
#endif
        webServer.handleClient();
        ledDriver.show();
    }
} // writeScreenBufferFade

void writeScreenBufferMatrix(uint16_t screenBufferOld[], uint16_t screenBufferNew[], uint8_t color, uint8_t brightness)
{
  uint16_t mline[NUMPIXELS_X] = {0};
  uint16_t wline[NUMPIXELS_X] = {0};
  uint16_t sline[NUMPIXELS_X] = {0};
  uint8_t aktline;
  uint16_t mleer = 0;  // zu prüfen ob wir fertig sind

  uint8_t mstep = 0;
  uint16_t mline_max[NUMPIXELS_X] = {0};
  uint16_t brightnessBuffer[NUMPIXELS_Y][NUMPIXELS_X] = {0};
  uint16_t brightness_16 = brightness;

  uint16_t zufallszeile[NUMPIXELS_X];
  uint16_t zufallsbuffer;
  uint16_t zufallsindex;

  for (uint8_t i = 0; i < NUMPIXELS_X; i++) zufallszeile[i] = i;
  
  // Zufälliges Vertauschen von jeweils 2 Zeilenwerten:
  for (uint8_t i = 0; i < NUMPIXELS_X; i++)
  {
    zufallsindex = random(0, NUMPIXELS_X);
    zufallsbuffer = zufallszeile[zufallsindex];
    zufallszeile[zufallsindex] = zufallszeile[i];
    zufallszeile[i] = zufallsbuffer;

  }
  // Variablen init
  for (uint8_t line = 0; line < NUMPIXELS_X; line++)
  {
    sline[line] = (NUMPIXELS_X - (zufallszeile[line] % NUMPIXELS_X )) * 5 / 2;
    mline_max[line] = 0;
    mline[line] = 0;
    wline[line] = 0;
  }

  for (uint16_t i = 0; i <= 1200; i++)
  {
    aktline = zufallszeile[i % NUMPIXELS_X];

    if ( sline[aktline] > 0 ) sline[aktline]--;
    if ( sline[aktline] == 0 )
    {
      sline[aktline] = 3 - (aktline % 2);

      if ( mline[aktline] == 0 && mline_max[aktline] < NUMPIXELS_Y)
      {
        mline[aktline] = 1;
        mline_max[aktline]++;
      }
      else
      {
        if ( mline_max[aktline] < NUMPIXELS_X )  // solange grün hinzu bis unten erreicht ist
        {
          if ( random(0, 6) == 0 && (mline[aktline] & 1) == 0 )
          {
            mline[aktline] = mline[aktline] << 1;
            mline[aktline] = mline[aktline] | 1;
            wline[aktline] = wline[aktline] << 1;
          }
          else
          {
            wline[aktline] = wline[aktline] << 1;
            wline[aktline] = wline[aktline] | 1;
            mline[aktline] = mline[aktline] << 1;
          }
          mline_max[aktline]++;
        }
        else
        {
          mline[aktline] = mline[aktline] << 1;
          wline[aktline] = wline[aktline] << 1;
          if ( (mline[aktline] & 0x3FF)  == 0 && (wline[aktline] & 0x3FF) == 0 ) mleer = mleer | 1 << aktline;
        }
      }
      ledDriver.clear();
      for ( uint16_t y = 0; y < NUMPIXELS_Y; y++ )
      {
        for ( uint16_t x = 0; x < NUMPIXELS_X; x++ )
        {
          if ( y > mline_max[x] - 1 )
          {
            if (bitRead(screenBufferOld[y], 15 - x)) ledDriver.setPixel(x, y, colorOld, brightness);
          }
          else
          {
            if (bitRead(screenBufferNew[y], 15 - x)) ledDriver.setPixel(x, y, color, brightness);
          }
          brightnessBuffer[y][x] = 0;
          if ( wline[x] & (1 << y) )
          {
            brightnessBuffer[y][x] = brightness_16 / 9;
            if ( brightnessBuffer[y][x] < MIN_BRIGHTNESS) brightnessBuffer[y][x] = MIN_BRIGHTNESS;
          }
          if ( mline[x] & (1 << y) )
          {
            brightnessBuffer[y][x] =  brightness_16 * 10 / 9;
            if ( brightnessBuffer[y][x] > MAX_BRIGHTNESS ) brightnessBuffer[y][x] = MAX_BRIGHTNESS;
          }

          if ( brightnessBuffer[y][x] > 0 )
          {
            ledDriver.setPixel(x, y, GREEN, brightnessBuffer[y][x]);
          }
        } // x
      }  // y
      ledDriver.show();
      delay (15);
    }
    webServer.handleClient();
    if ( i > 100 && mleer == 0x7FF) break;
  }

  writeScreenBuffer(screenBufferNew, color, brightness);
} // writeScreenBufferMatrix

//

//*****************************************************************************
// colorUpdate
//*****************************************************************************
void updateColor()
{
  settings.mySettings.color = random(0, COLOR_COUNT);
#ifdef DEBUG
  Serial.printf("Color changed to: %u\r\n", settings.mySettings.color);
#endif
}

//*****************************************************************************
// "On/off" pressed
//*****************************************************************************

void buttonOnOffPressed()
{
#ifdef DEBUG
    Serial.println("On/off pressed.");
#endif
    mode == MODE_BLANK ? setLedsOn() : setLedsOff();
}

//*****************************************************************************
// "Time" pressed
//*****************************************************************************

void buttonTimePressed()
{
#ifdef DEBUG
    Serial.println("Time pressed.");
#endif

    // Switch off alarm
#ifdef BUZZER
    if (alarmOn)
    {
#ifdef DEBUG
        Serial.println("Alarm: off");
#endif
        digitalWrite(PIN_BUZZER, LOW);
        alarmOn = false;
    }
#endif

    modeTimeout = 0;
    setMode(MODE_TIME);
}

//*****************************************************************************
// "Mode" pressed
//*****************************************************************************

void buttonModePressed()
{
#ifdef DEBUG
    Serial.println("Mode pressed.");
#endif

    // Switch off alarm
#ifdef BUZZER
    if (alarmOn)
    {
#ifdef DEBUG
        Serial.println("Alarm: off");
#endif
        digitalWrite(PIN_BUZZER, LOW);
        alarmOn = false;
        setMode(MODE_TIME);
        return;
    }
#endif
    if((mode == MODE_SET_1ST - 1) || (mode > MODE_COUNT)) {
      setMode(MODE_TIME);
    } else {
      setMode(mode++);
    }
}

//*****************************************************************************
// "Plus" pressed
//*****************************************************************************

void buttonPlusPressed()
{
#ifdef DEBUG
    Serial.println("Minus pressed.");
#endif
  switch (mode) {
#ifdef SHOW_MODE_SETTINGS
#ifdef LDR
  case MODE_SET_LDR:
    settings.mySettings.useAbc = !settings.mySettings.useAbc;
    updateBrightness(true);
  break;
#endif
  case MODE_SET_COLOR:
    if (settings.mySettings.color < COLOR_COUNT) settings.mySettings.color++;
    else settings.mySettings.color = 0;
    if (settings.mySettings.colorChange == COLORCHANGE_MOOD) settings.mySettings.colorChange = COLORCHANGE_NO;
  break;
  case MODE_SET_COLORCHANGE:
    if (settings.mySettings.colorChange < COLORCHANGE_COUNT) settings.mySettings.colorChange++;
    else settings.mySettings.colorChange = 0;
    if (settings.mySettings.colorChange == COLORCHANGE_MOOD) settings.mySettings.color = MOOD;
    if (settings.mySettings.colorChange == COLORCHANGE_NO) settings.mySettings.color = WHITE;
  break;
  case MODE_SET_TRANSITION:
    if (settings.mySettings.transition < TRANSITION_COUNT) settings.mySettings.transition++;
    else settings.mySettings.transition = 0;
  break;
  case MODE_SET_IT_IS:
    settings.mySettings.purist = !settings.mySettings.purist;
  break;
  case MODE_SET_GSI:
    settings.mySettings.chGsi = !settings.mySettings.chGsi;
  break;
  case MODE_SET_TIME:
    setTime(hour() + 1, minute(), second(), day(), month(), year());
  break;
  case MODE_SET_DAY:
    setTime(hour(), minute(), second(), day() + 1, month(), year());
  break;
  case MODE_SET_MONTH:
    setTime(hour(), minute(), second(), day(), month() + 1, year());
  break;
  case MODE_SET_YEAR:
    setTime(hour(), minute(), second(), day(), month(), year() + 1);
  break;
  case MODE_SET_NIGHTOFF:
    settings.mySettings.nightOffTime += 3600;
  break;
  case MODE_SET_DAYON:
    settings.mySettings.dayOnTime += 3600;
  break;
  case MODE_SET_TIMEOUT:
    if (settings.mySettings.timeout < 60) {
      settings.mySettings.timeout += 5;
    }
  break;
  default:
    if(settings.mySettings.brightness <= 90){
      settings.mySettings.brightness += 10;
    }
      setMode(MODE_SET_BRIGHTNESS);
    updateBrightness(true);
#endif
  }
  settings.saveToEEPROM();
}

//*****************************************************************************
// "Minus" pressed
//*****************************************************************************

void buttonMinusPressed()
{
#ifdef DEBUG
    Serial.println("Minus pressed.");
#endif
  switch (mode) {
#ifdef SHOW_MODE_SETTINGS
#ifdef LDR
  case MODE_SET_LDR:
    settings.mySettings.useAbc = !settings.mySettings.useAbc;
    updateBrightness(true);
  break;
#endif
  case MODE_SET_COLOR:
    if (settings.mySettings.color > 0) settings.mySettings.color--;
    else settings.mySettings.color = COLOR_COUNT;
    if (settings.mySettings.colorChange == COLORCHANGE_MOOD) settings.mySettings.colorChange = COLORCHANGE_NO;
  break;
  case MODE_SET_COLORCHANGE:
    if (settings.mySettings.colorChange > 0) settings.mySettings.colorChange--;
    else settings.mySettings.colorChange = COLORCHANGE_COUNT;
    if (settings.mySettings.colorChange == COLORCHANGE_MOOD) settings.mySettings.color = MOOD;
    if (settings.mySettings.colorChange == COLORCHANGE_NO) settings.mySettings.color = WHITE;
  break;
  case MODE_SET_TRANSITION:
    if (settings.mySettings.transition > 0) settings.mySettings.transition--;
    else settings.mySettings.transition = TRANSITION_COUNT;
  break;
  case MODE_SET_IT_IS:
    settings.mySettings.purist = !settings.mySettings.purist;
  break;
  case MODE_SET_GSI:
    settings.mySettings.chGsi = !settings.mySettings.chGsi;
  break;
  case MODE_SET_TIME:
    setTime(hour(), minute() + 1, second(), day(), month(), year());
  break;
  case MODE_SET_DAY:
    setTime(hour(), minute(), second(), day() - 1, month(), year());
  break;
  case MODE_SET_MONTH:
    setTime(hour(), minute(), second(), day(), month() - 1, year());
  break;
  case MODE_SET_YEAR:
    setTime(hour(), minute(), second(), day(), month(), year() - 1);
  break;
  case MODE_SET_NIGHTOFF:
    settings.mySettings.nightOffTime += 300;
  break;
  case MODE_SET_DAYON:
    settings.mySettings.dayOnTime += 300;
  break;
  case MODE_SET_TIMEOUT:
    if (settings.mySettings.timeout > 0) {
      settings.mySettings.timeout -= 5;
    }
  break;
  default:
    if(settings.mySettings.brightness >= 20){
      settings.mySettings.brightness -= 10;
    }
      setMode(MODE_SET_BRIGHTNESS);
    updateBrightness(true);
#endif
  }
  settings.saveToEEPROM();
}

//*****************************************************************************
// Set mode
//*****************************************************************************

void setMode(Mode newMode)
{
    screenBufferNeedsUpdate = true;
    runTransitionOnce = true;
    lastMode = mode;
    mode = newMode;

  if (mode != MODE_WPS) {
    if (connecting){
      connecting = false;
      postWiFiSetup();
    }
  }

    // set timeout for selected mode
    switch (mode)
    {
#ifdef SHOW_MODE_AMPM
    case MODE_AMPM:
#endif
#ifdef SHOW_MODE_SECONDS
    case MODE_SECONDS:
#endif
#ifdef SHOW_MODE_WEEKDAY
    case MODE_WEEKDAY:
#endif
#ifdef SHOW_MODE_DATE
    case MODE_DATE:
#endif
// #if defined(SHOW_MODE_SUNRISE_SUNSET) && defined(APIKEY)
//     case MODE_SUNRISE:
//     case MODE_SUNSET:
// #endif
#ifdef SHOW_MODE_MOONPHASE
    case MODE_MOONPHASE:
#endif
#if defined(RTC_BACKUP) || defined(SENSOR_DHT22) || defined(SENSOR_MCP9808) || defined(SENSOR_BME280)
    case MODE_TEMP:
#endif
#if defined(SENSOR_DHT22) || defined(SENSOR_BME280)
    case MODE_HUMIDITY:
#endif
#ifdef APIKEY
    case MODE_EXT_TEMP:
    case MODE_EXT_HUMIDITY:
#endif
#ifdef SHOW_MODE_SETTINGS
    case MODE_SET_TIME:
    case MODE_SET_NIGHTOFF:
    case MODE_SET_DAYON:
    case MODE_SET_BRIGHTNESS:
#endif
        modeTimeout = millis();
        break;
    default:
        modeTimeout = 0;
        break;
    }

    switch (mode) {
#ifdef APIKEY
#ifdef SHOW_MODE_SUNRISE_SUNSET
    case MODE_SUNRISE:
    case MODE_SUNSET:
#endif
    case MODE_EXT_TEMP:
    case MODE_EXT_HUMIDITY:
      if (String(settings.mySettings.owApiKey) == "") {
        buttonModePressed();
      }
    break;
#endif
    case MODE_SET_GSI:
      if((settings.mySettings.frontCover != FRONTCOVER_CH_BE) && (settings.mySettings.frontCover != FRONTCOVER_CH_ZH) && (settings.mySettings.frontCover != FRONTCOVER_CH_AG) && (settings.mySettings.frontCover != FRONTCOVER_CH_GR)) {
        buttonModePressed();
      }
      break;
    default:
      break;
    }
}

void updateBrightness(bool forcedUpdate) {
  uint8_t oldBrightness = brightness;
  maxBrightness = map(settings.mySettings.brightness, 10, 100, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
#ifdef LDR
  if (settings.mySettings.useAbc) {
    brightness = getBrightnessFromLDR(forcedUpdate);
  } else {
    brightness = maxBrightness;
  }
#else
  brightness = maxBrightness;
#endif
  if (oldBrightness != brightness){
    screenBufferNeedsUpdate = true;
  }
}
//*****************************************************************************
// Get brightness from LDR
//*****************************************************************************

#ifdef LDR
uint8_t getBrightnessFromLDR(bool forcedUpdate)
{
    uint32_t ldrLetterColor = ledDriver.getPixelColor(settings.mySettings.ldrPosX, settings.mySettings.ldrPosY);
    if (ldrLetterColor != 0){
      return brightness;
    }
#ifdef LDR_IS_INVERSE
    ldrValue = 1024 - analogRead(PIN_LDR);
#else
    ldrValue = analogRead(PIN_LDR);
#endif

#ifdef DEBUG_IR
    Serial.println("LDR value: " + String(ldrValue));
    Serial.println("LDR min: " + String(minLdrValue));
    Serial.println("LDR max: " + String(maxLdrValue));
#endif

    if (ldrValue < minLdrValue)
        minLdrValue = ldrValue;
    if (ldrValue > maxLdrValue)
        maxLdrValue = ldrValue;
    if ((ldrValue >= (lastLdrValue + LDR_HYSTERESIS)) || (ldrValue <= (lastLdrValue - LDR_HYSTERESIS)) || forcedUpdate)
    {
        lastLdrValue = ldrValue;
        return (uint8_t)map(ldrValue, minLdrValue, maxLdrValue, MIN_BRIGHTNESS, maxBrightness);
    }
    return brightness;
}
#endif

//*****************************************************************************
// Get update info from server
//*****************************************************************************

#ifdef UPDATE_INFOSERVER
void getUpdateInfo()
{
    WiFiClient wifiClient;
    HttpClient httpClient = HttpClient(wifiClient, UPDATE_INFOSERVER, 80);
    httpClient.get(UPDATE_INFOFILE);
    if (httpClient.responseStatusCode() == 200)
    {
        String response = httpClient.responseBody();
        response.trim();
        JSONVar updateArray = JSON.parse(response);
        if (JSON.typeof(updateArray) == "undefined")
        {
            #ifdef DEBUG
            Serial.println("Parsing updateArray failed!");
            #endif
            return;
        }
        updateInfo = (int)updateArray["channel"]["unstable"]["version"];
    }
#ifdef DEBUG
    updateInfo > int(FIRMWARE_VERSION) ? Serial.println("Firmwareupdate available! (" + String(updateInfo) + ")") : Serial.println("Firmware is uptodate.");
#endif
}
#endif

//*****************************************************************************
// Get room conditions
//*****************************************************************************

#if defined(RTC_BACKUP) || defined(SENSOR_DHT22) || defined(SENSOR_MCP9808) || defined(SENSOR_BME280)
void getRoomConditions()
{
#if defined(RTC_BACKUP)// && !defined(SENSOR_DHT22) && !defined(SENSOR_MCP9808)
    roomTemperature = RTC.temperature() / 4.0 + RTC_TEMPERATURE_OFFSET;
#ifdef DEBUG
    Serial.println("Temperature (RTC): " + String(roomTemperature) + "C");
#endif
#endif
#if defined(SENSOR_MCP9808)// && !defined(SENSOR_DHT22)
    float mcpTemperature = mcp.readTempC();
    if (!isnan(mcpTemperature)){
      errorCounterMCP = 0;
      roomTemperature = mcpTemperature + MCP_TEMPERATURE_OFFSET;
#ifdef DEBUG
      Serial.println("Temperature (MCP): " + String(roomTemperature) + "C");
#endif
    }
    else
    {
        if (errorCounterMCP < 255)
            errorCounterMCP++;
#ifdef DEBUG
        Serial.printf("Error (MCP): %u\r\n", errorCounterMCP);
#endif
    }
#endif
#ifdef SENSOR_DHT22
    float dhtTemperature = dht.readTemperature();
    float dhtHumidity = dht.readHumidity();
    if (!isnan(dhtTemperature) && !isnan(dhtHumidity))
    {
        errorCounterDHT = 0;
        roomTemperature = dhtTemperature + DHT_TEMPERATURE_OFFSET;
        roomHumidity = dhtHumidity + DHT_HUMIDITY_OFFSET;
#ifdef DEBUG
        Serial.println("Temperature (DHT): " + String(roomTemperature) + "C");
        Serial.println("Humidity (DHT): " + String(roomHumidity) + "%");
#endif
    }
    else
    {
        if (errorCounterDHT < 255)
            errorCounterDHT++;
#ifdef DEBUG
        Serial.printf("Error (DHT): %u\r\n", errorCounterDHT);
#endif
    }
#endif
#ifdef SENSOR_BME280
    float bmeTemperature = bme.readTemperature();
    float bmeHumidity = bme.readHumidity();
    if (!isnan(bmeTemperature) && !isnan(bmeHumidity))
    {
        errorCounterBME = 0;
        roomTemperature = bmeTemperature + BME_TEMPERATURE_OFFSET;
        roomHumidity = bmeHumidity + BME_HUMIDITY_OFFSET;
#ifdef DEBUG
        Serial.println("Temperature (BME): " + String(roomTemperature) + "C");
        Serial.println("Humidity (BME): " + String(roomHumidity) + "%");
#endif
    }
    else
    {
        if (errorCounterBME < 255)
            errorCounterBME++;
#ifdef DEBUG
        Serial.printf("Error (BME): %u\r\n", errorCounterBME);
#endif
    }
#endif
}
#endif

//*****************************************************************************
// Misc
//*****************************************************************************

#ifdef MODE_BUTTON
ICACHE_RAM_ATTR void buttonModeInterrupt()
{
    if (millis() > lastButtonPress + 250)
    {
        lastButtonPress = millis();
        lastModePress = lastButtonPress;
    modeButtonStage = 0;
        buttonModePressed();
    }
}
#endif

#ifdef ONOFF_BUTTON
ICACHE_RAM_ATTR void buttonOnOffInterrupt()
{
    if (millis() > lastButtonPress + 250)
    {
        lastButtonPress = millis();
        buttonOnOffPressed();
    }
}
#endif

#ifdef TIME_BUTTON
ICACHE_RAM_ATTR void buttonTimeInterrupt()
{
    if (millis() > lastButtonPress + 250)
    {
        lastButtonPress = millis();
        buttonTimePressed();
    }
}
#endif

#ifdef PLUS_BUTTON
ICACHE_RAM_ATTR void buttonPlusInterrupt()
{
    if (millis() > lastButtonPress + 250)
    {
        lastButtonPress = millis();
        buttonPlusPressed();
    }
}
#endif

#ifdef MINUS_BUTTON
ICACHE_RAM_ATTR void buttonMinusInterrupt()
{
    if (millis() > lastButtonPress + 250)
    {
        lastButtonPress = millis();
        buttonMinusPressed();
    }
}
#endif

// Switch off LEDs
void setLedsOff()
{
#ifdef DEBUG
    Serial.println("LEDs: off");
#endif
    setMode(MODE_BLANK);
}

// Switch on LEDs
void setLedsOn()
{
#ifdef DEBUG
    Serial.println("LEDs: on");
#endif
    setMode(lastMode);
}

// Calculate moonphase
#ifdef SHOW_MODE_MOONPHASE
int getMoonphase(int y, int m, int d)
{
    int b;
    int c;
    int e;
    double jd;
    if (m < 3) {
        y--;
        m += 12;
    }
    ++m;
    c = 365.25 * y;
    e = 30.6 * m;
    jd = c + e + d - 694039.09; // jd is total days elapsed
    jd /= 29.53;                // divide by the moon cycle (29.53 days)
    b = jd;                     // int(jd) -> b, take integer part of jd
    jd -= b;                    // subtract integer part to leave fractional part of original jd
    b = jd * 8 + 0.5;           // scale fraction from 0-8 and round by adding 0.5
    b = b & 7;                  // 0 and 8 are the same so turn 8 into 0
    return b;
}
#endif

// Write screenbuffer to console
#ifdef DEBUG_MATRIX
void debugScreenBuffer(uint16_t screenBuffer[])
{
    const char buchstabensalat[][12] =
    {
        { 'E', 'S', 'K', 'I', 'S', 'T', 'A', 'F', 'U', 'N', 'F', '1' },
        { 'Z', 'E', 'H', 'N', 'Z', 'W', 'A', 'N', 'Z', 'I', 'G', '2' },
        { 'D', 'R', 'E', 'I', 'V', 'I', 'E', 'R', 'T', 'E', 'L', '3' },
        { 'V', 'O', 'R', 'F', 'U', 'N', 'K', 'N', 'A', 'C', 'H', '4' },
        { 'H', 'A', 'L', 'B', 'A', 'E', 'L', 'F', 'U', 'N', 'F', 'A' },
        { 'E', 'I', 'N', 'S', 'X', 'A', 'M', 'Z', 'W', 'E', 'I', ' ' },
        { 'D', 'R', 'E', 'I', 'P', 'M', 'J', 'V', 'I', 'E', 'R', ' ' },
        { 'S', 'E', 'C', 'H', 'S', 'N', 'L', 'A', 'C', 'H', 'T', ' ' },
        { 'S', 'I', 'E', 'B', 'E', 'N', 'Z', 'W', 'O', 'L', 'F', ' ' },
        { 'Z', 'E', 'H', 'N', 'E', 'U', 'N', 'K', 'U', 'H', 'R', ' ' }
    };
    //Serial.println("\033[0;0H"); // set cursor to 0, 0 position
    Serial.println(" -----------");
    for (uint8_t y = 0; y <= 9; y++)
    {
        Serial.print('|');
        for (uint8_t x = 0; x <= 10; x++)
            Serial.print((bitRead(screenBuffer[y], 15 - x) ? buchstabensalat[y][x] : ' '));
        Serial.print('|');
        Serial.println((bitRead(screenBuffer[y], 4) ? buchstabensalat[y][11] : ' '));
    }
    Serial.println(" -----------");
}
#endif

// Write FPS to console
#ifdef DEBUG_FPS
void debugFps()
{
    static uint16_t frames;
    static uint32_t lastFpsCheck;
    frames++;
    //if ((millis() % 1000 == 0) && (millis() != lastFpsCheck))
    if (millis() % 1000 == 0)
    {
        //lastFpsCheck = millis();
        Serial.printf("FPS: %u\r\n", frames);
        frames = 0;
    }
}
#endif

//*****************************************************************************
// Webserver
//*****************************************************************************

void setupWebServer()
{
    webServer.onNotFound(handleNotFound);
    webServer.on("/", handleRoot);
    webServer.on("/handleButtonOnOff", []() { buttonOnOffPressed(); callRoot(); });
    webServer.on("/handleButtonSettings", handleButtonSettings);
    webServer.on("/handleButtonEvents", handleButtonEvents);
    webServer.on("/handleButtonMode", []() { buttonModePressed(); callRoot(); });
    webServer.on("/handleButtonTime", []() { buttonTimePressed(); callRoot(); });
    webServer.on("/commitSettings", []() { handleCommitSettings(); callRoot(); });
    webServer.on("/commitAdminSettings", []() { handleCommitAdminSettings(); callRoot(); });
    webServer.on("/commitEvents", []() { handleCommitEvents(); handleButtonSettings(); });
    webServer.on("/admin", handleAdmin);
    webServer.on("/reset", []() { handleReset(); callRoot(); });
    webServer.on("/wifiReset", handleWiFiReset);
    webServer.on("/factoryReset", handleFactoryReset);
    webServer.on("/settingsReset", handleSettingsReset);
    webServer.on("/showText", handleShowText);
    webServer.on("/control", handleControl);
    webServer.begin();
}

void callRoot()
{
    webServer.send(200, "text/html", "<!doctype html><html><head><script>window.onload=function(){window.location.replace('/');}</script></head></html>");
}

// Page 404
void handleNotFound()
{
    webServer.send(404, "text/plain", "404 - File Not Found.");
}

// Page /
void handleRoot()
{
    String message = F("<!doctype html>");
        message += F("<html>");
        message += F("<head>");
        message += F("<title>") + String(WEBSITE_TITLE) + F("</title>");
        message += F("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
        message += F("<meta http-equiv=\"refresh\" content=\"60\" charset=\"UTF-8\">");
        message += F("<link rel=\"shortcut icon\" type=\"image/x-icon\" href=\"web/favicon.ico\">");
        message += F("<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css\">");
        message += F("<style>");
        message += F("body{background-color:#FFFFFF;text-align:center;color:#333333;font-family:Sans-serif;font-size:16px;}");
        message += F("button{background-color:#1FA3EC;text-align:center;color:#FFFFFF;width:200px;padding:10px;border:5px solid #FFFFFF;font-size:24px;border-radius:10px;}");
        message += F("</style>");
        message += F("</head>");
        message += F("<body>");
        message += F("<h1>") + String(WEBSITE_TITLE) + F("</h1>");
#ifdef DEDICATION
    message += DEDICATION;
    message += F("<br><br>");
#endif
    if (mode == MODE_BLANK)
        message += F("<button title=\"Switch LEDs on\" onclick=\"window.location.href='/handleButtonOnOff'\"><i class=\"fa fa-toggle-off\"></i></button>");
    else
        message += F("<button title=\"Switch LEDs off\" onclick=\"window.location.href='/handleButtonOnOff'\"><i class=\"fa fa-toggle-on\"></i></button>");
    message += F("<button title=\"Settings\" onclick=\"window.location.href='/handleButtonSettings'\"><i class=\"fa fa-gear\"></i></button>");
        message += F("<br><br>");
        message += F("<button title=\"Switch modes\" onclick=\"window.location.href='/handleButtonMode'\"><i class=\"fa fa-bars\"></i></button>");
        message += F("<button title=\"Return to time\" onclick=\"window.location.href='/handleButtonTime'\"><i class=\"fa fa-clock-o\"></i></button>");
#if defined(RTC_BACKUP) || defined(SENSOR_DHT22) || defined(SENSOR_MCP9808) || defined(SENSOR_BME280)
    message += F("<br><br><i class = \"fa fa-home\" style=\"font-size:20px;\"></i>");
    message += F("<br><i class=\"fa fa-thermometer\" style=\"font-size:20px;\"></i> ") + String(roomTemperature) + F("&deg;C / ") + String(roomTemperature * 1.8 + 32.0) + F("&deg;F");
#endif
#if defined(SENSOR_DHT22) || defined(SENSOR_BME280)
    message += F("<br><i class=\"fa fa-tint\" style=\"font-size:20px;\"></i> ") + String(roomHumidity) + F("% RH");
        message += F("<br><span style=\"font-size:20px;\">");
    if (roomHumidity < 30)
        message += F("<i style=\"color:Red;\" class=\"fa fa-square\"\"></i>");
    else
        message += F("<i style=\"color:Red;\" class=\"fa fa-square-o\"></i>");
    if ((roomHumidity >= 30) && (roomHumidity < 40))
        message += F("&nbsp;<i style=\"color:Orange;\" class=\"fa fa-square\"></i>");
    else
        message += F("&nbsp;<i style=\"color:Orange;\" class=\"fa fa-square-o\"></i>");
    if ((roomHumidity >= 40) && (roomHumidity <= 50))
        message += F("&nbsp;<i style=\"color:MediumSeaGreen;\" class=\"fa fa-square\"></i>");
    else
        message += F("&nbsp;<i style=\"color:MediumSeaGreen;\" class=\"fa fa-square-o\"></i>");
    if ((roomHumidity > 50) && (roomHumidity < 60))
        message += F("&nbsp;<i style=\"color:Lightblue;\" class=\"fa fa-square\"></i>");
    else
        message += F("&nbsp;<i style=\"color:Lightblue;\" class=\"fa fa-square-o\"></i>");
    if (roomHumidity >= 60)
        message += F("&nbsp;<i style=\"color:Blue;\" class=\"fa fa-square\"></i>");
    else
        message += F("&nbsp;<i style=\"color:Blue;\" class=\"fa fa-square-o\"></i>");
    message += F("</span>");
#endif
#ifdef APIKEY
    if (String(settings.mySettings.owApiKey) != "") {
    message += F("<br><br><i class = \"fa fa-tree\" style=\"font-size:20px;\"></i>");
        message += F("<br><i class = \"fa fa-thermometer\" style=\"font-size:20px;\"></i> ") + String(outdoorWeather.temperature) + F("&deg;C / ") + String(outdoorWeather.temperature * 1.8 + 32.0) + F("&deg;F");
        message += F("<br><i class = \"fa fa-tint\" style=\"font-size:20px;\"></i> ") + String(outdoorWeather.humidity) + F("% RH");
        message += F("<br>") + String(outdoorWeather.pressure) + F(" hPa / ") + String(outdoorWeather.pressure / 33.865) + F(" inHg");
        message += F("<br><i class = \"fa fa-sun-o\" style=\"font-size:20px;\"></i> ") + String(hour(timeZone.toLocal(outdoorWeather.sunrise))) + F(":");
        if (minute(timeZone.toLocal(outdoorWeather.sunrise)) < 10) message += F("0");
        message += String(minute(timeZone.toLocal(outdoorWeather.sunrise)));
        message += F(" <i class = \"fa fa-moon-o\" style=\"font-size:20px;\"></i> ") + String(hour(timeZone.toLocal(outdoorWeather.sunset))) + F(":");
        if (minute(timeZone.toLocal(outdoorWeather.sunset)) < 10) message += F("0");
        message += String(minute(timeZone.toLocal(outdoorWeather.sunset)));
        message += F("<br>") + outdoorWeather.description;
    }
#endif
    message += F("<span style=\"font-size:12px;\">");
        message += F("<br><br><a href=\"http://shop.bracci.ch/\">zytQuadrat</a> was <i class=\"fa fa-code\"></i> with <i class=\"fa fa-heart\"></i> by <a href=\"https://github.com/ch570512/Qlockwork/\">ch570512</a> and <a href=\"https://github.com/bracci/Qlockwork/\">bracci</a>");
        message += F("<br>Firmware: ") + String(FIRMWARE_VERSION);
#ifdef UPDATE_INFOSERVER
    if (updateInfo > int(FIRMWARE_VERSION))
        message += F("<br><span style=\"color:red;\">Firmwareupdate available! (") + String(updateInfo) + F(")</span>");
#endif
#ifdef DEBUG_WEB
    time_t tempEspTime = now();
    message += F("<br><br>Time: ") + String(hour(tempEspTime)) + F(":");
    if (minute(tempEspTime) < 10)
        message += F("0");
    message += String(minute(tempEspTime));
    if (timeZone.locIsDST(now()))
        message += F(" (DST)");
    message += F(" up ") + String(int(upTime / 86400)) + F(" days, ") + String(hour(upTime)) + F(":");
    if (minute(upTime) < 10)
        message += F("0");
    message += String(minute(upTime));
    message += F("<br>") + String(dayStr(weekday(tempEspTime))) + F(", ") + String(monthStr(month(tempEspTime))) + F(" ") + String(day(tempEspTime)) + F(". ") + String(year(tempEspTime));
    message += F("<br>Moonphase: ") + String(moonphase);
    message += F("<br>Free RAM: ") + String(ESP.getFreeHeap()) + F(" bytes");
    message += F("<br>Max Free Block Size: ") + String(ESP.getMaxFreeBlockSize()) + F(" bytes");
    message += F("<br>RSSI: ") + String(WiFi.RSSI());
#ifdef LDR
    message += F("<br>Brightness: ") + String(brightness) + F(" (ABC: ");
    settings.mySettings.useAbc ? message += F("enabled") : message += F("disabled");
    message += F(", min: ") + String(MIN_BRIGHTNESS) + F(", max : ") + String(maxBrightness) + F(")");
    message += F("<br>LDR: ") + String(ldrValue) + F(" (min: ") + String(minLdrValue) + F(", max: ") + String(maxLdrValue) + F(")");
#endif
    message += F("<br>Error (NTP): ") + String(errorCounterNTP);
#ifdef SENSOR_DHT22
    message += F("<br>Error (DHT): ") + String(errorCounterDHT);
#endif
#ifdef SENSOR_BME280
    message += F("<br>Error (BME): ") + String(errorCounterBME);
#endif
#ifdef SENSOR_MCP9808
    message += F("<br>Error (MCP): ") + String(errorCounterMCP);
#endif
#ifdef APIKEY
    message += F("<br>Error (OpenWeather): ") + String(errorCounterOutdoorWeather);
#endif
    message += F("<br>Reset reason: ") + ESP.getResetReason();
    if (rtc_info->reason ==  REASON_WDT_RST  ||
    rtc_info->reason ==  REASON_EXCEPTION_RST  ||
    rtc_info->reason ==  REASON_SOFT_WDT_RST)  {
      if (rtc_info->reason ==  REASON_EXCEPTION_RST) {
        message += F("<br>Fatal exception (") + String(rtc_info->exccause) + F("):");
      }
      char exceptionAdr[64];
      sprintf(exceptionAdr, "<br>epc1=0x%08x, epc2=0x%08x, epc3=0x%08x, excvaddr=0x%08x, depc=0x%08x\n", rtc_info->epc1,  rtc_info->epc2, rtc_info->epc3, rtc_info->excvaddr,  rtc_info->depc); //The address of the last crash is printed, which is used to debug garbled output.
      message += String(exceptionAdr);
    }
    message += F("<br>Flags: ");
#ifdef RTC_BACKUP
    message += F("RTC ");
#else
    message += F("<s>RTC</s> ");
#endif
#ifdef SENSOR_MCP9808
    message += F("MCP9808 ");
#else
    message += F("<s>MCP9808</s> ");
#endif
#ifdef SENSOR_DHT22
    message += F("DHT22 ");
#else
    message += F("<s>DHT22</s> ");
#endif
#ifdef SENSOR_BME280
    message += F("BME280 ");
#else
    message += F("<s>BME280</s> ");
#endif
#ifdef LDR
    message += F("LDR ");
#else
    message += F("<s>LDR</s> ");
#endif
#ifdef BUZZER
    message += F("BUZZER ");
#else
    message += F("<s>BUZZER</s> ");
#endif
#ifdef IR_RECEIVER
    message += F("IR_RECEIVER ");
#else
    message += F("<s>IR_RECEIVER</s> ");
#endif
#ifdef ESP_LED
    message += F("ESP_LED ");
#else
    message += F("<s>ESP_LED</s> ");
#endif
#if defined(ONOFF_BUTTON) || defined(MODE_BUTTON) || defined(TIME_BUTTON)
    message += F("BUTTONS ");
#else
    message += F("<s>BUTTONS</s> ");
#endif
#endif
    message += F("</span></body></html>");
    webServer.send(200, "text/html", message);
}

// Page settings
void handleButtonSettings()
{
#ifdef DEBUG
    Serial.println("Settings pressed.");
#endif
    String message = F("<!doctype html>");
        message += F("<html>");
        message += F("<head>");
        message += F("<title>") + String(WEBSITE_TITLE) + F(" ");
        message += F(TXT_SETTINGS);
        message += F("</title>");
        message += F("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
        message += F("<meta charset=\"UTF-8\">");
        message += F("<link rel=\"shortcut icon\" type=\"image/x-icon\" href=\"web/favicon.ico\">");
        message += F("<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css\">");
        message += F("<style>");
        message += F("body{background-color:#FFFFFF;text-align:center;color:#333333;font-family:Sans-serif;font-size:16px;}");
        message += F("input[type=submit]{background-color:#1FA3EC;text-align:center;color:#FFFFFF;width:200px;padding:12px;border:5px solid #FFFFFF;font-size:20px;border-radius:10px;}");
        message += F("table{border-collapse:collapse;margin:0px auto;} td{padding:12px;border-bottom:1px solid #ddd;} tr:first-child{border-top:1px solid #ddd;} td:first-child{text-align:right;} td:last-child{text-align:left;}");
        message += F("select{font-size:16px;}");
        message += F("button{background-color:#1FA3EC;text-align:center;color:#FFFFFF;width:200px;padding:10px;border:5px solid #FFFFFF;font-size:24px;border-radius:10px;}");
        message += F("</style>");
        message += F("</head>");
        message += F("<body>");
        message += F("<h1>") + String(WEBSITE_TITLE) + F(" ");
        message += F(TXT_SETTINGS);
        message += F("</h1>");
        message += F("<form action=\"/commitSettings\">");
        message += F("<table>");
    // ------------------------------------------------------------------------
#ifdef BUZZER
        message += F("<tr><td>");
        message += F(TXT_ALARM);
        message += F(" 1</td><td>");
        message += F("<input type=\"radio\" name=\"a1\" value=\"1\"");
    if (settings.mySettings.alarm1)
        message += F(" checked");
        message += F("> ");
        message += F(TXT_ON);
        message += F(" <input type=\"radio\" name=\"a1\" value=\"0\"");
    if (!settings.mySettings.alarm1)
        message += F(" checked");
        message += F("> ");
        message += F(TXT_OFF);
        message += F(" <input type=\"time\" name=\"a1t\" value=\"");
    if (hour(settings.mySettings.alarm1Time) < 10)
        message += F("0");
        message += String(hour(settings.mySettings.alarm1Time)) + F(":");
    if (minute(settings.mySettings.alarm1Time) < 10)
        message += F("0");
        message += String(minute(settings.mySettings.alarm1Time)) + F("\">");
        message += F(" h<br><br>");
        message += F("<input type=\"checkbox\" name=\"a1w2\" value=\"4\"");
    if (bitRead(settings.mySettings.alarm1Weekdays, 2))
        message += F(" checked");
        message += F("> Mo. ");
        message += F("<input type=\"checkbox\" name=\"a1w3\" value=\"8\"");
    if (bitRead(settings.mySettings.alarm1Weekdays, 3))
        message += F(" checked");
        message += F("> Tu. ");
        message += F("<input type=\"checkbox\" name=\"a1w4\" value=\"16\"");
    if (bitRead(settings.mySettings.alarm1Weekdays, 4))
        message += F(" checked");
        message += F("> We. ");
        message += F("<input type=\"checkbox\" name=\"a1w5\" value=\"32\"");
    if (bitRead(settings.mySettings.alarm1Weekdays, 5))
        message += F(" checked");
        message += F("> Th. ");
        message += F("<input type=\"checkbox\" name=\"a1w6\" value=\"64\"");
    if (bitRead(settings.mySettings.alarm1Weekdays, 6))
        message += F(" checked");
        message += F("> Fr. ");
        message += F("<input type=\"checkbox\" name=\"a1w7\" value=\"128\"");
    if (bitRead(settings.mySettings.alarm1Weekdays, 7))
        message += F(" checked");
        message += F("> Sa. ");
        message += F("<input type=\"checkbox\" name=\"a1w1\" value=\"2\"");
    if (bitRead(settings.mySettings.alarm1Weekdays, 1))
        message += F(" checked");
        message += F("> Su. ");
        message += F("</td></tr>");
    // ------------------------------------------------------------------------
        message += F("<tr><td>");
        message += F(TXT_ALARM);
        message += F(" 2</td><td>");
        message += F("<input type=\"radio\" name=\"a2\" value=\"1\"");
    if (settings.mySettings.alarm2)
        message += F(" checked");
        message += F(">) ");
        message += F(TXT_ON);
        message += F(" <input type=\"radio\" name=\"a2\" value=\"0\"");
    if (!settings.mySettings.alarm2)
        message += F(" checked");
        message += F("> ");
        message += F(TXT_OFF);
        message += F(" <input type=\"time\" name=\"a2t\" value=\"");
    if (hour(settings.mySettings.alarm2Time) < 10)
        message += F("0");
        message += String(hour(settings.mySettings.alarm2Time)) + F(":");
    if (minute(settings.mySettings.alarm2Time) < 10)
        message += F("0");
        message += String(minute(settings.mySettings.alarm2Time)) + F("\">");
        message += F(" h<br><br>");
        message += F("<input type=\"checkbox\" name=\"a2w2\" value=\"4\"");
    if (bitRead(settings.mySettings.alarm2Weekdays, 2))
        message += F(" checked");
        message += F("> Mo. ");
        message += F("<input type=\"checkbox\" name=\"a2w3\" value=\"8\"");
    if (bitRead(settings.mySettings.alarm2Weekdays, 3))
        message += F(" checked");
        message += F("> Tu. ");
        message += F("<input type=\"checkbox\" name=\"a2w4\" value=\"16\"");
    if (bitRead(settings.mySettings.alarm2Weekdays, 4))
        message += F(" checked");
        message += F("> We. ");
        message += F("<input type=\"checkbox\" name=\"a2w5\" value=\"32\"");
    if (bitRead(settings.mySettings.alarm2Weekdays, 5))
        message += F(" checked");
        message += F("> Th. ");
        message += F("<input type=\"checkbox\" name=\"a2w6\" value=\"64\"");
    if (bitRead(settings.mySettings.alarm2Weekdays, 6))
        message += F(" checked");
        message += F("> Fr. ");
        message += F("<input type=\"checkbox\" name=\"a2w7\" value=\"128\"");
    if (bitRead(settings.mySettings.alarm2Weekdays, 7))
        message += F(" checked");
        message += F("> Sa. ");
        message += F("<input type=\"checkbox\" name=\"a2w1\" value=\"2\"");
    if (bitRead(settings.mySettings.alarm2Weekdays, 1))
        message += F(" checked");
        message += F("> Su. ");
        message += F("</td></tr>");
    // ------------------------------------------------------------------------
        message += F("<tr><td>");
        message += F(TXT_TIMER);
        message += F("</td><td>");
        message += F("<select name=\"ti\">");
    for (int i = 0; i <= 10; i++)
    {
        message += F("<option value=\"") + String(i) + F("\">");
        if (i < 10)
            message += F("0");
            message += String(i) + F("</option>");
    }
        message += F("<option value=\"15\">15</option>");
        message += F("<option value=\"20\">20</option>");
        message += F("<option value=\"25\">25</option>");
        message += F("<option value=\"30\">30</option>");
        message += F("<option value=\"45\">45</option>");
        message += F("<option value=\"60\">60</option>");
        message += F("</select> "); 
        message += F(TXT_MINUTES);
        message += F("</td></tr>");
    // ------------------------------------------------------------------------
        message += F("<tr><td>");
        message += F(TXT_HOURBEEP);
        message += F("</td><td>");
        message += F("<input type=\"radio\" name=\"hb\" value=\"1\"");
    if (settings.mySettings.hourBeep)
        message += F(" checked");
        message += F("> ");
        message += F(TXT_ON);
        message += F(" <input type=\"radio\" name=\"hb\" value=\"0\"");
    if (!settings.mySettings.hourBeep)
        message += F(" checked");
        message += F("> ");
        message += F(TXT_OFF);
        message += F(" </td></tr>");
#endif
    // ------------------------------------------------------------------------
#if defined(RTC_BACKUP) || defined(SENSOR_DHT22) || defined(SENSOR_MCP9808) || defined(SENSOR_BME280)
        message += F("<tr><td>");
        message += F(TXT_SHOW_TEMP);
        message += F("</td><td>");
        message += F("<input type=\"radio\" name=\"mc\" value=\"1\"");
    if (settings.mySettings.modeChange)
        message += F(" checked");
        message += F("> ");
        message += F(TXT_ON);
        message += F(" <input type=\"radio\" name=\"mc\" value=\"0\"");
    if (!settings.mySettings.modeChange)
        message += F(" checked");
        message += F("> ");
        message += F(TXT_OFF);
        message += F(" </td></tr>");
#endif
    // ------------------------------------------------------------------------
#ifdef LDR
        message += F("<tr><td>");
        message += F(TXT_AUTO_BRIGHTNESS);
        message += F("</td><td>");
        message += F("<input type=\"radio\" name=\"ab\" value=\"1\"");
    if (settings.mySettings.useAbc)
        message += F(" checked");
        message += F("> ");
        message += F(TXT_ON);
        message += F(" <input type=\"radio\" name=\"ab\" value=\"0\"");
    if (!settings.mySettings.useAbc)
        message += F(" checked");
        message += F("> ");
        message += F(TXT_OFF);
        message += F(" </td></tr>");
#endif
    // ------------------------------------------------------------------------
    message += F("<tr><td>");
        message += F(TXT_BRIGHTNESS);
        message += F("</td><td>");
        message += F("<select name=\"br\">");
    for (int i = 10; i <= 100; i += 10)
    {
        message += F("<option value=\"") + String(i) + F("\"");
        if (i == settings.mySettings.brightness)
            message += F(" selected");
        message += F(">");
        message += String(i) + F("</option>");
    }
        message += F("</select> %");
        message += F("</td></tr>");
    // ------------------------------------------------------------------------
        message += F("<tr><td>");
        message += F(TXT_COLOR);
        message += F("</td><td>");
        
        message += F("<select name=\"co\"");
        if (settings.mySettings.colorChange != COLORCHANGE_NO) message += F(" disabled");
        message += F(">");
        uint8_t colorNum = settings.mySettings.color;
        for(uint8_t j = 0; j <= COLOR_COUNT; j++){
          message += F("<option value=\"") +String(j) + F("\"");
          if (colorNum == j) message += F(" selected");
          message += F(">");
          message += String(FPSTR(sColorStr[j])) + F("</option>");
        }
        message += F("</select>");
        message += F("</td></tr>");
    // ------------------------------------------------------------------------
        message += F("<tr><td>");
        message += F(TXT_COLORCHANGE);
        message += F("</td><td>");
        message += F("<input type=\"radio\" name=\"cc\" value=\"4\"");
    if (settings.mySettings.colorChange == 4)
        message += F(" checked");
        message += F("> Mood ");
        message += F("<input type=\"radio\" name=\"cc\" value=\"3\"");
    if (settings.mySettings.colorChange == 3)
        message += F(" checked");
        message += F("> 24h ");
        message += F("<input type=\"radio\" name=\"cc\" value=\"2\"");
    if (settings.mySettings.colorChange == 2)
        message += F(" checked");
        message += F("> 1h ");
        message += F("<input type=\"radio\" name=\"cc\" value=\"1\"");
    if (settings.mySettings.colorChange == 1)
        message += F(" checked");
        message += F("> 5 min ");
        message += F("<input type=\"radio\" name=\"cc\" value=\"0\"");
    if (settings.mySettings.colorChange == 0)
        message += F(" checked");
        message += F("> ");
        message += F(TXT_OFF);
        message += F(" </td></tr>");
    // ------------------------------------------------------------------------
        message += F("<tr><td>");
        message += F(TXT_TRANSITION);
        message += F("</td><td>");
        message += F("<input type=\"radio\" name=\"tr\" value=\"2\"");
    if (settings.mySettings.transition == 2)
        message += F(" checked");
        message += F("> Fading ");
        message += F("<input type=\"radio\" name=\"tr\" value=\"3\"");
    if (settings.mySettings.transition == 3)
        message += F(" checked");
        message += F("> Matrix ");
        message += F("<input type=\"radio\" name=\"tr\" value=\"1\"");
    if (settings.mySettings.transition == 1)
        message += F(" checked");
        message += F("> Move ");
        message += F("<input type=\"radio\" name=\"tr\" value=\"0\"");
    if (settings.mySettings.transition == 0)
        message += F(" checked");
        message += F("> ");
        message += F(TXT_OFF);
        message += F(" </td></tr>");

    // ------------------------------------------------------------------------
        message += F("<tr><td>");
        message += F(TXT_TIMEOUT);
        message += F("</td><td>");
        message += F("<select name=\"to\">");
    for (int i = 0; i <= 60; i += 5) {
        message += F("<option value=\"") + String(i) + F("\"");
        if (i == settings.mySettings.timeout)
            message += F(" selected");
        message += F(">");
        if (i < 10)
            message += F("0");
        message += String(i) + F("</option>");
    }
        message += F("</select> s");
        message += F("</td></tr>");
    // ------------------------------------------------------------------------
        message += F("<tr><td>");
        message += F(TXT_NIGHT_OFF);
        message += F("</td><td>");
        message += F("<input type=\"time\" name=\"no\" value=\"");
    if (hour(settings.mySettings.nightOffTime) < 10)
        message += F("0");
        message += String(hour(settings.mySettings.nightOffTime)) + F(":");
    if (minute(settings.mySettings.nightOffTime) < 10)
        message += F("0");
        message += String(minute(settings.mySettings.nightOffTime)) + F("\">");
        message += F("</td></tr>");
    // ------------------------------------------------------------------------
        message += F("<tr><td>");
        message += F(TXT_DAY_ON);
        message += F("</td><td>");
        message += F("<input type=\"time\" name=\"do\" value=\"");
    if (hour(settings.mySettings.dayOnTime) < 10)
        message += F("0");
        message += String(hour(settings.mySettings.dayOnTime)) + F(":");
    if (minute(settings.mySettings.dayOnTime) < 10)
        message += F("0");
        message += String(minute(settings.mySettings.dayOnTime)) + F("\">");
        message += F("</td></tr>");
    // ------------------------------------------------------------------------
        message += F("<tr><td>");
        message += F(TXT_PURIST_MODE);
        message += F("</td><td>");
        message += F("<input type=\"radio\" name=\"ii\" value=\"1\"");
    if (settings.mySettings.purist)
        message += F(" checked");
        message += F("> ");
        message += TXT_ON;
        message += F(" <input type=\"radio\" name=\"ii\" value=\"0\"");
    if (!settings.mySettings.purist)
        message += F(" checked");
        message += F("> ");
        message += TXT_OFF;
        message += F(" </td></tr>");
    // ------------------------------------------------------------------------
    if((settings.mySettings.frontCover == FRONTCOVER_CH_BE) || (settings.mySettings.frontCover == FRONTCOVER_CH_ZH) || (settings.mySettings.frontCover == FRONTCOVER_CH_AG) || (settings.mySettings.frontCover == FRONTCOVER_CH_GR)) {
        message += F("<tr><td>");
        message += F(TXT_SHOW_GSI);
        message += F("</td><td>");
        message += F("<input type=\"radio\" name=\"gs\" value=\"1\"");
    if (settings.mySettings.chGsi)
        message += F(" checked");
        message += F("> ");
        message += TXT_ON;
        message += F(" <input type=\"radio\" name=\"gs\" value=\"0\"");
    if (!settings.mySettings.chGsi)
        message += F(" checked");
        message += F("> ");
        message += TXT_OFF;
        message += F(" </td></tr>");
    }
    // ------------------------------------------------------------------------
        message += F("<tr><td>");
        message += F(TXT_SET_DATE_TIME);
        message += F("</td><td>");
        message += F("<input type=\"datetime-local\" name=\"st\">");
        message += F("</td></tr>");
    // ------------------------------------------------------------------------
        message += F("</table>");
        message += F("<br><button title=\"Save Settings.\"><i class=\"fa fa-floppy-o\"></i></button>");
        message += F("</form>");
        message += F("<br><br>");
        message += F("<button title=\"Events\" onclick=\"window.location.href='/handleButtonEvents'\"><i class=\"fa fa-birthday-cake\"></i></button>");
        message += F("<br>");
        message += F("<button title=\"Home\" onclick=\"window.location.href='/'\"><i class=\"fa fa-home\"></i></button>");
        message += F("</body></html>");
    webServer.send(200, "text/html", message);
}

// Page settings.
void handleButtonEvents()
{
#ifdef DEBUG
  Serial.println("Events pressed.");
#endif
  String message = F("<!doctype html>");
  message += F("<html>");
  message += F("<head>");
  message += F("<title>") + String(WEBSITE_TITLE) + F(" Events</title>");
  message += F("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
  message += F("<link rel=\"shortcut icon\" type=\"image/x-icon\" href=\"web/favicon.ico\">");
  message += F("<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css\">");
  message += F("<style>");
  message += F("body{background-color:#FFFFFF;text-align:center;color:#333333;font-family:Sans-serif;font-size:16px;}");
  message += F("input[type=submit]{background-color:#1FA3EC;text-align:center;color:#FFFFFF;width:200px;padding:12px;border:5px solid #FFFFFF;font-size:20px;border-radius:10px;}");
  message += F("button{background-color:#1FA3EC;text-align:center;color:#FFFFFF;width:200px;padding:10px;border:5px solid #FFFFFF;font-size:24px;border-radius:10px;}");
  message += F("table{border-collapse:collapse;margin:0px auto;} td{padding:12px;border-bottom:1px solid #ddd; text-align:right;} tr:first-child{border-top:1px solid #ddd; text-align:right;} td:first-child{text-align:right;} td:last-child{text-align:left;}");
  message += F("select{font-size:16px;}");
  message += F("</style>");
  message += F("</head>");
  message += F("<body>");
  message += F("<h1>") + String(WEBSITE_TITLE) + F(" Events</h1>");
  message += F("<form action=\"/commitEvents\">");
  message += F("<table>");

  // ------------------------------------------------------------------------
  for (uint8_t i = 0; i < NUM_EVTS; i++){
    message += F("<tr><td rowspan=\"6\">");
    message += F("Event ") + String(i + 1);
    message += F("</td><td>");
    message += F(TXT_ACTIVE);
    message += F("</td>");
    message += F("<td>");
    message += F("<input type=\"radio\" name=\"ev") + String(i) + F("\" value=\"1\"");
    if (settings.mySettings.events[i].enabled) message += F(" checked");
    message += F(">");
    message += F(TXT_ON);
    message += F(" <input type=\"radio\" name=\"ev") + String(i) + F("\" value=\"0\"");
    if (!settings.mySettings.events[i].enabled) message += F(" checked");
    message += F("> ");
    message += F(TXT_OFF);
    message += F(" </td></tr>");
    message += F("<tr><td>");
    message += F(TXT_DATE);
    message += F("</td><td>");
    message += F("<input type=\"date\" name=\"ev") + String(i) + F("d\" value=\"");
    message += String(year(settings.mySettings.events[i].time)) + F("-");
    if (month(settings.mySettings.events[i].time) < 10) message += F("0");
    message += String(month(settings.mySettings.events[i].time)) + F("-");
    if (day(settings.mySettings.events[i].time) < 10) message += F("0");
    message += String(day(settings.mySettings.events[i].time))+ F("\" min=\"1970-01-01\">");
    message += F("</td></tr>");
    message += F("<tr><td>");
    message += F(TXT_TEXT);
    message += F("</td><td>");
    message += F("<input type=\"text\" name=\"ev") + String(i) + F("t\" value=\"");
    message += String(settings.mySettings.events[i].txt)+ F("\" pattern=\"[\\x20-\\x7e]{0,") + String(LEN_EVT_STR-1) + F("}\" placeholder=\"Event text ...\">");
    message += F("</td></tr>");
    message += F("<tr><td>");
    message += F(TXT_ANIMATION);
    message += F("</td><td>");
    message += F("<select name=\"ev") + String(i) + F("ani\">");
    for(uint8_t j = 0; j < MAXANIMATION; j++){
      if (myanimationslist[j].length() != 0){
        message += F("<option value=\"") +String(j) + F("\"");
        if (myanimationslist[j] == String(settings.mySettings.events[i].animation)) message += F(" selected");
        message += F(">");
        message += myanimationslist[j] + F("</option>");
      }
    }
    message += F("</td></tr>");
    message += F("<tr><td>");
    message += F(TXT_COLOR);
    message += F("</td><td>");
    message += F("<select name=\"ev") + String(i) + F("c\">");
    uint8_t colorNum = settings.mySettings.events[i].color;
    for(uint8_t j = 0; j <= COLOR_COUNT; j++){
      message += F("<option value=\"") +String(j) + F("\"");
      if (colorNum == j) message += F(" selected");
      message += F(">");
      message += String(FPSTR(sColorStr[j])) + F("</option>");
    }
    message += F("</select>");
    message += F("</td></tr>");
    message += F("<tr><td>");
    message += F(TXT_REP_RATE);
    message += F("</td><td>");
    message += F("<select name=\"ev") + String(i) + F("rep\">");
    for(uint8_t j = 0; j < EVT_REP_COUNT; j++){
      message += F("<option value=\"") + String(j) + F("\"");
      if (settings.mySettings.events[i].repRate == j) message += F(" selected");
      message += F(">") + String(FPSTR(sEvtRep[j])) + F("</option>");
    }
    message += F("</select> min.");
    message += F("</td></tr>");
  }
  // ------------------------------------------------------------------------

  message += F("</table>");
  message += F("<br><button title=\"Save Events.\"><i class=\"fa fa-floppy-o\"></i></button>");
  message += F("</form>");
  message += F("<br><br>");
  message += F("<button title=\"Settings\" onclick=\"window.location.href='/handleButtonSettings'\"><i class=\"fa fa-gear\"></i></button>");
  message += F("<br>");
  message += F("<button title=\"Home\" onclick=\"window.location.href='/'\"><i class=\"fa fa-home\"></i></button>");
  message += F("</body>");
  message += F("</html>");
  Serial.println("Free Heap: " + String(ESP.getFreeHeap()));
  Serial.println("Max Free Block: " + String(ESP.getMaxFreeBlockSize()));
  webServer.send(200, "text/html", message);
}

void handleAdmin()
{
  #ifdef DEBUG
    Serial.println("Admin settings entered.");
#endif
    String message = F("<!doctype html>");
        message += F("<html>");
        message += F("<head>");
        message += F("<title>") + String(WEBSITE_TITLE) + F(" ");
        message += F(TXT_ADMIN);
        message += F("</title>");
        message += F("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
        message += F("<meta charset=\"UTF-8\">");
        message += F("<link rel=\"shortcut icon\" type=\"image/x-icon\" href=\"web/favicon.ico\">");
        message += F("<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css\">");
        message += F("<style>");
        message += F("body{background-color:#FFFFFF;text-align:center;color:#333333;font-family:Sans-serif;font-size:16px;}");
        message += F("input[type=submit]{background-color:#1FA3EC;text-align:center;color:#FFFFFF;width:200px;padding:12px;border:5px solid #FFFFFF;font-size:20px;border-radius:10px;}");
        message += F("table{border-collapse:collapse;margin:0px auto;} td{padding:12px;border-bottom:1px solid #ddd;} tr:first-child{border-top:1px solid #ddd;} td:first-child{text-align:right;} td:last-child{text-align:left;}");
        message += F("select{font-size:16px;}");
        message += F("button{background-color:#1FA3EC;text-align:center;color:#FFFFFF;width:200px;padding:10px;border:5px solid #FFFFFF;font-size:24px;border-radius:10px;}");
        message += F("</style>");
        message += F("</head>");
        message += F("<body>");
        message += F("<h1>") + String(WEBSITE_TITLE) + F(" ");
        message += F(TXT_ADMIN);
        message += F("</h1>");
        message += F("<form action=\"/commitAdminSettings\">");
        message += F("<table>");
    // ------------------------------------------------------------------------
        message += F("<tr><td>");
        message += F(TXT_TIME_SERVER);
        message += F("</td><td>");
        message += F("<input type=\"text\" name=\"adts\" value=\"");
        message += String(settings.mySettings.timeServer)+ F("\" pattern=\"[\\x20-\\x7e]{0,") + String(LEN_TS_URL-1) + F("}\" placeholder=\"e.g. pool.ntp.org\">");
        message += F("</td></tr>");
#ifdef APIKEY
        message += F("<tr><td>");
        message += F(TXT_OW_API_KEY);
        message += F("</td><td>");
        message += F("<input type=\"text\" name=\"adwk\" value=\"");
        message += String(settings.mySettings.owApiKey)+ F("\" pattern=\"[\\x20-\\x7e]{0,") + String(LEN_OW_API_KEY-1) + F("}\" placeholder=\"API Key\">");
        message += F("</td></tr>");
        message += F("<tr><td>");
        message += F(TXT_OW_LOCATION);
        message += F("</td><td>");
        message += F("<input type=\"text\" name=\"adwl\" value=\"");
        message += String(settings.mySettings.owLocation)+ F("\" pattern=\"[\\x20-\\x7e]{0,") + String(LEN_OW_LOCATION-1) + F("}\" placeholder=\"e.g. Bern, CH\">");
        message += F("</td></tr>");
#endif
        message += F("<tr><td>");
        message += F(TXT_COVER_LANGUAGE);
        message += F("</td><td>");
        message += F("<select name=\"adfc\">");
        for(uint8_t j = 0; j < FRONTCOVER_COUNT; j++){
        message += F("<option value=\"") + String(j) + F("\"");
          if (settings.mySettings.frontCover == j) message += F(" selected");
          message += F(">") + String(FPSTR(sLanguageStr[j])) + F("</option>");
        }
        message += F("</select>");
        message += F("</td></tr>");
        message += F("<tr><td>");
        message += F(TXT_LDR_ROW);
        message += F("</td><td>");
        message += F("<select name=\"aday\">");
        for(uint8_t i = 0; i < NUMPIXELS_Y; i++){
        message += F("<option value=\"") + String(i) + F("\"");
          if (settings.mySettings.ldrPosY == i) message += F(" selected");
          message += F(">") + String(i+1) + F("</option>");
        }
        message += F("</select>");
        message += F("</td></tr>");
        message += F("<tr><td>");
        message += F(TXT_LDR_COLUMN);
        message += F("</td><td>");
        message += F("<select name=\"adax\">");
        for(uint8_t i = 0; i < NUMPIXELS_X; i++){
        message += F("<option value=\"") + String(i) + F("\"");
          if (settings.mySettings.ldrPosX == i) message += F(" selected");
          message += F(">") + String(i+1) + F("</option>");
        }
        message += F("</select>");
        message += F("</td></tr>");
    // ------------------------------------------------------------------------
        message += F("</table>");
        message += F("<br><button title=\"Save Settings.\"><i class=\"fa fa-floppy-o\"></i></button>");
        message += F("</form>");
        message += F("<br><br>");
        message += F("<button title=\"File System\" onclick=\"window.location.href='/fs'\"><i class=\"fa fa-folder-open\"></i></button>");
        message += F("<br>");
        message += F("<button title=\"Settings\" onclick=\"window.location.href='/handleButtonSettings'\"><i class=\"fa fa-gear\"></i></button>");
        message += F("<br>");
        message += F("<button title=\"Home\" onclick=\"window.location.href='/'\"><i class=\"fa fa-home\"></i></button>");
        message += F("</body></html>");
    webServer.send(200, "text/html", message);
}

void handleCommitSettings()
{
#ifdef DEBUG
    Serial.println("Commit settings pressed.");
#endif
    // ------------------------------------------------------------------------
#ifdef BUZZER
    time_t alarmTimeFromWeb = 0;
    webServer.arg("a1") == "0" ? settings.mySettings.alarm1 = false : settings.mySettings.alarm1 = true;
    alarmTimeFromWeb = webServer.arg("a1t").substring(0, 2).toInt() * 3600 + webServer.arg("a1t").substring(3, 5).toInt() * 60;
    if (settings.mySettings.alarm1Time != alarmTimeFromWeb)
        settings.mySettings.alarm1 = true;
    settings.mySettings.alarm1Time = alarmTimeFromWeb;
    settings.mySettings.alarm1Weekdays =
        webServer.arg("a1w1").toInt() +
        webServer.arg("a1w2").toInt() +
        webServer.arg("a1w3").toInt() +
        webServer.arg("a1w4").toInt() +
        webServer.arg("a1w5").toInt() +
        webServer.arg("a1w6").toInt() +
        webServer.arg("a1w7").toInt();
    // ------------------------------------------------------------------------
    webServer.arg("a2") == "0" ? settings.mySettings.alarm2 = false : settings.mySettings.alarm2 = true;
    alarmTimeFromWeb = webServer.arg("a2t").substring(0, 2).toInt() * 3600 + webServer.arg("a2t").substring(3, 5).toInt() * 60;
    if (settings.mySettings.alarm2Time != alarmTimeFromWeb)
        settings.mySettings.alarm2 = true;
    settings.mySettings.alarm2Time = alarmTimeFromWeb;
    settings.mySettings.alarm2Weekdays =
        webServer.arg("a2w1").toInt() +
        webServer.arg("a2w2").toInt() +
        webServer.arg("a2w3").toInt() +
        webServer.arg("a2w4").toInt() +
        webServer.arg("a2w5").toInt() +
        webServer.arg("a2w6").toInt() +
        webServer.arg("a2w7").toInt();
    // ------------------------------------------------------------------------
    webServer.arg("hb") == "0" ? settings.mySettings.hourBeep = false : settings.mySettings.hourBeep = true;
    // ------------------------------------------------------------------------
    if (webServer.arg("ti").toInt()) {
        alarmTimer = webServer.arg("ti").toInt();
        alarmTimerSecond = second();
        alarmTimerSet = true;
        setMode(MODE_TIMER);
#ifdef DEBUG
        Serial.println("Timer started.");
#endif
    }
    else
    {
        if (alarmTimerSet)
        {
            alarmTimer = 0;
            alarmTimerSecond = 0;
            alarmTimerSet = false;
            setMode(MODE_TIME);
#ifdef DEBUG
            Serial.println("Timer stopped.");
#endif
        }
    }
#endif
    // ------------------------------------------------------------------------
#if defined(RTC_BACKUP) || defined(SENSOR_DHT22) || defined(SENSOR_MCP9808) || defined(SENSOR_BME280)
    webServer.arg("mc") == "0" ? settings.mySettings.modeChange = false : settings.mySettings.modeChange = true;
#endif
    // ------------------------------------------------------------------------
#ifdef LDR
    if (webServer.arg("ab") == "0")
    {
        settings.mySettings.useAbc = false;
    }
    else
        settings.mySettings.useAbc = true;
#endif
    // ------------------------------------------------------------------------
    settings.mySettings.brightness = webServer.arg("br").toInt();
    updateBrightness(true);
    // ------------------------------------------------------------------------
    settings.mySettings.color = webServer.arg("co").toInt();
    // ------------------------------------------------------------------------
    switch (webServer.arg("cc").toInt())
    {
    case 0:
        settings.mySettings.colorChange = COLORCHANGE_NO;
        break;
    case 1:
        settings.mySettings.colorChange = COLORCHANGE_FIVE;
        break;
    case 2:
        settings.mySettings.colorChange = COLORCHANGE_HOUR;
        break;
    case 3:
        settings.mySettings.colorChange = COLORCHANGE_DAY;
        break;
    case 4:
        settings.mySettings.colorChange = COLORCHANGE_MOOD;
        settings.mySettings.color = MOOD;
        break;
    }
    // ------------------------------------------------------------------------
    switch (webServer.arg("tr").toInt())
    {
    case 0:
        settings.mySettings.transition = TRANSITION_NORMAL;
        break;
    case 1:
        settings.mySettings.transition = TRANSITION_MOVEUP;
        break;
    case 2:
        settings.mySettings.transition = TRANSITION_FADE;
        break;
    case 3:
        settings.mySettings.transition = TRANSITION_MATRIX;
        break;
    }
    // ------------------------------------------------------------------------
    settings.mySettings.timeout = webServer.arg("to").toInt();
    // ------------------------------------------------------------------------
    settings.mySettings.nightOffTime = webServer.arg("no").substring(0, 2).toInt() * 3600 + webServer.arg("no").substring(3, 5).toInt() * 60;
    // ------------------------------------------------------------------------
    settings.mySettings.dayOnTime = webServer.arg("do").substring(0, 2).toInt() * 3600 + webServer.arg("do").substring(3, 5).toInt() * 60;
    // ------------------------------------------------------------------------
    webServer.arg("ii") == "0" ? settings.mySettings.purist = false : settings.mySettings.purist = true;
    // ------------------------------------------------------------------------
    if((settings.mySettings.frontCover == FRONTCOVER_CH_BE) || (settings.mySettings.frontCover == FRONTCOVER_CH_ZH) || (settings.mySettings.frontCover == FRONTCOVER_CH_AG) || (settings.mySettings.frontCover == FRONTCOVER_CH_GR)) {
      webServer.arg("gs") == "0" ? settings.mySettings.chGsi = false : settings.mySettings.chGsi = true;
    }
    // ------------------------------------------------------------------------
    if (webServer.arg("st").length())
    {
        Serial.println(webServer.arg("st"));
        setTime(webServer.arg("st").substring(11, 13).toInt(), webServer.arg("st").substring(14, 16).toInt(), 0, webServer.arg("st").substring(8, 10).toInt(), webServer.arg("st").substring(5, 7).toInt(), webServer.arg("st").substring(0, 4).toInt());
#ifdef RTC_BACKUP
        RTC.set(now());
#endif
    }
    // ------------------------------------------------------------------------
    settings.saveToEEPROM();
    screenBufferNeedsUpdate = true;
}

void handleCommitEvents()
{
#ifdef DEBUG
  Serial.println("Commit events pressed.");
#endif
  // ------------------------------------------------------------------------
  for (uint8_t i = 0; i < NUM_EVTS; i++){
    char text[LEN_EVT_STR];
    memset(text, 0, sizeof(text));
    webServer.arg("ev" + String(i) + "t").toCharArray(text, sizeof(text), 0);
    memcpy(settings.mySettings.events[i].txt, text, sizeof(text));
    settings.mySettings.events[i].enabled = webServer.arg("ev" + String(i)).toInt();
    char ani[LEN_ANI_STR];
    myanimationslist[webServer.arg("ev" + String(i) + "ani").toInt()].toCharArray(ani, sizeof(ani), 0);
    memcpy(settings.mySettings.events[i].animation, ani, sizeof(ani));
    settings.mySettings.events[i].color = (eColor)(webServer.arg("ev" + String(i) + "c").toInt());
    settings.mySettings.events[i].repRate = (eEvtRep)(webServer.arg("ev" + String(i) + "rep").toInt());
    tmElements_t eventTm;
    eventTm.Year = webServer.arg("ev" + String(i) + "d").substring(0, 4).toInt() - 1970;
    eventTm.Month = webServer.arg("ev" + String(i) + "d").substring(5, 7).toInt();
    eventTm.Day = webServer.arg("ev" + String(i) + "d").substring(8, 10).toInt();
    eventTm.Hour = 0;
    eventTm.Minute = 0;
    eventTm.Second = 0;
    time_t eventTime = makeTime(eventTm);
    settings.mySettings.events[i].time = eventTime;
  }
  // ------------------------------------------------------------------------
#ifdef DEBUG
  for (uint8_t i = 0; i < NUM_EVTS; i++){
      Serial.println("Enable" + String(i) + ": " + webServer.arg("ev" + String(i)).toInt());
      Serial.println("Date" + String(i) + ": " + webServer.arg("ev" + String(i) + "d"));
      Serial.println("Text" + String(i) + ": " + webServer.arg("ev" + String(i) + "t"));
      Serial.println("Animation" + String(i) + ": " + myanimationslist[webServer.arg("ev" + String(i) + "ani").toInt()]);
      Serial.println("Color" + String(i) + ": " + webServer.arg("ev" + String(i) + "c").toInt());
      Serial.println("Rep" + String(i) + ": " + webServer.arg("ev" + String(i) + "rep").toInt());
  }
#endif
  settings.saveToEEPROM();
  screenBufferNeedsUpdate = true;
}

void handleCommitAdminSettings()
{
  webServer.arg("adts").toCharArray(settings.mySettings.timeServer, sizeof(settings.mySettings.timeServer), 0);
  webServer.arg("adwk").toCharArray(settings.mySettings.owApiKey, sizeof(settings.mySettings.owApiKey), 0);
  webServer.arg("adwl").toCharArray(settings.mySettings.owLocation, sizeof(settings.mySettings.owLocation), 0);
  settings.mySettings.frontCover = (eFrontCover)webServer.arg("adfc").toInt();
  settings.mySettings.ldrPosX = webServer.arg("adax").toInt();
  settings.mySettings.ldrPosY = webServer.arg("aday").toInt();
  settings.saveToEEPROM();
  screenBufferNeedsUpdate = true;
}

// Page reset
void handleReset()
{
    webServer.send(200, "text/plain", "OK. I'll be back!");
    ESP.restart();
}

// Settings Reset
void handleSettingsReset()
{
  settings.resetToDefault();
  webServer.send(200, TEXT_PLAIN, F("OK. I'll be back!"));
  ESP.restart();
}

// Factory Reset
void handleFactoryReset()
{
  settings.resetToDefault();
  handleWiFiReset();
}

// WiFi Reset
void handleWiFiReset()
{
  webServer.send(200, TEXT_PLAIN, F("OK. I'll be back as AP!"));
  delay(0);
  webServer.handleClient();
  delay(1000);
  webServer.handleClient();
  delay(5000);
  WiFi.disconnect(true);
  delay(1000);
  ESP.eraseConfig();
  Serial.println(F("ESP Config gelöscht"));
  delay(1000);
  ESP.restart();
}

// Page showText
void handleShowText()
{
    uint8_t feedBuzzer = webServer.arg("buzzer").toInt();
    feedColor = webServer.arg("color").toInt();
    feedText = "  " + webServer.arg("text").substring(0, 80) + "   ";
    feedPosition = 0;
    webServer.send(200, "text/plain", "OK.");

#ifdef DEBUG
    Serial.println("Show text: " + webServer.arg("text").substring(0, 80));
#endif

#ifdef BUZZER
    for (uint8_t i = 0; i < feedBuzzer; i++)
    {
        digitalWrite(PIN_BUZZER, HIGH);
        delay(75);
        digitalWrite(PIN_BUZZER, LOW);
        delay(100);
    }
#endif

    setMode(MODE_FEED);
}

void handleControl()
{
    setMode((Mode)webServer.arg("mode").toInt());
    webServer.send(200, "text/plain", "OK.");
}

//################################################################################################################
// ANIMATIONEN
bool showAnimation(uint8_t brightness)
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  //  unsigned long aktmillis = millis();
  // beim ersten Frame in der ersten loop die Corner LEDs löschen
  if (  ! akt_aniframe && ! akt_aniloop )
  {
    for (uint8_t cp = 0; cp <= 3; cp++)
    {
      ledDriver.setPixelRGB(110 + cp, 0, 0, 0);
    }
    if ( myanimation.laufmode < 2 ) frame_fak = 1;
  }

  if ( akt_aniloop >= myanimation.loops )
  {
    akt_aniloop = 0;
    akt_aniframe = 0;
    frame_fak = 1;
    return false;
  }
  else
  {
#ifdef  DEBUG_ANIMATION
    Serial.println("Start Animation: " + String(myanimation.name) + " Loop: " +  String(akt_aniloop) + " Frame: " + String(akt_aniframe) );
#endif

    for ( uint8_t z = 0; z <= 9; z++)
    {
      for ( uint8_t x = 0; x <= 10; x++)
      {
        red = myanimation.frame[akt_aniframe].color[x][z].red * brightness * 0.0039;
        green = myanimation.frame[akt_aniframe].color[x][z].green * brightness * 0.0039;
        blue = myanimation.frame[akt_aniframe].color[x][z].blue * brightness * 0.0039;
        ledDriver.setPixelRGB(x, z, red, green, blue);
      }
    }
    ledDriver.show();
    webServer.handleClient();
    if ( myanimation.frame[akt_aniframe].delay > 200 )
    {
      delay (myanimation.frame[akt_aniframe].delay / 2);
      webServer.handleClient();
      delay (myanimation.frame[akt_aniframe].delay / 2);
    }
    else
    {
      delay (myanimation.frame[akt_aniframe].delay);
    }
    if ( myanimation.laufmode == 0 )
    {
      akt_aniframe++;
      if (  (myanimation.frame[akt_aniframe].delay == 0 || akt_aniframe > MAXFRAMES ))
      {
        akt_aniframe = 0;
        akt_aniloop++;
      }
    }
    if ( myanimation.laufmode == 1 )
    {
      akt_aniframe = akt_aniframe + frame_fak;
      if (myanimation.frame[akt_aniframe].delay == 0 || akt_aniframe > MAXFRAMES )
      {
        frame_fak = -1;
        akt_aniframe = akt_aniframe - 2;
        akt_aniloop++;
      }
      if (akt_aniframe == 0 ) {
        frame_fak = 1;
        akt_aniloop++;
      }
    }
    if ( myanimation.laufmode == 2 )
    {
      frame_fak++;
      for ( uint8_t i = 0; i <= 20; i++)
      {
        akt_aniframe = random(0, MAXFRAMES);
        if (myanimation.frame[akt_aniframe].delay != 0 ) break;
      }
      if ( frame_fak == 20 )
      {
        frame_fak = 0;
        akt_aniloop++;
      }
    }
    screenBufferNeedsUpdate = true;
    return true;
  }
}

void updateOutdoorWeather (void) {
  if (WiFi.isConnected() && (String(settings.mySettings.owApiKey) != ""))
  {
    // Get weather from OpenWeather
#ifdef APIKEY
#ifdef DEBUG
    Serial.println("Getting outdoor weather for location " + String(settings.mySettings.owLocation) + ":");
#endif
    !outdoorWeather.getOutdoorConditions(String(settings.mySettings.owLocation), String(settings.mySettings.owApiKey), LANGSTR) ? errorCounterOutdoorWeather++ : errorCounterOutdoorWeather = 0;
#ifdef DEBUG
    Serial.println("Location: " + String(settings.mySettings.owLocation));
    Serial.println("Weather description: " + String(outdoorWeather.description));
    Serial.println("Outdoor temperature: " + String(outdoorWeather.temperature));
    Serial.println("Outdoor humidity: " + String(outdoorWeather.humidity));
    Serial.println("Pressure: " + String(outdoorWeather.pressure));
    Serial.println("Sunrise: " + String(hour(timeZone.toLocal(outdoorWeather.sunrise))) + ":" + String(minute(timeZone.toLocal(outdoorWeather.sunrise))));
    Serial.println("Sunset: " + String(hour(timeZone.toLocal(outdoorWeather.sunset))) + ":" + String(minute(timeZone.toLocal(outdoorWeather.sunset))));
#endif
#endif
  }
}

void setupWPS (void) {
  WiFi.softAPdisconnect();
  WiFi.mode(WIFI_STA);
  Serial.println("Starting WPS.");
  setMode(MODE_WPS);
  startWps = true;
}

void setupWiFi (void) {
  renderer.clearScreenBuffer(matrix);
  renderer.setSmallText("WI", TEXT_POS_TOP, matrix);
  renderer.setSmallText("FI", TEXT_POS_BOTTOM, matrix);
  writeScreenBuffer(matrix, WHITE, brightness);
  WiFi.mode(WIFI_STA);
  Serial.println("Setting up WiFiManager.");
  WiFiManager wifiManager;
  wifiManager.setTimeout(WIFI_SETUP_TIMEOUT);
  wifiManager.autoConnect(HostName, WIFI_AP_PASS);
}

void postWiFiSetup (void) {
  if (!WiFi.isConnected())
  {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(HostNameAp);
    Serial.println("No WLAN connected. Switching to AP mode.");
    writeScreenBuffer(matrix, RED, brightness);
#if defined(BUZZER) && defined(WIFI_BEEPS)
    digitalWrite(PIN_BUZZER, HIGH);
    delay(1500);
    digitalWrite(PIN_BUZZER, LOW);
#endif
    delay(1000);
    myIP = WiFi.softAPIP();
  }
  else
  {
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
    Serial.println("WLAN connected. Switching to STA mode.");
    Serial.println("RSSI: " + String(WiFi.RSSI()));
    writeScreenBuffer(matrix, GREEN, brightness);
#if defined(BUZZER) && defined(WIFI_BEEPS)
    for (uint8_t i = 0; i <= 2; i++)
    {
#ifdef DEBUG
      Serial.println("Beep!");
#endif
      digitalWrite(PIN_BUZZER, HIGH);
      delay(100);
      digitalWrite(PIN_BUZZER, LOW);
      delay(100);
    }
#endif
    delay(1000);
    myIP = WiFi.localIP();

    // mDNS is needed to see HostName in Arduino IDE
    Serial.println("Starting mDNS responder.");
    MDNS.begin(HostName);
    //MDNS.addService("http", "tcp", 80);
  }
  renderer.clearScreenBuffer(matrix);
  screenBufferNeedsUpdate = true;

  updateOutdoorWeather();

#ifdef SHOW_IP
  WiFi.isConnected() ? feedText = "  IP: " : feedText = "  AP-IP: ";
  feedText += String(myIP[0]) + "." + String(myIP[1]) + "." + String(myIP[2]) + "." + String(myIP[3]) + "   ";
  feedPosition = 0;
  feedColor = WHITE;
  setMode(MODE_FEED);
#else
  setMode(MODE_TIME);
#endif
}
