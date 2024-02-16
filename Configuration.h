//*****************************************************************************
// Configuration.h
// See README.txt for help
//*****************************************************************************

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

//*****************************************************************************
// Software settings
//*****************************************************************************

#define PRODUCT_NAME "zytQuadrat"
#define WEBSITE_TITLE "zytQuadrat"
//#define DEDICATION "The only reason for time is so that everything doesn't happen at once.<br>(Albert Einstein)"
#define WIFI_SETUP_TIMEOUT 30
#define WIFI_CONNECT_TIMEOUT 15000
#define BRIGHTNESS_TIMEOUT 1000
#define WIFI_AP_PASS "12345678"
#define OTA_PASS "1234"
#define NTP_DEFAULT_SERVER "pool.ntp.org"
#define SHOW_IP
//#define WIFI_BEEPS

//#define NONE_TECHNICAL_ZERO
#define AUTO_MODECHANGE_TIME 60 // seconds
#define ALARM_LED_COLOR RED
//#define ABUSE_CORNER_LED_FOR_ALARM

//#define POWERON_SELFTEST
//#define SHOW_MODE_AMPM
#define SHOW_MODE_SECONDS
#define SHOW_MODE_WEEKDAY
#define SHOW_MODE_DATE
//#define SHOW_MODE_MOONPHASE
#define SHOW_MODE_SUNRISE_SUNSET // works only if APIKEY is defined
//#define SHOW_MODE_TEST
#define SHOW_MODE_SETTINGS

#define APIKEY
#define DEFAULT_LOCATION "Bern, CH"

//--------------
#define MOOD_INTERVAL_MIN 300
#define MOOD_INTERVAL_MAX 4000

#define MIN_ANI_DURATION 2500

#ifdef SHOW_MODE_SETTINGS
#define SETTINGS_TITLE_TIMEOUT 2500
#endif

//#define LANGUAGE_EN
#define LANGUAGE_DE
//#define LANGUAGE_FR
//#define LANGUAGE_IT
//#define LANGUAGE_ES
//#define LANGUAGE_NL

//*****************************************************************************
// Timezone
//*****************************************************************************

//#define TIMEZONE_IDLW  // IDLW  International Date Line West UTC-12
//#define TIMEZONE_SST   // SST   Samoa Standard Time UTC-11
//#define TIMEZONE_HST   // HST   Hawaiian Standard Time UTC-10
//#define TIMEZONE_AKST  // AKST  Alaska Standard Time UTC-9
//#define TIMEZONE_USPST // USPST Pacific Standard Time (USA) UTC-8
//#define TIMEZONE_USMST // USMST Mountain Standard Time (USA) UTC-7
//#define TIMEZONE_USAZ  // USAZ  Mountain Standard Time (USA) UTC-7 (no DST)
//#define TIMEZONE_USCST // USCST Central Standard Time (USA) UTC-6
//#define TIMEZONE_USEST // USEST Eastern Standard Time (USA) UTC-5
//#define TIMEZONE_AST   // AST   Atlantic Standard Time UTC-4
//#define TIMEZONE_BST   // BST   Eastern Brazil Standard Time UTC-3
//#define TIMEZONE_VTZ   // VTZ   Greenland Eastern Standard Time UTC-2
//#define TIMEZONE_AZOT  // AZOT  Azores Time UTC-1
//#define TIMEZONE_GMT   // GMT   Greenwich Mean Time UTC
#define TIMEZONE_CET     // CET   Central Europe Time UTC+1
//#define TIMEZONE_EST   // EST   Eastern Europe Time UTC+2
//#define TIMEZONE_MSK   // MSK   Moscow Time UTC+3 (no DST)
//#define TIMEZONE_GST   // GST   Gulf Standard Time UTC+4
//#define TIMEZONE_PKT   // PKT   Pakistan Time UTC+5
//#define TIMEZONE_BDT   // BDT   Bangladesh Time UTC+6
//#define TIMEZONE_JT    // JT    Java Time UTC+7
//#define TIMEZONE_CNST  // CNST  China Standard Time UTC+8
//#define TIMEZONE_HKT   // HKT   Hong Kong Time UTC+8
//#define TIMEZONE_PYT   // PYT   Pyongyang Time (North Korea) UTC+8.5
//#define TIMEZONE_CWT   // CWT   Central West Time (Australia) UTC+8.75
//#define TIMEZONE_JST   // JST   Japan Standard Time UTC+9
//#define TIMEZONE_ACST  // ACST  Australian Central Standard Time UTC+9.5
//#define TIMEZONE_AEST  // AEST  Australian Eastern Standard Time UTC+10
//#define TIMEZONE_LHST  // LHST  Lord Howe Standard Time UTC+10.5
//#define TIMEZONE_SBT   // SBT   Solomon Islands Time UTC+11
//#define TIMEZONE_NZST  // NZST  New Zealand Standard Time UTC+12

