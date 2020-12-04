#include "wifi_setup.h"

Ticker ticker;
WiFiManager wifiManager;


void tick()
{
  //toggle state
  int state = digitalRead(LED_BUILTIN); // get the current state of GPIO1 pin
  digitalWrite(LED_BUILTIN, !state);    // set pin to the opposite state
}

//gets called when WiFiManager enters configuration mode
void configModeCallback(WiFiManager *myWiFiManager)
{
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
  ticker.attach(0.2, tick);
}

void initWifi()
{
  pinMode(LED_BUILTIN, OUTPUT);
  ticker.attach(0.6, tick);

  wifiManager.setAPCallback(configModeCallback);

  Serial.println("Connecting...");
  if (!wifiManager.autoConnect(AP_NAME, AP_PASSWORD))
  {
    Serial.println("Failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }

  //if you get here you have connected to the WiFi
  Serial.printf("Wifi connected...IP: %s\n", WiFi.localIP().toString().c_str());
  ticker.detach();
  //keep LED on
  ticker.attach(1, tick);
}