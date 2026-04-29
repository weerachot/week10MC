#include "ModbusMaster.h"
//https://github.com/4-20ma/ModbusMaster
#define Slave_ID 1
#define RX_PIN 16
#define TX_PIN 17
ModbusMaster modbus;
void setup() {
  Serial.begin(115200, SERIAL_8N1);
  Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  modbus.begin(Slave_ID, Serial2);
}
long lastMillis = 0;
double tempp_dbl, humid_dbl;
String resString;
void loop() {
  long currentMillis = millis();
  if (currentMillis - lastMillis > 1000) {
    resString = "\r\n";
    uint8_t result = modbus.readInputRegisters(1, 2);
    if (result == modbus.ku8MBSuccess) {
      tempp_dbl = modbus.getResponseBuffer(0) / 10.0;
      humid_dbl = modbus.getResponseBuffer(1) / 10.0;
      resString += "Temperature: " + String(tempp_dbl) + " C\r\n";
      resString += "Humidity: " + String(humid_dbl) + " %";
    } else
      resString += "Modbus Read Error";
    Serial.println(resString);
    lastMillis = currentMillis;
  }
}
