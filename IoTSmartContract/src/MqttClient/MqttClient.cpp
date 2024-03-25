#include "MqttClient.h"
#include <string.h>

int LAST_STATE = 0; // 0 = CONSERVADOR , 1 = MODERADO ,2 = ARRIESGADO
char MODE_TOPIC[50];
MqttClient::MqttClient(const char *mqtt_server) : mqtt_server(mqtt_server)
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
            // Obtener la dirección IP local
            IPAddress localIP = WiFi.localIP();

            // Construir el topic con la dirección IP
            char ip_str[50]; // Ajusta el tamaño según sea necesario
            sprintf(ip_str, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);
            sprintf(mode_topic, "%s/mode", ip_str);
            sprintf(consumption_topic, "%s/consumption", ip_str);
            mqttClient.subscribe(mode_topic);

            char buffer[3]; // Tamaño del buffer

            sprintf(buffer, "%d", LAST_STATE);
            // Publicar un mensaje al conectar
            strcpy(MODE_TOPIC,mode_topic);
            mqttClient.publish(mode_topic, buffer);
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
        connect();
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
    if (strcmp(topic, MODE_TOPIC) == 0)
    {
        LAST_STATE = receivedInt;
    }
}
void MqttClient::set_consumption(int consumption){
                char buffer[20]; // Tamaño del buffer

            sprintf(buffer, "%d", consumption);
            mqttClient.publish(consumption_topic, buffer);
}