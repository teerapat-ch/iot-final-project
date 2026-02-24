#include "HX711.h"

#define DT  21   // ขา DT ต่อกับ GPIO 21
#define SCK 22   // ขา SCK ต่อกับ GPIO 22

HX711 scale;

void setup() {
  Serial.begin(115200);

  scale.begin(DT, SCK);

  Serial.println("Initializing the scale...");

  scale.set_scale();     // ตั้งค่าเริ่มต้น
  scale.tare();          // รีเซ็ตค่าน้ำหนักเป็น 0

  Serial.println("Scale ready");
}

void loop() {
  Serial.print("Weight: ");
  Serial.print(scale.get_units(10), 2);  // อ่านค่าเฉลี่ย 10 ครั้ง
  Serial.println(" g");
  delay(1000);
}