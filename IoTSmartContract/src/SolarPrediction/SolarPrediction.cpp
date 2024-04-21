#include "SolarPrediction.h"
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <../NTPClient/NTPClient.h>
#include <TimeLib.h>
#include <string.h>
const long utcOffsetInSeconds = 7200; // Adjust according to your timezone offset

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

SolarPrediction::SolarPrediction()
{
}

// Método para obtener la predicción para la siguiente hora
int SolarPrediction::getPredictionForNextHour()
{
  // Supongamos que aquí haces una petición HTTP para obtener las predicciones en tiempo real
  // Por ahora, simplemente supongamos que tienes predicciones estáticas
  // Constructor
  timeClient.begin();
  timeClient.update(); // Update the time

  // Obtener la fecha y hora actual
  time_t rawTime = timeClient.getEpochTime();
  struct tm *timeinfo;
  timeinfo = localtime(&rawTime);
  int day = timeinfo->tm_mday;
  int next_hour = timeinfo->tm_hour + 1;
  if (next_hour > 23)
  {
    next_hour = 0;
    day = day + 1;
  }
  for (hour_production prediction : predictions)
  {
    if (prediction.day == day && prediction.hour == next_hour)
    {
      return prediction.production;
    }
  }

  return -1;
}

void SolarPrediction::updateCurrentDateTime()
{
  // Constructor
  timeClient.begin();
  timeClient.update(); // Update the time

  // Obtener la fecha y hora actual
  time_t rawTime = timeClient.getEpochTime();
  struct tm *timeinfo;
  timeinfo = localtime(&rawTime);
  SolarPrediction::current_date.day = timeinfo->tm_mday;
  SolarPrediction::current_date.hour = timeinfo->tm_hour; // Corrected from tm_mday to tm_hour
}

void SolarPrediction::updatePredictions()
{
  HTTPClient http;
  SolarPrediction::updateCurrentDateTime(); // Added semicolon here
  for (int i = 0; i < 24; ++i)
  {
    predictions[i].day = current_date.day;
    predictions[i].hour = i;
    predictions[i].production = 0;
  }

  for (int i = 24; i < 48; ++i)
  {
    predictions[i].day = current_date.day + 1;
    predictions[i].hour = i - 24;
    predictions[i].production = 0;
  }

  // Realizar la solicitud HTTP GET
  http.begin("http://192.168.0.35:8000/estimate/40.52037/-3.54458/35/0/6");

  int httpCode = http.GET(); // Realizar la solicitud GET

  if (httpCode == HTTP_CODE_OK)
  {
    String payload = http.getString(); // Obtener el cuerpo de la respuesta

    // Parsear el JSON
    JsonDocument doc;
    deserializeJson(doc, payload);

    // Obtener el valor de watt_hours
    JsonObject result = doc["result"];
    JsonObject watt_hours = result["watt_hours_period"];

    for (JsonPair kv : watt_hours)
    {
      // Obtener la clave como cadena
      String key = kv.key().c_str();
      // Extrayendo los dos primeros dígitos de la hora y convirtiéndolos a un entero
      int prediction_hour = key.substring(11, 13).toInt();
      // Extrayendo el día del mes
      int prediction_day = key.substring(8, 10).toInt();
      int prediction_production = kv.value().as<int>();
      for (int i = 0; i < 48; ++i)
      {
        if (predictions[i].day == prediction_day && predictions[i].hour == prediction_hour)
        {
          // Se encontró una coincidencia, cambiar la producción a mil
          predictions[i].production = prediction_production;
          // Romper el bucle ya que ya hemos realizado el cambio
          break;
        }
      }
    }
    http.end(); // Cerrar la conexión
  }
  else
  {
    Serial.println("Error al solicitar http");
    http.end(); // Cerrar la conexión
  }
}
