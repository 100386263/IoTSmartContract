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
const char* SMART_CONTRACT = "0x0F967ABE2A72c86b8797591C125700fCa6E849E9";
const char* WALLET = "0x54E7483f633aBAaCe5611DD54c906948f32c2fB7";
#endif  // CONFIG_H