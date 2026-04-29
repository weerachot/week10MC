int pinOutput[] = { 22, 23 };
String MassageID[] = { "AA11", "AA22",
                       "BB11", "BB22", "CC11", "CC22" };
String myID = "CC";
String strRxBuffer;
void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
  Serial2.setPins(16 /*16=Rx*/, 17
                  /*17=Tx*/);
  Serial2.setTimeout(10);
  for (int i = 0; i < 2; i++)
    pinMode(pinOutput[i], OUTPUT);
  Serial.println("Rx Slave, myID = " + myID);
}
void loop() {
  if (Serial2.available() > 0) {
    String strBuffer = Serial2.readString();
    strBuffer.trim();
    Serial.print(strBuffer + " >> ");
    String getID = strBuffer.substring(0, 2);
    int getPort = strBuffer.substring(2,
                                      4)
                    .toInt();
    int getValue = strBuffer.substring(5,
                                       6)
                     .toInt();
    strBuffer = "Get ID = " + getID + ", " + String(getPort) + ", " + String(getValue);
    Serial.println(strBuffer);
    if ((getID == myID) & (getPort == 11))
      digitalWrite(pinOutput[0], getValue);
    if ((getID == myID) & (getPort == 22))
      digitalWrite(pinOutput[1], getValue);
  }
}