#include <functional>
#include <IPAddress.h>
#include "mqtt_setup.h"

#define STATUS_ON "ON"
#define STATUS_OFF "OFF"

#define LWT_TOPIC "devices/{DEVICE_ID}/LWT"
#define LWT_INTERVAL 10000



void mqtt_tick()
{
  // if (mqttClient.connected()) {
  //   Serial.println("Send LWT message");
  //   mqttClient.publish(LWT_TOPIC, STATUS_ON);
  // }
}

void MQTTSetup::_messageHandler(char* topic, uint8_t* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.printf("], data length: %d \n", length);

  const int capacity = JSON_OBJECT_SIZE(2) + 50;
  DynamicJsonDocument doc(capacity);
  DeserializationError error = deserializeJson(doc, payload);

  // Test if parsing succeeded.
  if (error) {
    Serial.print("deserialize MQTT message failed: ");
    Serial.println(error.f_str());
    return;
  }

  // const char* action = doc["action"];
  // int id = doc["id"];
  // Serial.println(id);
  // Serial.println(action);

  MESSAGE_CALLBACK_SIGNATURE cb = this->_messageCallbacks[std::string(topic)];

  if (cb) {
    //call the callback func
    this->_messageCallbacks[topic](std::string(topic), doc);
  }
  else {
    Serial.println("callback is NULL");
  }
}

MQTTSetup::MQTTSetup()
{
}

MQTTSetup::~MQTTSetup()
{
}

void MQTTSetup::_reconnect() {
  // Loop until we're reconnected
  while (!pubSubClient.connected()) {
    pubSubClient.disconnect();

    Serial.printf("MQTT Attempting connect %s...\n", this->_host.c_str());

    // clientId += String(random(0xffff), HEX);

    // Attempt to connect
    String lwtTopic(LWT_TOPIC);
    lwtTopic.replace("{DEVICE_ID}", this->_clientId);
    if (pubSubClient.connect(this->_clientId.c_str(), 
                this->_username.c_str(), this->_password.c_str(), 
                lwtTopic.c_str(),
                1, true, STATUS_OFF)) {

      Serial.println("MQTT connected");

      // Once connected, publish an announcement...
      this->pubSubClient.publish(lwtTopic.c_str(), STATUS_ON);

    } else {
      Serial.print("MQTT connect failed, rc=");
      Serial.print(pubSubClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void MQTTSetup::init(const String clientId, const String host, const String username, const String password)
{
  this->_clientId = clientId;
  this->_host = host;
  this->_username = username;
  this->_password = password;


  using std::placeholders::_1;
  using std::placeholders::_2;
  using std::placeholders::_3;

  // IPAddress ip;
  // ip.fromString(MQTT_SERVER);


  Serial1.println("Init MQTT client...");
  pubSubClient.setBufferSize(512);
  pubSubClient.setClient(wifiClient);
  pubSubClient.setServer(this->_host.c_str(), 1883);
  pubSubClient.setKeepAlive(5);
  pubSubClient.setCallback(std::bind(&MQTTSetup::_messageHandler, this, _1, _2, _3));


  lwtTicker.attach(LWT_INTERVAL, mqtt_tick);
}

void MQTTSetup::connectAndReceive()
{
  if (!pubSubClient.connected()) {
    this->_reconnect();

    Serial.println("Subcribing to topics registered...");

    //MQTT connected, we subcribe all topics that registered
    callback_map::iterator itBegin = this->_messageCallbacks.begin();
    callback_map::iterator itEnd = this->_messageCallbacks.end();
    for (auto const& item : this->_messageCallbacks)
    {
        // this->pubSubClient.subscribe(item.first.c_str());
        this->pubSubClient.subscribe(item.first.c_str());
    }

    Serial.println("MQTT establish connection done. Start receiving message...");
  }
  pubSubClient.loop();
}

MESSAGE_CALLBACK_SIGNATURE MQTTSetup::registerCallback(std::string topic, MESSAGE_CALLBACK_SIGNATURE callback)
{
  // subcribe to topic, the function will check mqtt is connected or not
  this->pubSubClient.subscribe(topic.c_str());

  MESSAGE_CALLBACK_SIGNATURE _oldcb = this->_messageCallbacks[topic];
  this->_messageCallbacks[topic] = callback;

  return _oldcb;
}

MESSAGE_CALLBACK_SIGNATURE MQTTSetup::unregisterCallbac(std::string topic)
{
  this->pubSubClient.unsubscribe(topic.c_str());

  MESSAGE_CALLBACK_SIGNATURE _oldcb = this->_messageCallbacks[topic];
  this->_messageCallbacks.erase(topic);
  return _oldcb;
}