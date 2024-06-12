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
int days_in_month(int month, int year)
{
  switch (month)
  {
  case 1:  // Enero
  case 3:  // Marzo
  case 5:  // Mayo
  case 7:  // Julio
  case 8:  // Agosto
  case 10: // Octubre
  case 12: // Diciembre
    return 31;
  case 4:  // Abril
  case 6:  // Junio
  case 9:  // Septiembre
  case 11: // Noviembre
    return 30;
  case 2: // Febrero
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
    {
      // Año bisiesto, febrero tiene 29 días
      return 29;
    }
    else
    {
      // No es un año bisiesto, febrero tiene 28 días
      return 28;
    }
  default:
    return -1; // Mes inválido
  }
}

int SolarPrediction::getPredictionForNextHour()
{
  timeClient.begin();
  timeClient.update(); // Update the time

  // Obtener la fecha y hora actual
  time_t rawTime = timeClient.getEpochTime();
  struct tm *timeinfo;
  timeinfo = localtime(&rawTime);

  int day = timeinfo->tm_mday;
  int next_hour = timeinfo->tm_hour + 1;

  // Verificar si el siguiente hora está en el próximo día
  if (next_hour > 23)
  {
    next_hour = 0;
    day = day + 1;

    // Verificar si el próximo día es el primer día del mes siguiente
    if (day > days_in_month(timeinfo->tm_mon + 1, timeinfo->tm_year + 1900))
    {
      day = 1;
      timeinfo->tm_mon += 1;

      // Verificar si el próximo mes es el siguiente año
      if (timeinfo->tm_mon > 11)
      {
        timeinfo->tm_mon = 0;
        timeinfo->tm_year += 1;
      }
    }
  }
  // Buscar la predicción para la próxima hora
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
