#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <PubSubClient.h>
#include <WiFiClient.h>

class MQTTManager {
  public:
    MQTTManager(const char* server, WiFiClient& wifiClient);
    void connect(const char* topic1, const char* topic2, const char* topic3);
    void setCallback(MQTT_CALLBACK_SIGNATURE);
    void loop();
    void publishMessage(const char* topic, const char* message);
    // bool testMsg(byte* payload, const char* msg, size_t length);

  private:
    PubSubClient client;
    const char* server;
    unsigned long lastMsg;
};

#endif
