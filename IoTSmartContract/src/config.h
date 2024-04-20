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

// Constantes para la configuración de la red WiFi
const char* WIFI_SSID = "DIGIFIBRA-ttZU";
const char* WIFI_PASSWORD = "maraljo1";
const char* SMART_CONTRACT = "0x811b069ea466578c88436b339F4A2cAf4205Bc6d";
const char* WALLET = "0x12fBEbcc5d9697D72C0927f15a44c599CAF49Fc2";
const char* MQTT_SERVER_IP = "192.168.0.35";

const float strategy[3] = {0.1,0.5,0.7};


#endif  // CONFIG_H