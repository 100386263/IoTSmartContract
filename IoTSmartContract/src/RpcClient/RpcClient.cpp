#include "RpcClient.h"
#include <HTTPClient.h>

RpcClient::RpcClient(String url, String contract_addr, String wallet_addr)
{
    NODE_URL = url;
    wallet = wallet_addr;
    smart_contract = contract_addr;
}

String RpcClient::send_rpc(String function, int arguments)
{

    HTTPClient http;
    String jsonRequest = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_sendTransaction\",\"params\":[{\"from\":\"";
    jsonRequest += wallet;
    jsonRequest += "\",\"to\":\"";
    jsonRequest += smart_contract;
    jsonRequest += "\",\"data\":\"";
    jsonRequest += generate_function_code(arguments, function);
    jsonRequest += "\"}],\"id\":1}";
    http.begin(NODE_URL);
    http.addHeader("Content-Type", "application/json");
    http.POST(jsonRequest);

    int httpResponseCode = http.POST(jsonRequest);

    if (httpResponseCode > 0)
    {
        String response = http.getString();
        return response;
    }
    else
    {
        http.end();
        return String(httpResponseCode);
    }
}

String RpcClient::generate_function_code(int value, String function_hex)
{
    char buffer[65];                 
    sprintf(buffer, "%064X", value);
    String arguments = String(buffer);
    String keccak = function_hex + arguments;
    return keccak;
}
