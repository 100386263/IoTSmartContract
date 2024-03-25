// DataRecorder.cpp
#include "DataRecorder.h"

DataRecorder::DataRecorder(int saveInterval) : dataCount(0), sum(0), lastRecordTime(0) {
    NUM_DATA = 3600 / saveInterval; // Calcula NUM_DATA en base al intervalo de guardado
    data = new int[NUM_DATA]; // Crea el array de longitud NUM_DATA
    save_interval = saveInterval;
}

void DataRecorder::recordData(int newData) {
    // Comprueba si ha pasado el intervalo de guardado
    if (millis() - lastRecordTime >= save_interval * 1000) {
        // Guarda newData en el array si hay espacio
        if (dataCount < NUM_DATA) {
            data[dataCount] = newData;
            dataCount++;
        } else {
            // Si el array está lleno, descarta el dato más antiguo y guarda el nuevo dato
            for (int i = 0; i < NUM_DATA - 1; i++) {
                data[i] = data[i + 1];
            }
            data[NUM_DATA - 1] = newData;
        }
        lastRecordTime = millis(); // Actualiza el tiempo de la última grabación
    }
}
int DataRecorder::calculateAverage() {
    if (dataCount == 0) {
        return 0; // Retorna 0 si no hay datos guardados para evitar divisiones por cero
    }

    int sum = 0;
    for (int i = 0; i < dataCount; i++) {
        sum += data[i];
    }
    int average = int(sum/dataCount);
    return average; // Redondea al entero más cercano
}
