#include "REYAX_RYL896.hpp"

void LoRaSend(const String &data)
{
    int valueLength = data.length();
    String message;
    message = "AT+SEND=0,";
    message += valueLength;
    message += ",";
    message += data;
    message += "\n";
    Serial1.println(message);
    Serial.print(message);
}
