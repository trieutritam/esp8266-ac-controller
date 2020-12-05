#include <Arduino.h>

#include "config_setup.h"



void loadConfigFile(JsonDocument &doc)
{
  String filename = "/config.json";
  File file = LittleFS.open(filename, "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  DeserializationError error = deserializeJson(doc, file);
  if (error) {
    Serial.println(F("Failed to read config file. Restart after 3s..."));
    delay(3000);
    ESP.restart();
  }

  file.close();

  Serial.printf("Config:\n %s \n", doc.as<String>().c_str());
}

void listDir(const char* dirname)
{
  Serial.printf("Reading config file: %s\n", dirname);

  Dir root = LittleFS.openDir(dirname);

  while (root.next()) {
    File file = root.openFile("r");
    Serial.print("  FILE: ");
    Serial.print(root.fileName());
    Serial.print("  SIZE: ");
    Serial.print(file.size());
    time_t cr = file.getCreationTime();
    time_t lw = file.getLastWrite();
    file.close();
    struct tm * tmstruct = localtime(&cr);
    Serial.printf("    CREATION: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
    tmstruct = localtime(&lw);
    Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
  }
}

void initFS() {
  Serial.println("Mount LittleFS");
  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed");
    return;
  }

  listDir("/");
}