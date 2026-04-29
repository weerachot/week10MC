int pinInput[] = { 5, 18, 19, 21, 22, 23 };
String MassageID[] = { "AA11", "AA22",
                       "BB11", "BB22", "CC11", "CC22" };
void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
  Serial2.setPins(16 /*16=Rx*/, 17
                  /*17=Tx*/);
  for (int i = 0; i < 6; i++)
    pinMode(pinInput[i], INPUT_PULLUP);
}
void loop() {
  for (int i = 0; i < 6; i++) {
    int temp = digitalRead(pinInput[i]);
    String sbuff = MassageID[i];
    sbuff += "=" + String(temp);
    Serial.println(sbuff);
    Serial2.println(sbuff);
    delay(250);
  }
  Serial.println();
  Serial2.println();
}