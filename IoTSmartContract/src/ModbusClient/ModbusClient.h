// ModbusClient.h
#ifndef MODBUSCLIENT_H
#define MODBUSCLIENT_H

#include <Arduino.h>
#include <ModbusMaster.h>

class ModbusClient {
public:
    ModbusClient(); // Constructor declaration
    float consultarDatos();

private:
    ModbusMaster modbusNode;
};

#endif
