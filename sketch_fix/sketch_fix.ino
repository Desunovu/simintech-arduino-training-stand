#define CLK 2
#define DT 4

#include "EncButton.h"

EncButton<EB_TICK, CLK, DT> enc;

byte i, j;
byte p, k;
byte os;
byte EncIsRight;
byte EncIsLeft;

uint32_t myTimer;
int period = 100; 

void setup() {
  myTimer = millis();

  Serial.begin(9600);

  pinMode(A4, INPUT_PULLUP);
  pinMode(A2, INPUT);
  pinMode(A6, INPUT);
  pinMode(3, OUTPUT);
  digitalWrite(3, 0);
}

void loop() {


  // обязательная функция отработки. Должна постоянно опрашиваться
  enc.tick();

  // Проверка на факт движения энкодера
  if (enc.right()) {
    EncIsRight = 1;
  }

  if (enc.left()) {
    EncIsLeft = 1;
  }


  // делает какую то работу с кнопкой
  if ( digitalRead(A4) == 0  &&  j == 0) {
    i = !i;
    j = 1;
  }

  // делает какую то работу с кнопкой
  if ( digitalRead(A4) == 1 && j == 1) {
    j = 0;
  }

  //Считывание с аналогового порта 0, 1023 и производится преобразование к байтовым значениям A, B, 
  p = map(analogRead(A2), 0, 1023, 0, 255);
  k = map(analogRead(A6), 0, 1023, 0, 100); // приводится не к 0-255, а к 0-100, просто потому что так надо

  //Отправление данных по таймеру
  if (millis() - myTimer >= period) {
    myTimer += period;

    //Чтение из порта
    if (Serial.available() != 0) {
      os = Serial.read();
      if ((os < p + 10 && os > p - 10) || i == 0) {
        digitalWrite(3, 0);
      }
      else {
        digitalWrite(3, 1);
      }
    }

    // Передача в порт обмена данными с SimInTech
    Serial.write(i);
    Serial.write(p);
    Serial.write(k);
    Serial.write(EncIsRight);
    Serial.write(EncIsLeft);

    //Обнуление после отправки
    EncIsRight = 0;
    EncIsLeft = 0;
  }

}
