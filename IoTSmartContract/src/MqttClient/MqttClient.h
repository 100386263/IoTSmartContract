#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <WiFi.h>
#include <PubSubClient.h>

class MqttClient
{
public:
    MqttClient(const char *mqtt_server, int *variable);
    void connect();
    void loop();

private:
    static void callback(char *topic, byte *payload, unsigned int length);
    WiFiClient espClient;
    PubSubClient mqttClient{espClient};
    const char *mqtt_server;
    int *variable;
};
extern int LAST_STATE;
#endif
