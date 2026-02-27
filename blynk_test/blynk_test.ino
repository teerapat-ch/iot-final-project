#define BLYNK_TEMPLATE_ID "TMPL6Tg7kQjXZ"
#define BLYNK_TEMPLATE_NAME "IoT Final Project"
#define BLYNK_AUTH_TOKEN "YMZqGQiciOKl5zzoBaPVM4EprpYGrlkd"

#include <BlynkSimpleEsp32.h>

BlynkTimer timer;

char ssid[] = "darkblue";
char pass[] = "bluedark";

void myTimer() {
  Blynk.virtualWrite(V0, 100);
}

void setup()
{
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(600000L, myTimer);
}

void loop()
{
  Blynk.run();
  timer.run();
}

