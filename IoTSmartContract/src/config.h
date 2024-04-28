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
const char* SMART_CONTRACT = "0xbb7fA9FCb8C070dcf22739fBf4Fa8640DF059b89";
const char* WALLET = "0x30B2F3A2Dbd2B2B326596eCf5F1F9CC538c02CD3";
const char* MQTT_SERVER_IP = "192.168.0.35";

const float strategy[3] = {0.1,0.5,0.7};


#endif  // CONFIG_H