// ModbusClient.h
#ifndef MODBUSCLIENT_H
#define MODBUSCLIENT_H

#include <Arduino.h>
#include <ModbusMaster.h>

class ModbusClient {
public:
    ModbusClient(); // Constructor declaration
    float query_modbus_data();

private:
    ModbusMaster modbusNode;
};

#endif
