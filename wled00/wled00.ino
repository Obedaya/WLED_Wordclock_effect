#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <Hash.h>
#include <NeoPixelBus.h>
#include <FS.h>

/*
 * @title WLED project sketch
 * @version 0.3pd
 * @author Christian Schwinne
 */
//Default CONFIG
String clientssid = "Your_Network_Here";
String clientpass = "Dummy_Pass";
String cmdns = "led";
String apssid = "WLED-AP";
String appass = "wled1234";
uint8_t apchannel = 1;
uint8_t aphide = 0;
boolean useap = true;
IPAddress staticip(0, 0, 0, 0);
IPAddress staticgateway(0, 0, 0, 0);
IPAddress staticsubnet(255, 255, 255, 0);
byte col[]{255, 127, 0};
boolean fadeTransition = true;
boolean seqTransition = false;
uint16_t transitionDelay = 1500;
boolean ota_lock = false;
boolean only_ap = false;
uint8_t led_amount = 16;
uint8_t buttonPin = 3; //needs pull-up
boolean buttonEnabled = true;
String notifier_ips[]{"10.10.1.128","10.10.1.129"};
boolean notifyDirect = true, notifyButton = true, notifyForward = true;
boolean receiveNotifications = true;
uint8_t bri_n = 100;
uint8_t nightlightDelayMins = 60;
boolean nightlightFade = true;

double transitionResolution = 0.01;

//Internal vars
byte col_old[]{0, 0, 0};
byte col_t[]{0, 0, 0};
long transitionStartTime;
long nightlightStartTime;
float tper_last = 0;
byte bri = 127;
byte bri_old = 0;
byte bri_t = 0;
byte bri_last = 127;
boolean transitionActive = false;
boolean buttonPressedBefore = false;
int notifier_ips_count = 0;
String notifier_ips_raw = "";
boolean nightlightActive = false;


NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart800KbpsMethod> strip(led_amount, 1);

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

File fsUploadFile;

void down()
{
  bri_t = 0;
  setAllLeds();
  Serial.println("MODULE TERMINATED");
  while (1) {delay(1000);}
}

void reset()
{
  bri_t = 0;
  setAllLeds();
  Serial.println("MODULE RESET");
  ESP.reset();
}

uint8_t bool2int(boolean value)
{
  if (value) return 1;
  return 0;
}

void handleNightlight()
{
  if (nightlightActive)
  {
    float nper = (millis() - nightlightStartTime)/(float)(((int)nightlightDelayMins)*60000);
    if (nper >= 1)
    {
      
    }
  }
}

void setup() {
    wledInit();
}

void loop() {
    server.handleClient();
    handleTransitions();
    handleNightlight();
    handleAnimations();
    handleButton();
}

