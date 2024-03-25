// ModbusClient.cpp
#include "ModbusClient.h"
#include "ModbusMaster.h"

ModbusClient::ModbusClient() {
    // Aquí es donde se inicializa modbusNode con la comunicación serie
    modbusNode.begin(1, Serial2); // Se asume que el dispositivo Modbus está en la dirección 1
}

float ModbusClient::consultarDatos() {
  uint8_t result = modbusNode.readHoldingRegisters(0x2004, 4);  // Lee 4 registros empezando desde la dirección 0x2000

  // Verifica si la lectura fue exitosa
  if (result == modbusNode.ku8MBSuccess) {
    // Combina los valores leídos en un solo valor de 32 bits
    uint32_t val = 0;
    val |= (uint32_t)modbusNode.getResponseBuffer(0) << 16;  // Lee los primeros dos bytes y los desplaza 16 bits a la izquierda
    val |= modbusNode.getResponseBuffer(1);   // Lee los siguientes dos bytes
    float f_val = val/100;
   return f_val;  // Devuelve el valor leído
  } else {

    return result;  // Devuelve el código de error
  }
}
