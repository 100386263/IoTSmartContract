#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <PubSubClient.h>
#include <WiFi.h>

class MqttClient {
private:
    IPAddress mqttServer;
    int *mqttPort;
    WiFiClient wifiClient;  // Objeto WiFiClient para la conexión MQTT
    PubSubClient mqttClient;

public:
    // Constructor
    MqttClient(IPAddress server, int *selectedMode, WiFiClient& client);

    // Método para conectar al servidor MQTT
    void connect(const char *ssid, const char *password);

    // Método para publicar un mensaje en un tema MQTT
    void publish(const char *topic, const char *payload);

    // Método para manejar las suscripciones y los mensajes recibidos
    void handleSubscriptions();

    // Función de callback para manejar los mensajes recibidos
    static void callback(char *topic, byte *payload, unsigned int length);
};

#endif
