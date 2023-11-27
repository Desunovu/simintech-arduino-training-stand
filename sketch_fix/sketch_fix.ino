#define PIN_POT_1 A0 //потенциометр 1
#define PIN_POT_2 A1 //потенциометр 2

#define PIN_VRX_1 A2 //Джойстик 1 (oX)
#define PIN_VRY_1 A3 //Джойстик 1 (oY)
#define PIN_VRBUTTON_1 12 //Джойстик 1 (кнопка)

#define PIN_VRX_2 A4 //Джойстик 2 (oX)
#define PIN_VRY_2 A5 //Джойстик 2 (oY)
#define PIN_VRBUTTON_2 13 //Джойстик 2 (кнопка)

#define PIN_BUTTON_1 10 //Кнопка 1
#define PIN_BUTTON_2 11 //Кнопка 2

#define PIN_LED_1 2 //LED 1
#define PIN_LED_2 3 //LED 2
#define PIN_LED_3 4 //LED 3
#define PIN_LED_4 5 //LED 4
#define PIN_LED_5 6 //LED 5
#define PIN_LED_6 7 //LED 6
#define PIN_LED_7 8 //LED 7
#define PIN_LED_8 9 //LED 8

uint32_t myTimer; // Текущее время с момента старта
int period = 100; // Период отправки пакетов
int limit = 255; // Верхняя граница приведения значений (по умолчанию 255)

// Переменные для считанных показаний, приведенные к значениям в приделах 1 байта
byte pot1, pot2, vrx1, vry1, vrx2, vry2; //Потенциометр 1-2, Джойстик1 x-y, Джойстик2 x-y

// Переменные цифрового состояния кнопок
int vrbutton1, vrbutton2, button1, button2;

// Байтовая переменная состояния кнопок
byte buttonStatus; //Побитно: [_, _, _, _, vrbutton2, vrbutton1, button2, button1]

// Переменные для полученных данных (состояние светодиодов)
byte ledStatus; //Побитно: [LED8, LED7, LED6, LED5, LED4, LED3, LED2, LED1]

// Функция чтения значений с пинов
// analogRead возвращает значения от 0 до 1023, которые надо привести к значениям в пределах 1 байта
void readData() {
  pot1=map(analogRead(PIN_POT_1), 0, 1023, 0, limit);
  pot2=map(analogRead(PIN_POT_2), 0, 1023, 0, limit);
  vrx1=map(analogRead(PIN_VRX_1), 0, 1023, 0, limit);
  vry1=map(analogRead(PIN_VRY_1), 0, 1023, 0, limit);
  vrx2=map(analogRead(PIN_VRX_2), 0, 1023, 0, limit);
  vry2=map(analogRead(PIN_VRY_2), 0, 1023, 0, limit);

  button1=digitalRead(PIN_BUTTON_1);
  button2=digitalRead(PIN_BUTTON_2);
  vrbutton1=digitalRead(PIN_VRBUTTON_1);
  vrbutton2=digitalRead(PIN_VRBUTTON_2);
  
  buttonStatus=0;
  buttonStatus |= (button1 << 0); // Запись состояния кнопки button1 в bit_0
  buttonStatus |= (button2<<1); // bit_1
  buttonStatus |= (vrbutton1<<2); // bit_2
  buttonStatus |= (vrbutton2<<3); // bit_3
}

// Функция получения данных
void getData() {
  if (Serial.available() != 0) {
    ledStatus = Serial.read();
  }
  // [TEST] Записать все биты как 1
  // ledStatus = 0xFF;
}

// Функция подачи напряжения на пины светодиодов
void setLed() {
  digitalWrite(PIN_LED_1, ledStatus&(1<<0));
  digitalWrite(PIN_LED_2, ledStatus&(1<<1));
  digitalWrite(PIN_LED_3, ledStatus&(1<<2));
  digitalWrite(PIN_LED_4, ledStatus&(1<<3));
  digitalWrite(PIN_LED_5, ledStatus&(1<<4));
  digitalWrite(PIN_LED_6, ledStatus&(1<<5));
  digitalWrite(PIN_LED_7, ledStatus&(1<<6));
  digitalWrite(PIN_LED_8, ledStatus&(1<<7));
}

// Функция отправки данных
void sendData() {
    Serial.write(pot1);
    Serial.write(pot2);
    Serial.write(vrx1);
    Serial.write(vry1);
    Serial.write(vrx2);
    Serial.write(vry2);
    Serial.write(buttonStatus);
    // [TEST] Отправить в сериал порт символами:
    //  Serial.print(buttonStatus);
    // Serial.println();
}

void setup() {
  myTimer = millis();
  Serial.begin(9600);

  // Настройка пинов с LED на выход
  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);
  pinMode(PIN_LED_3, OUTPUT);
  pinMode(PIN_LED_4, OUTPUT);
  pinMode(PIN_LED_5, OUTPUT);
  pinMode(PIN_LED_6, OUTPUT);
  pinMode(PIN_LED_7, OUTPUT);
  pinMode(PIN_LED_8, OUTPUT);
}

void loop() {
  if (millis() - myTimer >= period) {
    myTimer += period;

    getData(); // Получаем данные из SimInTech
    setLed(); // Переводим светодиоды в переданное состояние
    readData(); // Считываем данные с аналоговых и цифровых пинов
    sendData(); // Передаем данные в SimInTech
  }

}
