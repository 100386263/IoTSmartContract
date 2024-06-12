// DataRecorder.h
#ifndef DATA_RECORDER_H
#define DATA_RECORDER_H

#include <Arduino.h>

class DataRecorder {
public:
    DataRecorder(int saveInterval);
    void recordData(int newData);
    int calculateAverage();

private:
    int save_interval;
    int NUM_DATA;
    int* data;
    int dataCount;
    int sum;
    unsigned long lastRecordTime;
};

#endif // DATA_RECORDER_H
