#include "MqttClient.h"

int LAST_STATE = 0;
MqttClient::MqttClient(const char *mqtt_server, int *variable_) : mqtt_server(mqtt_server), variable(variable_)
{
    mqttClient.setServer(mqtt_server, 1883);
    mqttClient.setCallback(callback);
}

void MqttClient::connect()
{
    while (!mqttClient.connected())
    {
        Serial.println("Conectando al servidor MQTT...");
        if (mqttClient.connect("arduino-client", "marcos", "Maraljo1"))
        {
            Serial.println("Conectado al servidor MQTT");
            mqttClient.subscribe("prueba/iot");
        }
        else
        {
            Serial.print("Error de conexión, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" Intentando de nuevo en 5 segundos");
            delay(5000);
        }
    }
}

void MqttClient::loop()
{
    if (!mqttClient.connected())
    {
        mqttClient.connect("arduino-client", "marcos", "Maraljo1");
    }
    mqttClient.loop();
}

void MqttClient::callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Mensaje recibido en el tema: ");
    Serial.println(topic);
    Serial.print("Contenido: ");

    // Convertir payload a un entero
    int receivedInt = atoi((char *)payload);

    // Imprimir el valor del entero recibido
    Serial.println(receivedInt);
    LAST_STATE = receivedInt;
}