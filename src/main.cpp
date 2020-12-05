#include <Arduino.h>
#include <assert.h>

#include <ESP8266mDNS.h>


#include "config_setup.h"
#include "wifi_setup.h"
#include "mqtt_setup.h"
#include "irac_message_handler.h"

// #define MQTT_SERVER "192.168.2.103"
// #define MQTT_USER "admin"
// #define MQTT_PASS "admin"

StaticJsonDocument<512> configDoc;

MQTTSetup mqtt;

String deviceId("f9919919");

void setup()
{
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  while (!Serial){
    ;
  }

  Serial.println("\n");
  Serial.println("Starting.... Device Id:" + deviceId + " - " + String(ESP.getFlashChipId(), HEX));

  initFS();

  loadConfigFile(configDoc);

  initWifi(deviceId.c_str());

  if (!MDNS.begin("esp8266")) {
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.println("mDNS responder started");

  mqtt.init(deviceId, configDoc["mqtt"]["host"], configDoc["mqtt"]["user"], configDoc["mqtt"]["pass"]);

  irAcInit(configDoc["ac_type"]);

  String dataTopic = "devices/" + deviceId + "/data";
  mqtt.registerCallback(dataTopic.c_str(), &irAcMQTTMessageHandler);
}

void loop()
{
  if (!WiFi.isConnected())
  {
    ESP.restart();
  }
  else
  {
    mqtt.connectAndReceive();
  }

  MDNS.update();

  // Serial.println("Send ....");
  // irAcSetup.sendAc();
  // delay(1000);

  // irAcSetup.debugIRReceived();
}