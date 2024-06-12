// config.h
#ifndef CONFIG_H
#define CONFIG_H

// Definición de constantes
#define MODBUS_SLAVE_ADDRESS 1
#define MODBUS_SERIAL_BAUDRATE 9600
#define MODBUS_SERIAL_RX_PIN 16
#define MODBUS_SERIAL_TX_PIN 17
#define NODE_URL "http://192.168.0.35:8545"
#define SAVE_DATA_INTERVAL 5
#define PURCHASE_SELL_INTERVAL 20


const char* WIFI_SSID = "DIGIFIBRA-ttZU";
const char* WIFI_PASSWORD = "contraseñawifi";
const char* SMART_CONTRACT = "0x6000377E652025D23051929a042214f0182aFe8c";
const char* WALLET = "0x6947191f0E3d309d43C74A390C9F5c0810c7244C";
const char* MQTT_SERVER_IP = "192.168.0.35";

const float strategy[3] = {0.1,0.5,0.7};


#endif  // CONFIG_H