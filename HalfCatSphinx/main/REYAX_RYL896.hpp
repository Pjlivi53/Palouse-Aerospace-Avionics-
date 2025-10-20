#pragma once
#include <Arduino.h>

void LoRaSend(String data)
{
  int valuelength = data.length();
  String mymessage;
  mymessage = mymessage + "AT+SEND=0" + "," + valuelength + "," + data + "\n";
  Serial1.println(mymessage);
  Serial.print(mymessage);
}


