#ifndef __WIFI_SETUP__
#define __WIFI_SETUP__

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  

#define AP_NAME "ESP8266_NODEMCU"
#define AP_PASSWORD "12345678"

//for LED status
#include <Ticker.h>

void initWifi(const char* deviceId);


#endif