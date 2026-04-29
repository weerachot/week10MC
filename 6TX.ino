// // ===== LED =====
// int ledPins[8] = {4, 5, 18, 19, 21, 22, 23, 2};

// ===== TX BOARD =====
#define BAUD 115200
int swPins[8] = {13, 12, 14, 27, 26, 25, 33, 32};
bool lastState[8];
void setup() {
  Serial.begin(BAUD);
  Serial2.begin(BAUD, SERIAL_8N1, 16, 17);

  for (int i = 0; i < 8; i++) { 
    pinMode(swPins[i], INPUT_PULLUP);
    lastState[i] = digitalRead(swPins[i]);
  }

  Serial.println("TX Ready");
}

void loop() {
  for (int i = 0; i < 8; i++) {
    bool now = digitalRead(swPins[i]);

    // กดใหม่ (HIGH → LOW)
    if (lastState[i] == HIGH && now == LOW) {
      char c = '1' + i;   // ASCII '1'..'8'

      Serial.print("Send: ");
      Serial.println(c);

      Serial2.write(c);   // ส่งไป RX
      delay(50);          // debounce
    }

    lastState[i] = now;
  }
}