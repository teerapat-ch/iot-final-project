#include "HX711.h"
#include <Servo.h>

#define LoadDT  21   // ขา DT ต่อกับ GPIO 21
#define LoadSCK 22   // ขา SCK ต่อกับ GPIO 22
#define servoPin 10 // Motor
#define buzzPin 2  // Buzzer

HX711 scale;
Servo myservo;

ฺbool activated = true; // เปิดการให้อาหารอัติโนมัติไหม (เดี๋ยวคุยอีกทีว่าจะมีมั้ย)

unsigned long interval = 1000; // วัดน้ำหนักทุกๆ X วินาที
unsigned long lastSec = 0; // ไว้เช็คเวลาแทน delay()

void fillFood() {
  tone(buzzPin, 1000);

  // Motor code
  // ...

  delay(1000);
}

void setup() {
  Serial.begin(115200);

  // Load cell setup
  scale.begin(LoadDT, LoadSCK);
  Serial.println("Initializing the scale...");
  scale.set_scale();     // ตั้งค่าเริ่มต้น
  scale.tare();          // รีเซ็ตค่าน้ำหนักเป็น 0
  Serial.println("Scale ready");

  // Servo setup
  myservo.attach(servoPin);
}

void loop() {
  unsigned long currSec = millis();
  if (currSec - lastSec >= 1000) {
    lastSec = currSec;
    float w = scale.get_units(10);
    Serial.print("Weight: ");
    Serial.println(w);

    if (currentWeight < 5.0) {
      fillFood();
    }
  }
}