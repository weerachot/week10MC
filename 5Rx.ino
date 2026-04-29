#define led 23

String buffer = "";
// --- แก้จุดนี้: กำหนดค่าเริ่มต้นให้โปรแกรมมีค่าใช้งานตั้งแต่แรก ---
int pulseCount = 100; 
int delayMs = 50;     
// -------------------------------------------------------

bool running = false;
unsigned long lastToggle = 0;
bool state = LOW;
int counter = 0;

void setup() {
  Serial.begin(115200); 
  delay(1000); 
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
  
  Serial.println("\n--- System Ready ---");
  showInfo(); 
}

void loop() {
  handleSerial();
  generatePulse();
}

void handleSerial() {
  while (Serial.available()) {
    char c = Serial.read();
    
    if (c == '\r' || c == '\n') {
      if (buffer.length() > 0) {
        processCommand(buffer);
        buffer = ""; 
      }
    } 
    else {
      char upperC = toupper(c);
      if (upperC == 'G') {
        startRun();
      } else if (upperC == '?') {
        showInfo();
      } else {
        buffer += upperC; 
      }
    }
  }
}

void processCommand(String cmd) {
  cmd.trim(); 
  // ตรวจสอบ P=xxx
  if (cmd.startsWith("P=")) {
    String valStr = cmd.substring(2);
    int val = valStr.toInt();
    // ยอมรับค่า 0 (ต่อเนื่อง) หรือ 100-999
    if (val == 0 || (val >= 100 && val <= 999)) {
      pulseCount = val;
      Serial.print(">> Updated: pulseCount = "); Serial.println(pulseCount);
    } else {
      Serial.println(">> Error: Pulse must be 0 or 100-999");
    }
  }
  // ตรวจสอบ D=yy
  else if (cmd.startsWith("D=")) {
    String valStr = cmd.substring(2);
    int val = valStr.toInt();
    if (val >= 10 && val <= 99) {
      delayMs = val;
      Serial.print(">> Updated: delayMs = "); Serial.print(delayMs); Serial.println(" ms");
    } else {
      Serial.println(">> Error: Delay must be 10-99 ms");
    }
  }
}

void startRun() {
  if(running) {
    Serial.println("Already running...");
    return;
  }
  
  // ป้องกัน delayMs เป็น 0 เพื่อไม่ให้คำนวณผิดพลาด
  if (delayMs <= 0) delayMs = 50; 

  running = true;
  counter = 0;
  state = LOW;
  lastToggle = millis();
  
  float freq = 1000.0 / (2.0 * delayMs);
  Serial.println("\n[Action: START]");
  Serial.print("Target Pulse: "); Serial.println(pulseCount == 0 ? "Continuous" : String(pulseCount));
  Serial.print("Interval: "); Serial.print(delayMs); Serial.println(" ms");
  Serial.print("Frequency: "); Serial.print(freq); Serial.println(" Hz");
}

void generatePulse() {
  if (!running) return;

  if (millis() - lastToggle >= (unsigned long)delayMs) {
    lastToggle = millis();
    state = !state;
    digitalWrite(led, state);

    if (state == HIGH) {
      counter++;
      if (pulseCount != 0 && counter >= pulseCount) {
        running = false;
        digitalWrite(led, HIGH); 
        Serial.print("Done! Total pulses: "); Serial.println(counter);
      }
    }
  }
}

void showInfo() {
  Serial.println("\n===== DEVICE INFO =====");
  Serial.println("Author: Weerachot Hongkongka");
  Serial.println("Commands:");
  Serial.println("  P=xxx (e.g., P=500, P=0)");
  Serial.println("  D=yy  (e.g., D=20)");
  Serial.println("  G     (Start)");
  Serial.println("  ?     (Info)");
  Serial.println("------------------------");
  Serial.print("Current Pulse: "); Serial.println(pulseCount);
  Serial.print("Current Delay: "); Serial.print(delayMs); Serial.println(" ms");
  Serial.println("------------------------");
}