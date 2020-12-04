#include <Arduino.h>
#include <assert.h>

#include "wifi_setup.h"
#include "mqtt_setup.h"
#include "irac_setup.h"

MQTTSetup mqtt;
IRACSetup irAcSetup;

String deviceId("f9919919");

void messageHandler(std::string, DynamicJsonDocument doc)
{
  const char *action = doc["action"];
  int id = doc["id"];

  Serial.printf("id %d, action: %s\n", id, action);
  if (strcmp(action, "ac.power") == 0)
  {
    bool isOn = doc["value"];
    irAcSetup.setOnOff(isOn);
  }
  else if (strcmp(action, "ac.temp") == 0)
  {
    uint8_t temperature = doc["value"];
    irAcSetup.setTemperature(temperature);
  }
  else if (strcmp(action, "ac.fan") == 0)
  {
    uint8_t fanMode = doc["value"];
    irAcSetup.setFanSpeed(fanMode);
  }

  if (strstr(action, "ac."))
  {
    irAcSetup.printState();
    irAcSetup.sendAc();
  }
}

void setup()
{
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  while (!Serial){
    ;
  }

  Serial.println("Starting...." + deviceId);

  initWifi();

  mqtt.init(deviceId);

  using std::placeholders::_1;
  using std::placeholders::_2;
  String dataTopic = "devices/" + deviceId + "/data";
  mqtt.registerCallback(dataTopic.c_str(), &messageHandler);

  irAcSetup.init(USE_AC_SHARP);
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