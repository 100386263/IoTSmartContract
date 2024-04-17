#ifndef SOLARPREDICTION_H
#define SOLARPREDICTION_H

#include <Arduino.h>

class SolarPrediction
{
private:
  float prediction[24]; // Array para almacenar las predicciones para cada hora del día

public:
  // Constructor
  SolarPrediction();
  void updatePredictions();

  // Método para obtener la predicción para la siguiente hora
  float getPredictionForNextHour(int currentHour);
};

#endif