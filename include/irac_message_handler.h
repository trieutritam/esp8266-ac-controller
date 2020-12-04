#ifndef __IRAC_MESSAGE_HANDLER__
#define __IRAC_MESSAGE_HANDLER__

#include <ArduinoJson.h>

#include "irac_setup.h"

void irAcInit(uint8_t type);

void irAcMQTTMessageHandler(std::string, DynamicJsonDocument doc);


#endif