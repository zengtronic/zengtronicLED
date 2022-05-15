/********************************************
 *      ZENGTRONIC WS2812 LED CONTROL
 *       Written By Stefan Zengerle
 *******************************************/
//Defines for features. Uncomment to disable
#define HWBTNS
#define SOUNDREACTIVE
#define USE_ESP32
//#define DEBUG

#include <FastLED.h>
#include <EEPROM.h>
#ifdef USE_ESP32
#define ADC_RESOLUTION 4096
#define A0_IN 36
#include <WiFi.h>
#include <WebServer.h>
//#include <ESP32httpUpdate.h>
#else
#define ADC_RESOLUTION 1024
#define A0_IN A0
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
//#include <ESP8266HTTPUpdateServer.h>
#endif

//Sound things
#include <MegunoLink.h>
#include <Filter.h>
ExponentialFilter<long> ADCFilter(30, 0);

//#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#define LED_PIN 14
#define COLOR_ORDER GRB
#define CHIPSET WS2812B
#define EEPROMSIZE 4096
#define EEPROMOFFSET 128
#define IDENTIFIER 812
#define FASTLED_ALLOW_INTERRUPTS 0

//Strings for Web
const WiFiMode_t wifiModes[] = {WIFI_STA, WIFI_AP, WIFI_AP_STA};
const String wifiModesStrings[] = {"Client", "Access Point", "Client and AP"};

const String modes[] = {
    "solid",
    "solid blink",
    "KITT",
    "KITT Rainbow",
    "KITT Rainbow Slow",
    "SoundLevel",
    "SoundLevel Rainbow",
    "SoundLevel Centered",
    "SoundLevel Centered Rainbow",
    "SoundSolid",
    "SoundSolid Rainbow",
    "SoundFlow",
    "SoundFlow Rainbow",
    "SoundFlow Centered",
    "SoundFlow Centered Rainbow",
    "Moving Palette Smooth",
    "Moving Palette Hard",
    "Solid Palette Smooth",
    "Solid Palette Hard",
    "Pacifica",
    "discostrobe"};
const String palettes[] = {
    "Rainbow",
    "Rainbow Stripe",
    "Heat",
    "Lava",
    "Forest",
    "Cloud",
    "Ocean",
    "Party",
    "Black / White",
    "Red White Blue",
    "Red White Green",
    "RGB",
    "CMY",
    "Rastafari",
    "Red Yellow",
    "Police USA",
    "Police DE",
    "Random",
    "Palette Knife 1",
    "Palette Knife 2",
    "Palette Knife 3"};
const ColorTemperature colorTemp[] = {DirectSunlight, Candle, Tungsten40W, Tungsten100W, Halogen, CarbonArc, HighNoonSun, OvercastSky, ClearBlueSky, WarmFluorescent, StandardFluorescent, CoolWhiteFluorescent, FullSpectrumFluorescent, GrowLightFluorescent, BlackLightFluorescent, MercuryVapor, SodiumVapor, MetalHalide, HighPressureSodium};
const String colorTempStrings[] = {"Normal / Direct Sunlight (6000K)", "Candle (1900K)", "Tungsten 40W (2600K)", "Tungsten 100W (2850K)", "Halogen (3200K)", "Carbon Arc (5200K)", "High Noon Sun (5400K)", "Overcast Sky (7000K)", "Clear Blue Sky (20000K)", "Warm Fluorescent", "Standard Fluorescent", "Cool White Fluorescent", "Full Spectrum Fluorescent", "Grow Light Fluorescent", "Black Light Fluirescent", "Mercury Vapor", "Sodium Vapor", "Metal Halide", "High Pressure Sodium"};

//---------- Environment Vars
int num_leds = 60;
bool leds_odd = (num_leds % 2);
uint8_t frames_per_second = 30;
uint8_t max_brightness = 192;
bool power_mngmt = false;
uint16_t max_current = 5000;

//for sound input
uint8_t ain_adjustment = 25;   //how much % counts as 100% in sound visulations
uint16_t ain_noise_floor = 20; //floor noise (German: Grundrauschen) which is ignored in sound visulations
boolean ain_reverse = false;   // Flip input value?

//----------- WiFi Vars
uint8_t wifimode = 0;
String ssid1 = ""; //WiFi 1
String password1 = "";
String ssid2 = ""; //WiFi 2
String password2 = "";
String apssid = "";     //SSID of the AP
String appassword = ""; //Password of the AP

