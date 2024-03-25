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
    MqttClient(IPAddress server, int *selectedMode, WiFiClient& client): mqttServer(server), mqttPort(selectedMode), wifiClient(client), mqttClient(wifiClient) {}

    // Método para conectar al servidor MQTT
    void connect(const char *ssid, const char *password) {
        mqttClient.setServer(mqttServer, *mqttPort);
        // Agrega aquí cualquier configuración adicional que necesites, como autenticación
        // Luego, intenta conectar al servidor MQTT
        while (!mqttClient.connected()) {
            if (mqttClient.connect("ArduinoClient")) {
                Serial.println("Conexión exitosa al servidor MQTT");
                // Suscribirse a los temas si es necesario
                // mqttClient.subscribe("topic");
            } else {
                Serial.print("Fallo en la conexión al servidor MQTT, rc=");
                Serial.print(mqttClient.state());
                Serial.println(" Intentando de nuevo en 5 segundos...");
                // Esperar 5 segundos antes de volver a intentar la conexión
                delay(5000);
            }
        }
    }

    // Método para publicar un mensaje en un tema MQTT
    void publish(const char *topic, const char *payload) {
        mqttClient.publish(topic, payload);
    }

    // Método para manejar las suscripciones y los mensajes recibidos
    void handleSubscriptions() {
        mqttClient.loop();
    }

    // Función de callback para manejar los mensajes recibidos
    static void callback(char *topic, byte *payload, unsigned int length) {
        Serial.print("Mensaje recibido en el tema: ");
        Serial.println(topic);

        Serial.print("Contenido del mensaje: ");
        for (int i = 0; i < length; i++) {
            Serial.print((char)payload[i]);
        }
        Serial.println();
    }
};
