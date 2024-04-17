#include "SolarPrediction.h"
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

SolarPrediction::SolarPrediction()
{
  // Constructor
}

// Método para obtener la predicción para la siguiente hora
float SolarPrediction::getPredictionForNextHour(int currentHour)
{
  // Supongamos que aquí haces una petición HTTP para obtener las predicciones en tiempo real
  // Por ahora, simplemente supongamos que tienes predicciones estáticas

  // Definir un arreglo de predicciones para cada hora del día (estático)
  float predicciones[] = {0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};

  // Asegurarse de que la hora actual esté dentro del rango válido (0-23)
  currentHour %= 24;

  // Obtener la predicción para la siguiente hora
  int nextHour = (currentHour + 1) % 24;
  return predicciones[nextHour];
}

void SolarPrediction::updatePredictions()
{
  HTTPClient http;

  // Realizar la solicitud HTTP GET
  http.begin("http://192.168.0.33:1880/endpoint/estimate/40.52037/-3.54458/35/0/6");

  int httpCode = http.GET(); // Realizar la solicitud GET

  if (httpCode == HTTP_CODE_OK)
  {
    String payload = http.getString(); // Obtener el cuerpo de la respuesta

    // Parsear el JSON
    JsonDocument doc;
    deserializeJson(doc, payload);

    // Obtener el valor de watt_hours
    JsonObject result = doc["result"];
    JsonObject watt_hours = result["watt_hours"];

    // Iterar sobre los elementos de watt_hours y imprimirlos
    Serial.println("Valores de watt_hours:");
    for (JsonPair kv : watt_hours)
    {
      Serial.print(kv.key().c_str());
      Serial.print(": ");
      Serial.println(kv.value().as<int>());
    }

    http.end(); // Cerrar la conexión
  }
  else
  {
    Serial.println("Error al solicitar http");
    http.end(); // Cerrar la conexión
  }
}