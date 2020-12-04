#include <map> 

#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <ArduinoJson.h>


#define MESSAGE_CALLBACK_SIGNATURE std::function<void(std::string, DynamicJsonDocument)>
typedef std::map<std::string, MESSAGE_CALLBACK_SIGNATURE> callback_map;


class MQTTSetup {
private:
  String  _clientId;
  WiFiClient wifiClient;
  PubSubClient pubSubClient;
  Ticker lwtTicker;

  callback_map _messageCallbacks;

  void _reconnect();
  void _messageHandler(char*, unsigned char*, unsigned int);

public:
  MQTTSetup();
  ~MQTTSetup();

  void init(const String deviceId);
  void connectAndReceive();
  MESSAGE_CALLBACK_SIGNATURE registerCallback(std::string topic, MESSAGE_CALLBACK_SIGNATURE);
  MESSAGE_CALLBACK_SIGNATURE unregisterCallbac(std::string topic);
};