//to get saved
uint8_t t_wifimode = 0;
String t_ssid1 = ""; //WiFi 1
String t_password1 = "";
String t_ssid2 = ""; //WiFi 2
String t_password2 = "";
String t_apssid = "";     //SSID of the AP
String t_appassword = ""; //Password of the AP

//-------------- States
bool power = false;
uint8_t mode = 0;
uint8_t speed = 1;
uint8_t color = 0;
uint8_t saturation = 255;
uint8_t palette = 0;
uint8_t extraparam = 80;
uint8_t currentTemp = 0;
CRGBPalette256 currentPalette;
TBlendType currentBlending;
int sound_in = 0;

//Status Updates per Console
int oldWifiStatus = 0;

//Counts
const uint8_t modeCount = 21;
const uint8_t paletteCount = 21;
const uint8_t colorTempCount = 20;
const uint8_t wifiModeCount = 3;

#ifdef USE_ESP32
WebServer httpServer(80);
#else
ESP8266WebServer httpServer(80);
#endif
//ESP8266HTTPUpdateServer httpUpdater(true);
CRGB leds[1024];

bool reverseDir = true; //sometimes it makes sense to reverse the palette animation direction
uint8_t gHue = 0;
static uint8_t startIndex = 0;

#include "owneeprom.h"
#include "palettes.h"
#include "functions.h"
#include "effects.h"
#include "web.h"

unsigned long SpreviousMillis = 0;

#ifdef HWBTNS
const int wifiResetPin = 15;         //GPIO of the wifi reset button
const int powerBtnPin = 13;          //GPIO of the power button
unsigned long btnDebounceTime = 100; //debounce time in ms
unsigned long powerBtnLastDebounceTime = 0;
unsigned long wifiResetLastDebounceTime = 0;
unsigned long wifiResetLastPress = 0;
unsigned long wifiResetDelay = 2000; //Time in which the second press musst occur.

// Wifi Reset Button Interrupt Routine.
ICACHE_RAM_ATTR void wifiResetInterrupt()
{
  unsigned long triggerTime = millis();
  if ((triggerTime - wifiResetLastDebounceTime) > btnDebounceTime)
  {
    if ((triggerTime - wifiResetLastPress) < wifiResetDelay)
    {
      ESP.restart();
      wifiResetLastPress = 0;
    }
    else
    {
      wifiResetLastPress = triggerTime;
    }
  }
  wifiResetLastDebounceTime = triggerTime;
}

// PowerButton Interrupt Routine.
ICACHE_RAM_ATTR void powerBtnInterrupt()
{
  unsigned long triggerTime = millis();
  if ((triggerTime - powerBtnLastDebounceTime) > btnDebounceTime)
  {
    switchPower();
  }
  powerBtnLastDebounceTime = triggerTime;
}
#endif

void setup()
{
#ifdef HWBTNS
  attachInterrupt(digitalPinToInterrupt(wifiResetPin), wifiResetInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(powerBtnPin), powerBtnInterrupt, FALLING);
#endif
  Serial.begin(115200);
  EEPROM.begin(EEPROMSIZE);
  eepromDumpInt();
  delay(1000); // sanity delay
  loadConfig();
  leds_odd = (num_leds % 2);
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, num_leds).setCorrection(TypicalLEDStrip);
  switchPalette();
  currentBlending = LINEARBLEND;

  if (ssid1 == "")
  {
    if (apssid == "")
    {
      WiFi.mode(WIFI_AP);
      WiFi.softAP("Zengtronic LED");
    }
    else
    {
      WiFi.mode(WIFI_AP);
      WiFi.softAP(apssid.c_str(), appassword.c_str());
    }
  }
  else
  {
    switch (wifimode)
    {
    case 0:
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid1.c_str(), password1.c_str());
      break;
    case 1:
      if (apssid == "")
      {
        apssid = "Zengtronic LED";
      }
      WiFi.mode(WIFI_AP);
      WiFi.softAP(apssid.c_str(), appassword.c_str());
      break;
    case 2:
      WiFi.mode(WIFI_AP_STA);
      WiFi.begin(ssid1.c_str(), password1.c_str());
      WiFi.softAP(apssid.c_str(), appassword.c_str());
      break;
    }
  }

  //  WiFi.begin(ssid, password);
  //Webserver Handles
  //httpUpdater.setup(&httpServer);
  httpServer.on("/", handleRoot);
  httpServer.on("/mode", handleMode);
  httpServer.on("/brightness", handleBrightness);
  httpServer.on("/color", handleColor);
  httpServer.on("/palette", handlePalette);
  httpServer.on("/saturation", handleSaturation);
  httpServer.on("/speed", handleSpeed);
  httpServer.on("/extra", handleExtra);
  httpServer.on("/temperature", handleTemperature);
  httpServer.on("/power", handlePower);
  httpServer.on("/info", handleInfo);
  httpServer.on("/save", handleSave);
  httpServer.on("/settings", handleSettings);
  httpServer.on("/firmware", handleFirmwareUpdate);
  httpServer.on("/slight", handleSettingsLight);
  httpServer.on("/ssys", handleSettingsSystem);
  httpServer.on("/ssound", handleSettingsSound);
  httpServer.on("/swifi", handleSettingsWifi);
  httpServer.on("/sss", handleSettingsSave);
  httpServer.on("/restart", handleRestart);
  //Soundconfig
  httpServer.on("/ssound", handleSettingsSound);
  httpServer.on("/aina", handleAINadjustment);
  httpServer.on("/ainn", handleAINnoise);
  httpServer.on("/ainr", handleAINmirror);
  httpServer.onNotFound(handleNotFound);
  httpServer.begin();

  if (leds_odd)
  {
    Serial.println("Odd LED amount");
  }
  else
  {
    Serial.println("Even LED amount");
  }
  analogSetCycles(50);
}

