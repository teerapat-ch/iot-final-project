#include "HX711.h"
#include <Servo.h>

#define LoadDT  21   // ขา DT ต่อกับ GPIO 21
#define LoadSCK 22   // ขา SCK ต่อกับ GPIO 22
#define servoPin 10

HX711 scale;
Servo myservo;

unsigned long lastSec = 0;

void setup() {
  Serial.begin(115200);

  scale.begin(LoadDT, LoadSCK);
  Serial.println("Initializing the scale...");
  scale.set_scale();     // ตั้งค่าเริ่มต้น
  scale.tare();          // รีเซ็ตค่าน้ำหนักเป็น 0
  Serial.println("Scale ready");

  myservo.attach(servoPin);
}

void loop() {

  unsigned long currSec = millis();

  if (currSec - lastSec >= 1000) {
    Serial.print("Weight: ");
    Serial.print(scale.get_units(10), 2);  // อ่านค่าเฉลี่ย 10 ครั้ง
    Serial.println(" g");

    if (scale.get_units(10) < 1) {
      myservo.writeMicroseconds(0);
      delay(1000);
    }
  }
}