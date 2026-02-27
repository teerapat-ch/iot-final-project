#include "HX711.h"
#include <Servo.h>

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
#define buzzPin 2  // Buzzer

HX711 scale;
Servo myservo;

bool activated = true; // เปิดการให้อาหารอัติโนมัติไหม (เดี๋ยวคุยอีกทีว่าจะมีมั้ย)

void checkWeight() {
    float w = scale.get_units(10);
    Serial.print("Weight: ");
    Serial.println(w);

    if (currentWeight < 5.0) { // เหลือแค่น้ำหนักถาด
      fillFood();
    }
}

void fillFood() {
  tone(buzzPin, 1000, 1000);

  // Servo code
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

  // Blynk setup
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(600000L, checkWeight); // checkWeight() ทุกๆ 10 นาที
}

void loop() {
  Blynk.run();
  timer.run();
}