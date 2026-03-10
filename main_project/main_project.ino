#include "HX711.h"
#include <ESP32Servo.h>
#include "secret.h"

// ---------- Blynk setup ----------

#define BLYNK_TEMPLATE_ID "TMPL6Tg7kQjXZ"
#define BLYNK_TEMPLATE_NAME "IoT Final Project"
#define BLYNK_AUTH_TOKEN SECRET_AUTH

#include <BlynkSimpleEsp32.h>

BlynkTimer timer;

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

// ---------- Sensors setup ----------

#define LoadDT 5
#define LoadSCK 4
#define servoPin 13

HX711 scale;
Servo myservo;

// ---------- Variables ----------

bool activated = true; // เปิดการให้อาหารอัติโนมัติไหม
bool isFeeding = false; // กำลัง feed หรือเปล่า

// -------------------------------

BLYNK_CONNECTED() {
  Blynk.virtualWrite(V2, 0); // กันเครื่อง feed เองหลัง connect
  Blynk.syncVirtual(V1);
}

// ตั้งค่า activated จาก Blynk
BLYNK_WRITE(V1) {
  activated = param.asInt();
  Serial.print("auto feeding: ");
  Serial.println(activated);
}

// Manual feed จาก Blynk
BLYNK_WRITE(V2) {
  int input = param.asInt();
  if (input) {
    fillFood();
    Serial.println("feed manually");
  }
}

void checkWeight() {
  // อ่านค่าน้ำหนักแล้วลบออก 33
  float rawWeight = scale.get_units(10);
  float w = rawWeight - 33.0; 

  // ป้องกันค่าน้ำหนักติดลบ (กรณีถาดเบากว่า 33g เล็กน้อย)
  if (w < 0) w = 0; 

  Serial.print("Weight (Net): ");
  Serial.println(w);

  // เงื่อนไข: ถ้าเหลือน้อยกว่า 25 กรัม (หลังจากหักถาดแล้ว) ให้เติมอาหาร
  if (w < 25.0 && activated && !isFeeding) { 
    fillFood();
    Serial.println("feed automatically");
  }

  Blynk.virtualWrite(V0, w); 
}

void fillFood() {
  if (isFeeding) return;
  isFeeding = true;
  myservo.write(180);
  timer.setTimeout(3000L, stopFilling);
}

void stopFilling() {
  isFeeding = false;
  myservo.write(90);
  Serial.println("Stop feeding");
}

void setup() {
  Serial.begin(115200);

  // Load cell setup
  scale.begin(LoadDT, LoadSCK);
  Serial.println("Initializing the scale...");
  scale.set_scale(1239.53f);     // ตั้งค่าเริ่มต้น
  scale.tare();          // รีเซ็ตค่าน้ำหนักเป็น 0
  Serial.println("Scale ready");

  // Servo setup
  myservo.attach(servoPin);

  // WiFi setup
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi Connected");

  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connect();

  timer.setInterval(5000L, checkWeight); // checkWeight() ทุกๆ 5 วินาที
}

void loop() {
  Blynk.run();
  timer.run();
}