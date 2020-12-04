#include <Arduino.h>
#include <assert.h>

#include "wifi_setup.h"
#include "mqtt_setup.h"
#include "irac_message_handler.h"

#define MQTT_SERVER "192.168.2.103"
#define MQTT_USER "admin"
#define MQTT_PASS "admin"


MQTTSetup mqtt;

String deviceId("f9919919");

void setup()
{
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  while (!Serial){
    ;
  }

  Serial.println("\n");
  Serial.println("Starting...." + deviceId);

  initWifi();

  mqtt.init(deviceId, MQTT_SERVER, MQTT_USER, MQTT_PASS);

  irAcInit(USE_AC_SHARP);

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

  // Serial.println("Send ....");
  // irAcSetup.sendAc();
  // delay(1000);

  // irAcSetup.debugIRReceived();
}