//*****************************************************************************
// Hardware settings
//*****************************************************************************

//#define BOARD_DEFAULT
#define BOARD_QWF_V1_2
//#define BOARD_QWF_MINI_V1_1
//#define BOARD_QWF_V2_0

#ifdef BOARD_DEFAULT
#define ESP_LED
//#define SENSOR_DHT22
//#define SENSOR_MCP9808
//#define SENSOR_BME280
//#define RTC_BACKUP
//#define LDR
//#define BUZZER
//#define IR_RECEIVER

//#define ONOFF_BUTTON
//#define MODE_BUTTON
//#define TIME_BUTTON
//#define PLUS_BUTTON
//#define MINUS_BUTTON

// Pin Definition ESP8266 (Wemos D1 mini)
#define PIN_IR_RECEIVER  16 // D0 (no interrupt)
//#define PIN_WIRE_SCL     05 // D1 SCL
//#define PIN_WIRE_SDA     04 // D2 SDA
#define PIN_MODE_BUTTON  00 // D3 LOW_Flash
#define PIN_LED          02 // D4 ESP8266_LED
#define PIN_BUZZER       14 // D5
#define PIN_DHT22        12 // D6
#define PIN_LEDS_CLOCK   13 // D7
#define PIN_LEDS_DATA    15 // D8
#define PIN_LDR          A0 // ADC
#define PIN_TIME_BUTTON  01 // TXD0
#define PIN_ONOFF_BUTTON 03 // RXD0
// GPIO 06 to GPIO 11 are
// used for flash memory databus

//#define LED_LAYOUT_HORIZONTAL_1
#define LED_LAYOUT_VERTICAL_1
//#define LED_LAYOUT_VERTICAL_2
//#define LED_LAYOUT_VERTICAL_3
//#define LED_LAYOUT_VERTICAL_4

#define NEOPIXEL_RGB
//#define NEOPIXEL_RGBW

#define NEOPIXEL_TYPE NEO_GRB + NEO_KHZ800     // see Adafruit_NeoPixel.h for help
//#define NEOPIXEL_TYPE NEO_WRGB + NEO_KHZ800
//#define NEOPIXEL_TYPE NEO_GRBW + NEO_KHZ800
#endif // BOARD_DEFAULT

// QlockWiFive Board v1.2
#ifdef BOARD_QWF_V1_2
#define ESP_LED
#define RTC_BACKUP
#define LDR
#define BUZZER
#define IR_RECEIVER

//#define ONOFF_BUTTON
#define MODE_BUTTON
//#define TIME_BUTTON
#define PLUS_BUTTON
#define MINUS_BUTTON

// Pin Definition ESP8266 (Wemos D1 mini)
#define PIN_IR_RECEIVER  12 // D6 
#define PIN_MODE_BUTTON  16 // D0 (no interrupt)
#define PIN_LED          02 // D4 ESP8266_LED
#define PIN_BUZZER       15 // D8
#define PIN_LEDS_CLOCK   14 // D5
#define PIN_LEDS_DATA    13 // D7
#define PIN_LDR          A0 // ADC
//#define PIN_TIME_BUTTON  01 // TXD0
//#define PIN_ONOFF_BUTTON 03 // RXD0
#define PIN_MINUS_BUTTON 03 // RXD0
#define PIN_PLUS_BUTTON 01 // TXD0
// GPIO 06 to GPIO 11 are
// used for flash memory databus

//#define LED_LAYOUT_HORIZONTAL_1
#define LED_LAYOUT_VERTICAL_1
//#define LED_LAYOUT_VERTICAL_2
//#define LED_LAYOUT_VERTICAL_3
//#define LED_LAYOUT_VERTICAL_4
//#define LED_LAYOUT_VERTICAL_4_XXL

#define NEOPIXEL_RGB
//#define NEOPIXEL_RGBW

