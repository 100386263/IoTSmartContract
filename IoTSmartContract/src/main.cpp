// main.cpp
#include <Arduino.h>
#include "./ModbusClient/ModbusClient.h"
#include <WiFi.h>
// WiFi network settings
const char* ssid = "DIGIFIBRA-ttZU";
const char* password = "maraljo1";
ModbusClient modbus_client;

void setup()
{
  Serial.begin(9600);                      // Inicializar la comunicación serial con el monitor serie
  Serial2.begin(9600, SERIAL_8N1, 16, 17); // Inicializar Serial2 para comunicación Modbus

  // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");
  // Muestra la dirección IP
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  Serial.println(modbus_client.consultarDatos());
  delay(5000);
}
