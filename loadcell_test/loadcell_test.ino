#include "HX711.h"

#define DT  4   // ขา DT ต่อกับ GPIO 21
#define SCK 5   // ขา SCK ต่อกับ GPIO 22

HX711 scale;

// --- ขั้นตอนสำคัญ ---
// 1. นำของที่ทราบน้ำหนักแน่นอนมาวาง (เช่น ลูกตุ้ม 100g หรือขวดน้ำ)
// 2. ปรับค่าตัวเลขนี้จนกว่าหน้าจอจะแสดงค่าตรงกับของที่วาง
// 3. ถ้าค่ายิ่งวางของยิ่งติดลบ ให้ใส่เครื่องหมายลบหน้าตัวเลข เช่น -415.0
float calibration_factor = 15.0; 

void setup() {
  Serial.begin(115200);
  scale.begin(DT, SCK);

  Serial.println("Initializing the scale...");

  scale.set_scale(calibration_factor); // ใส่ค่าตัวคูณเพื่อแปลงเป็นกรัม
  scale.tare();                         // รีเซ็ตค่าเริ่มต้นให้เป็น 0 (ห้ามมีของวางบนตาชั่งตอนเปิดเครื่อง)

  Serial.println("Scale ready");
}

void loop() {
  // อ่านค่าเฉลี่ย 10 ครั้งเพื่อให้ค่านิ่งขึ้น
  float weight = scale.get_units(10);

  // ป้องกันการแสดงค่าติดลบกรณีเซนเซอร์เพี้ยนเล็กน้อย (Drift)
  // if (weight < 0) {
  //   weight = 0.00;
  // }

  Serial.print("Weight: ");
  Serial.print(weight, 2); 
  Serial.println(" g");

  delay(500);
}