#define NEOPIXEL_TYPE NEO_GRB + NEO_KHZ800     // see Adafruit_NeoPixel.h for help
//#define NEOPIXEL_TYPE NEO_WRGB + NEO_KHZ800
//#define NEOPIXEL_TYPE NEO_GRBW + NEO_KHZ800
#endif // BOARD_QWF_V1_2


// QlockWiFive Board Mini v1.1
#ifdef BOARD_QWF_MINI_V1_1
#define ESP_LED
#define RTC_BACKUP
#define LDR
#define IR_RECEIVER
//#define BUZZER

//#define ONOFF_BUTTON
//#define MODE_BUTTON
//#define TIME_BUTTON
//#define PLUS_BUTTON
//#define MINUS_BUTTON

// Pin Definition ESP8266 (Wemos D1 mini)
#define PIN_IR_RECEIVER  12 // D6 (no interrupt)
//#define PIN_MODE_BUTTON  02 // D4 LOW_Flash
#define PIN_LED          02 // D4 ESP8266_LED
#define PIN_LEDS_DATA    13 // D7
#define PIN_LDR          A0 // ADC
#define PIN_BUZZER       15 // D8
//#define PIN_TIME_BUTTON  01 // TXD0
//#define PIN_ONOFF_BUTTON 03 // RXD0
//#define PIN_MINUS_BUTTON  16 // D0
//#define PIN_PLUS_BUTTON 14 // D5
// GPIO 06 to GPIO 11 are
// used for flash memory databus

//#define LED_LAYOUT_HORIZONTAL_1
//#define LED_LAYOUT_VERTICAL_1
//#define LED_LAYOUT_VERTICAL_2
//#define LED_LAYOUT_VERTICAL_3
//#define LED_LAYOUT_VERTICAL_4
//#define LED_LAYOUT_VERTICAL_4_XXL
#define LED_LAYOUT_VERTICAL_5

#define NEOPIXEL_RGB
//#define NEOPIXEL_RGBW

//#define NEOPIXEL_TYPE NEO_GRB + NEO_KHZ800     // see Adafruit_NeoPixel.h for help
//#define NEOPIXEL_TYPE NEO_WRGB + NEO_KHZ800
#define NEOPIXEL_TYPE NEO_GRBW + NEO_KHZ800
#endif // BOARD_QWF_MINI_V1_1

// QlockWiFive Board v2.0
#ifdef BOARD_QWF_V2_0
#define SENSOR_MCP9808
#define SENSOR_BME280
#define RTC_BACKUP
#define LDR
#define IR_RECEIVER

//#define ONOFF_BUTTON
#define MODE_BUTTON
//#define TIME_BUTTON
#define PLUS_BUTTON
#define MINUS_BUTTON

// Pin Definition ESP8266
#define PIN_IR_RECEIVER  12
#define PIN_MODE_BUTTON  02
#define PIN_LEDS_DATA    13
#define PIN_LDR          A0
//#define PIN_TIME_BUTTON  14
//#define PIN_ONOFF_BUTTON 00
#define PIN_MINUS_BUTTON  14
#define PIN_PLUS_BUTTON 00

//#define LED_LAYOUT_HORIZONTAL_1
//#define LED_LAYOUT_VERTICAL_1
//#define LED_LAYOUT_VERTICAL_2
//#define LED_LAYOUT_VERTICAL_3
#define LED_LAYOUT_VERTICAL_4

#define NEOPIXEL_RGB
//#define NEOPIXEL_RGBW

#define NEOPIXEL_TYPE NEO_GRB + NEO_KHZ800     // see Adafruit_NeoPixel.h for help
//#define NEOPIXEL_TYPE NEO_WRGB + NEO_KHZ800
//#define NEOPIXEL_TYPE NEO_GRBW + NEO_KHZ800

#endif // BOARD_QWF_V2_0

#define SERIAL_SPEED 115200

#define NUMPIXELS_X 11
#define NUMPIXELS_Y 10
#define NUMPIXELS_CORNERS 4

#define PIXEL_NO_CORNER_1 NUMPIXELS_X*NUMPIXELS_Y
#define PIXEL_NO_CORNER_2 PIXEL_NO_CORNER_1 + 1
#define PIXEL_NO_CORNER_3 PIXEL_NO_CORNER_2 + 1
#define PIXEL_NO_CORNER_4 PIXEL_NO_CORNER_3 + 1
#define PIXEL_NO_ALARM PIXEL_NO_CORNER_4 + 1     

#define MIN_BRIGHTNESS 20
#define MAX_BRIGHTNESS 255
#define TEST_BRIGHTNESS 80

