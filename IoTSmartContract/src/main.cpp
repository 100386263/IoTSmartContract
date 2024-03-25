#include <Arduino.h>
#include <WiFi.h>
#include "./ModbusClient/ModbusClient.h"
#include "./RpcClient/RpcClient.h"
#include "./DataRecorder/DataRecorder.h"
#include "./MqttClient/MqttClient.h" // Asegúrate de incluir el archivo de encabezado de MqttClient
#include <config.h>
int variable = 0;
ModbusClient modbus_client;
RpcClient rpc_client(NODE_URL, SMART_CONTRACT, WALLET);
DataRecorder data_recorder(SAVE_DATA_INTERVAL);
MqttClient mqtt_client(MQTT_SERVER_IP, &variable);

int lastMarketOperation = 0;

void setup()
{
  Serial.begin(MODBUS_SERIAL_BAUDRATE);
  Serial2.begin(MODBUS_SERIAL_BAUDRATE, SERIAL_8N1, MODBUS_SERIAL_RX_PIN, MODBUS_SERIAL_TX_PIN);

  // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");
  // Muestra la dirección IP
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
  mqtt_client.connect();
}

void loop()
{
  unsigned long startTime = millis(); // Tiempo de inicio

  int modbus_data = int(modbus_client.consultarDatos());
  data_recorder.recordData(modbus_data);
  Serial.println(modbus_data);
  if (millis() - lastMarketOperation >= PURCHASE_SELL_INTERVAL * 1000)
  {
    int average = data_recorder.calculateAverage();
    float threshold = modbus_data * 0.1; // Calcula el 10% de modbus_data

    if (average >= modbus_data - threshold && average <= modbus_data + threshold)
    {
      Serial.println(rpc_client.send_rpc("0x6e5d8e03", modbus_data));
    }

    lastMarketOperation = millis();
  }
  mqtt_client.loop();
  unsigned long executionTime = millis() - startTime; // Tiempo transcurrido
  // Verificar si el tiempo de ejecución es menor que 1 segundo
  if (executionTime < 1000)
  {
    delay(1000 - executionTime); // Agregar el retraso restante
  }
  else
  {
    Serial.println("Eres muy lento");
  }
}
