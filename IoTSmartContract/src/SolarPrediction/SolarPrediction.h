#ifndef SOLARPREDICTION_H
#define SOLARPREDICTION_H

#include <Arduino.h>
struct date_struct
{
  int day;
  int hour;
}; // Atributo para almacenar la fecha y hora actual
class SolarPrediction
{
private:
  struct hour_production
  {
    int day;
    int hour;
    int production;
  };

  hour_production predictions[48]; // Array para almacenar las predicciones para cada hora del día
  date_struct current_date;

public:
  // Constructor
  SolarPrediction();
  void updatePredictions();

  // Método para obtener la predicción para la siguiente hora
  int getPredictionForNextHour();

  // Método para actualizar la fecha y hora actual
  void updateCurrentDateTime();
};

#endif
