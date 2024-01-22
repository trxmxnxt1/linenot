#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL6YwVpZ-37"
#define BLYNK_TEMPLATE_NAME "linenotify"
#define BLYNK_AUTH_TOKEN "Ym6BgvR0Ov0kZPUnz_VIiQHMfyKBHe6o"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <TridentTD_LineNotify.h>


#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define LINE_TOKEN  "S5grUBCC70yrA41XZBkqdx6Ma2Um59hyz7KatOh8zWX"
#define sw1 15

char ssid[] = "Luv";//ปล่อยสัญญาณจากมือถือ
char pass[] = "12345678";

BLYNK_CONNECTED() {
  Blynk.syncAll();
}

boolean flag = 0;
unsigned long pre = 2000;

void setup() {
  Serial.begin(115200);
  pinMode(sw1, INPUT_PULLUP);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  dht.begin();

  LINE.setToken(LINE_TOKEN);

}

void loop() {
  Blynk.run();
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.print("h="); Serial.println(h);
  Serial.print("t="); Serial.println(t);
  Serial.print("f="); Serial.println(f);
  int vr1;

  if (digitalRead(sw1) == LOW) {

    if(flag ==0){ flag = 1;}
    else{ flag=0;}
    while (digitalRead(sw1) == LOW) {
      delay(2);
    }
  }

  if (millis() - pre > 2000) { //ส่งข้อมูลทุก 1 วินาที
    pre = millis();
    Blynk.virtualWrite(V0, t);
  }
  if (t >= 30) {
    if (flag == 1) {
      LINE.notify("HEAT OVER");
      Serial.println("line on");
    }
    Serial.print("HEAT OVER");
    Blynk.virtualWrite(V1, 1);
  }
  else {
    Serial.println("line off");
    Blynk.virtualWrite(V1, 0);
  }

}