#ifdef SENSOR_DHT22
#define DHT_TEMPERATURE_OFFSET 0.5
#define DHT_HUMIDITY_OFFSET -2.0
#endif

#ifdef SENSOR_BME280
#define BME_TEMPERATURE_OFFSET 0.0
#define BME_HUMIDITY_OFFSET 0.0
#endif

#ifdef SENSOR_MCP9808
#define MCP_TEMPERATURE_OFFSET -0.1
#endif

#ifdef RTC_BACKUP
#define RTC_TEMPERATURE_OFFSET -3.0
#endif

#ifdef LDR
//#define LDR_IS_INVERSE
#define LDR_MIN_DEFAULT 99 // The ESP will crash if LDR_MIN_DEFAULT and LDR_MAX_DEFAULT are equal due to an error in map()
#define LDR_MAX_DEFAULT 100
#define LDR_HYSTERESIS 10
#define LDR_X_DEFAULT 7
#define LDR_Y_DEFAULT 9
#endif

#ifdef BUZZER
#define BUZZTIME_ALARM_1 30
#define BUZZTIME_ALARM_2 30
#define BUZZTIME_TIMER 30
#endif

#ifdef IR_RECEIVER
#define IR_CODE_ONOFF        (0xFFE01F) // QWF V1.0 Remote Power
#define IR_CODE_TIME         (0xFFA05F) // QWF V1.0 Remote Time
#define IR_CODE_SETTINGS     (0xFFC837) // QWF V1.0 Remote Mode
#define IR_CODE_PLUS         (0xFF58A7) // QWF V1.0 Remote Bright. Plus
#define IR_CODE_MINUS        (0xFF48B7) // QWF V1.0 Remote Bright. Minus

#define IR_CODE_MODE         (0xFFD827) // QWF V1.0 Remote Mode
#define IR_CODE_SECONDS      (0xFF20DF) // QWF V1.0 Remote Seconds
#define IR_CODE_DATE         (0xFF609F) // QWF V1.0 Remote Date

#define IR_CODE_RED          (0xFF906F) // QWF V1.0 Remote Red
#define IR_CODE_GREEN        (0xFF10EF) // QWF V1.0 Remote Green
#define IR_CODE_DARKBLUE     (0xFF50AF) // QWF V1.0 Remote Darkblue
#define IR_CODE_PINK         (0xFFD02F) // QWF V1.0 Remote Pink
#define IR_CODE_WHITE        (0xFFB04F) // QWF V1.0 Remote White
#define IR_CODE_BRIGHTBLUE   (0xFF30CF) // QWF V1.0 Remote Brightblue
#define IR_CODE_YELLOW       (0xFF708F) // QWF V1.0 Remote Yellow
#define IR_CODE_ORANGE       (0xFFF00F) // QWF V1.0 Remote RGB

#define IR_CODE_NORMAL       (0xFF9867) // QWF V1.0 Remote Normal
#define IR_CODE_FADE         (0xFF18E7) // QWF V1.0 Remote Fade
#define IR_CODE_MATRIX       (0xFF8877) // QWF V1.0 Remote Matrix
#define IR_CODE_SLIDE        (0xFF08F7) // QWF V1.0 Remote Slide

#define IR_CODE_MOOD         (0xFFA857) // QWF V1.0 Remote Mood
#define IR_CODE_5MIN         (0xFF28D7) // QWF V1.0 Remote 5min
#define IR_CODE_1H           (0xFF6897) // QWF V1.0 Remote 1h
#define IR_CODE_24H          (0xFFE817) // QWF V1.0 Remote 24h

//#define IR_LETTER_OFF
#define IR_LETTER_X 6 // left column = 0
#define IR_LETTER_Y 9 // top row = 0

#define IR_DEBOUNCE_T 250
#endif

//*****************************************************************************
// Misc
//*****************************************************************************

//#define DEBUG
//#define DEBUG_WEB
//#define DEBUG_IR
//#define DEBUG_MATRIX
//#define DEBUG_FPS

//#define SYSLOGSERVER_SERVER "192.168.0.1"
//#define SYSLOGSERVER_PORT 514

//#define UPDATE_INFOSERVER "thorsten-wahl.ch"
//#define UPDATE_INFOFILE "/qlockwork/updateinfo.json"

#define __NAME__ (strrchr(__FILE__,'\\')?strrchr(__FILE__,'\\')+1:__FILE__)
#endif
