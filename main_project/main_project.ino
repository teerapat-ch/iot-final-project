#include "HX711.h"
#include <ESP32Servo.h>

// ---------- Blynk setup ----------

#define BLYNK_TEMPLATE_ID "TMPL6Tg7kQjXZ"
#define BLYNK_TEMPLATE_NAME "IoT Final Project"
#define BLYNK_AUTH_TOKEN "YMZqGQiciOKl5zzoBaPVM4EprpYGrlkd"

#include <BlynkSimpleEsp32.h>

BlynkTimer timer;

char ssid[] = "darkblue";
char pass[] = "bluedark";

// ---------- Sensors setup ----------

#define LoadDT  21   // ขา DT ต่อกับ GPIO 21
#define LoadSCK 22   // ขา SCK ต่อกับ GPIO 22
#define servoPin 10 // Motor

HX711 scale;
Servo myservo;

// ---------- Variables ----------

bool activated = true; // เปิดการให้อาหารอัติโนมัติไหม (เดี๋ยวคุยอีกทีว่าจะมีมั้ย)
bool isFeeding = false;

// -------------------------------

void checkWeight() {
  float w = scale.get_units(10);
  Serial.print("Weight: ");
  Serial.println(w);

  if (w < 5.0 && activated && !isFeeding) { // เหลือแค่น้ำหนักถาด
    fillFood();
  }

  Blynk.virtualWrite(V0, w); // ส่งข้อมูล Weight ขึ้น Blynk
}

void fillFood() {
  isFeeding = true;
  myservo.write(90);
  timer.setTimeout(3000L, stopFilling);
}

void stopFilling() {
  isFeeding = false;
  myservo.write(0);
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

  // Blynk setup
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(5000L, checkWeight); // checkWeight() ทุกๆ 5 วินาที
}

void loop() {
  Blynk.run();
  timer.run();
}