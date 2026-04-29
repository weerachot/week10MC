// ===== RX BOARD =====
#define BAUD 115200

// int ledPins[8] = {13, 12, 14, 27, 26, 25, 33, 32};
int ledPins[8] = {2, 4, 5, 18, 19, 21, 22, 23};
void setup() {
  Serial.begin(BAUD);
  Serial2.begin(BAUD, SERIAL_8N1, 16, 17);

  for (int i = 0; i < 8; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  Serial.println("RX Ready");
}

void loop() {
  if (Serial2.available()) {
    char c = Serial2.read();

    Serial.print("Received: ");
    Serial.println(c);

    if (c >= '1' && c <= '8') {
      int index = c - '1';

      // ดับทั้งหมด
      for (int i = 0; i < 8; i++) {
        digitalWrite(ledPins[i], LOW);
      }

      // เปิด LED ตามเลข
      digitalWrite(ledPins[index], HIGH);
    }
  }
}