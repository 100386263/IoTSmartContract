// main.cpp
#include <Arduino.h>
#include <WiFi.h>

#include "./ModbusClient/ModbusClient.h"
#include "./RpcClient/RpcClient.h"
#include <config.h>

ModbusClient modbus_client;
RpcClient rpc_client(NODE_URL, SMART_CONTRACT, WALLET );

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
}

void loop()
{
  int modbus_data= int(modbus_client.consultarDatos());
  Serial.println(modbus_data);
  Serial.println(rpc_client.send_rpc("0x6e5d8e03",modbus_data));
  delay(5000);
}
