#ifndef __CONFIG_FILE_SETUP__
#define __CONFIG_FILE_SETUP__

#include <LittleFS.h>
#include <ArduinoJson.h>


void loadConfigFile(JsonDocument &doc);
void initFS();

#endif