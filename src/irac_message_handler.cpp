#include "irac_message_handler.h"

IRACSetup irAcSetup;

void irAcInit(uint8_t type) {
    irAcSetup.init(type);
}


void irAcMQTTMessageHandler(std::string, DynamicJsonDocument doc)
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