void loop()
{
  httpServer.handleClient();
  handleProgramLogic();
  handleAutomaticSaving();
  handleStatusUpdates();
  //if(mode == 3){
  //handleSoundVisu();
  //}
}

void handleSoundVisu()
{
  unsigned long ScurrentMillis = millis();
  int interval = 5;
  if (ScurrentMillis - SpreviousMillis >= interval)
  {
    SpreviousMillis = ScurrentMillis;
    // currently empty
  }
}

void handleAutomaticSaving()
{
  EVERY_N_MILLISECONDS(300000)
  {
    Serial.println("--- Autosave ---");
    saveConfig();
  }
}

void handleProgramLogic()
{
  switchPalette();
  if (power == true)
  {
    switch (mode)
    {
      case 0:
        colorSolid(CHSV(color, saturation, 255));
        break;

      case 1:
        colorSolidBlink(CHSV(color, saturation, 255));
        break;

      case 2:
        kitt(color, false, 1, extraparam);
        break;

      case 3:
        kitt(color, true, (int)(256 / num_leds), extraparam);
        break;

      case 4:
        kitt(color, true, 1, extraparam);
        break;

      case 5:
        process_sound_in();
        soundLevel();
        break;

      case 6:
        process_sound_in();
        soundLevel(true);
        break;

      case 7:
        process_sound_in();
        soundLevelCentered();
        break;

      case 8:
        process_sound_in();
        soundLevelCentered(true);
        break;

      case 9:
        process_sound_in();
        soundSolid();
        break;

      case 10:
        process_sound_in();
        soundSolid(true);
        break;

      case 11:
        process_sound_in();
        soundFlow();
        break;

      case 12:
        process_sound_in();
        soundFlow(true);
        break;

      case 13:
        process_sound_in();
        soundFlowCentered();
        break;

      case 14:
        process_sound_in();
        soundFlowCentered(true);
        break;

      case 15:
        movingPalette(currentPalette, true);
        break;

      case 16:
        movingPalette(currentPalette, false);
        break;

      case 17:
        playPalette(currentPalette, 50,0,true);
        break;

      case 18:
        playPalette(currentPalette, 50,0,false);
        break;
      
      case 19:
        pacifica();
        break;

      case 20:
        discostrobe();
        break;
    }
  }
  else
  {
    fadeall();
  }
  FastLED.show();
}

void handleStatusUpdates()
{
  EVERY_N_MILLISECONDS(500)
  {
    if (WiFi.status() != oldWifiStatus)
    {
      switch (WiFi.status())
      {
      case 0:
        Serial.println("Status: Idle");
        break;
      case 1:
        Serial.println("Status: No SSID Available");
        break;
      case 2:
        Serial.println("Status: Scan Complete");
        break;
      case 3:
        Serial.print("Status: Connected. IP: ");
        Serial.print(WiFi.localIP());
        Serial.print(" @ SSID: ");
        Serial.println(WiFi.SSID());
        break;
      case 4:
        Serial.println("Status: Failed to Connect");
        break;
      case 5:
        Serial.println("Status: Connection Lost");
        break;
      case 6:
        Serial.println("Status: Disconnected");
        break;
      }
      oldWifiStatus = WiFi.status();
    }
  }
}
