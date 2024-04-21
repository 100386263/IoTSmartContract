#include <Arduino.h>
#include <WiFi.h>
#include "./ModbusClient/ModbusClient.h"
#include "./RpcClient/RpcClient.h"
#include "./DataRecorder/DataRecorder.h"
#include "./MqttClient/MqttClient.h" // Asegúrate de incluir el archivo de encabezado de MqttClient
#include "./SolarPrediction/SolarPrediction.h"
#include <config.h>

ModbusClient modbus_client;
RpcClient rpc_client(NODE_URL, SMART_CONTRACT, WALLET);
DataRecorder data_recorder(SAVE_DATA_INTERVAL);
MqttClient mqtt_client(MQTT_SERVER_IP);
SolarPrediction solar_predictor;
int lastMarketOperation = 0;
int last_strategy = 0;
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
  solar_predictor.updatePredictions();
}

void purchase_sell(int modbus_data, float threshold)
{
  if (millis() - lastMarketOperation >= PURCHASE_SELL_INTERVAL * 1000)
  {
    int average = data_recorder.calculateAverage();                        // Media de CONSUMO de la vivienda
    int next_hour_production = solar_predictor.getPredictionForNextHour(); // Predicción de producción solar próxima hora
    int result = next_hour_production - average;
    Serial.print("Produccion: "); // Predicción menos consumo
    Serial.println(next_hour_production);
    Serial.print("Media: "); // Predicción menos consumo
    Serial.println(average);
    Serial.print("Resultado: "); // Predicción menos consumo
    Serial.println(result);
    if (result > 0)
    {
      // Hay que llamar a la funcion de venta
      Serial.print("Se venden ");
      int energy_to_sell = int(result * threshold);
      Serial.print(energy_to_sell);
      Serial.println(" kWh");
      Serial.println(rpc_client.send_rpc("0x09baa07d", energy_to_sell));
    }
    else if (result < 0)
    {
      // Hay que llamar a la funcion de compra
      Serial.println("Se compran ");
      int energy_to_buy = int(average * threshold);
      Serial.print(energy_to_buy);
      Serial.println(" kWh");
      Serial.println(rpc_client.send_rpc("0x02bcd335", energy_to_buy));
    }

    lastMarketOperation = millis();
  }
}

void loop()
{
  unsigned long startTime = millis(); // Tiempo de inicio
  // Comprobamos estrategia mqtt
  mqtt_client.loop();
  if (STRATEGY != last_strategy)
  {
    mqtt_client.send_confirmation(STRATEGY);
  }
  last_strategy = STRATEGY;
  // Leemos consumo mqtt
  unsigned long startModbus = millis(); // Tiempo de inicio
  int modbus_data = int(modbus_client.consultarDatos());
  // Guardamos consumo y mandamos por mqtt
  data_recorder.recordData(modbus_data);
  // Publicamos consumo en mqtt
  int actual_production = solar_predictor.getPredictionForNextHour();
  mqtt_client.set_consumption(modbus_data, actual_production);

  // Se comprueba si con la estrategia actual se puede comprar o vender
  purchase_sell(modbus_data, strategy[STRATEGY]);

  unsigned long executionTime = millis() - startTime; // Tiempo transcurrido

  // Verificar si el tiempo de ejecución es menor que 1 segund
  if (executionTime < 1000)
  {
    delay(1000 - executionTime); // Agregar el retraso restante
  }
  else
  {
    Serial.println("Eres muy lento");
  }
}
