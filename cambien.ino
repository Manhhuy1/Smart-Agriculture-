/**
 * Created by @Author: Mai Manh Huy
 * Project : Smart Agritecture Using IoT System.
 * Create Time : 23:22 - 23/10/2022
 * For all issue contact me : huymm98@gmail.com
 * IoT node Sensor using Arduino Uno 
 */
#include "DHT.h"
const int DHTPIN = 4;
const int DHTTYPE = DHT11;
#include "LoRa_E32.h"
#include "Arduino.h"
#define pin_analog A0
#define pin_digital 2
#define pin_led 13
#include <stdlib.h>
#define POWER_PIN  7
#define SIGNAL_PIN A5

DHT dht(DHTPIN, DHTTYPE);
LoRa_E32 e32ttl100(2, 3);          //chân 2,3 truyển nhận dữ liệu với LoRa

int relaybom=5;      //chân relay 5 điều khiển bơm
int relayden=6;      //chân relay 6 điều khiển đèn 
int doccb_dat; 
int anhsang;
int muc_nuoc = 0;
void setup(){
  Serial.begin(9600);
  e32ttl100.begin();
  dht.begin();

  pinMode (pin_analog, INPUT);
  pinMode(pin_digital, INPUT);
  pinMode (pin_led, OUTPUT);
  pinMode(10, INPUT);
  pinMode(relaybom, OUTPUT);
  digitalWrite(relaybom, LOW);
  pinMode(relayden, OUTPUT);
  digitalWrite(relayden, LOW);
}

void loop() {
  int h = dht.readHumidity(); 
  int t = dht.readTemperature();
     doccb_dat= analogRead(pin_analog);
     int phantramao = map(doccb_dat, 0, 1023, 0, 100);      //Chuyển giá trị sang %
     int phantramthuc = 100 - phantramao;
     anhsang = digitalRead(10);
     if (anhsang == 1)
     Serial.println("Troi toi");
     else Serial.println("Troi sang");

     muc_nuoc = analogRead(SIGNAL_PIN);      //doc gia tri cam bien muc nuoc
     
  Serial.print("Nhiet do: ");            //in ra giá trị nhiệt độ
  Serial.println(t);               
  Serial.print("Do am: ");               // in ra giá trị độ ẩm không khí
  Serial.println(h); 
  Serial.print("Do am dat: ");               // in ra giá trị độ ẩm đất
  Serial.print(phantramthuc);         
  Serial.println("%");
  int den=0,bom=0;
  long num=0;

  if ( phantramthuc <= 40 && muc_nuoc >= 200)      //dieu kien bat may bom khi dat kho
    {
      digitalWrite(relaybom, LOW);
      bom=1;
    }
    else
    {
      digitalWrite(relaybom, HIGH);
      bom=0;
    }

  if ( anhsang == 1 )                  // dieu kien bat den khi troi toi
  {
     digitalWrite(relayden, LOW);
     den=1;
  }
  else
  {
     digitalWrite(relayden, HIGH);
     den=0;
  }

 if ( muc_nuoc >= 200 )          // dieu kien tat may bom khi het nuoc
  {
      Serial.println("Muc nuoc cao");
    }
    else
    {
      Serial.println("Muc nuoc thap");
    } 
    Serial.println(); 
  num = h*1000000 + phantramthuc*10000 + t*100 +den*10 + bom ;
  delay(1000); 
  char cstr[8];
  ltoa(num, cstr, 10);
  Serial.println(cstr); 
  ResponseStatus rs = e32ttl100.sendMessage(cstr);
}
