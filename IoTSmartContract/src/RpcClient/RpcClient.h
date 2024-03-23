#ifndef MiClase_h
#define MiClase_h

#include <Arduino.h>
#include <string>

class RpcClient {
private:
    String NODE_URL;
    String wallet;
    String smart_contract;
    String function;

public:
    RpcClient(String url, String contract_addr, String wallet_addr);
    String send_rpc(String function, int arguments);
    String generate_function_code(int value, String function_hex);

};

#endif
