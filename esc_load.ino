#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#define IN1 14  // Pin untuk mengontrol arah motor
#define IN2 27  // Pin untuk mengontrol arah motor
#define ENA 26  // Pin untuk PWM kontrol kecepatan motor

LiquidCrystal_I2C lcd(0x27, 20, 4);

typedef struct struct_message {
  int recData;
} struct_message;

struct_message myData;

int savedValue, eepromAddress = 0;

// Callback when data is received
void onDataRecv(const esp_now_recv_info *mac_addr, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.println(myData.recData);
  if (myData.recData == 1) {
    saveValueToEEPROM(1);
    lcd.setCursor(0, 3);
    lcd.print("|     Very Low     |");
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 159);
    delay(100);
    analogWrite(ENA, 127);
  } else if (myData.recData == 2) {
    saveValueToEEPROM(2);
    lcd.setCursor(0, 3);
    lcd.print("|       Low        |");
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 159);
  } else if (myData.recData == 3) {
    saveValueToEEPROM(3);
    lcd.setCursor(0, 3);
    lcd.print("|      Medium      |");
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 191);
  } else if (myData.recData == 4) {
    saveValueToEEPROM(4);
    lcd.setCursor(0, 3);
    lcd.print("|       High       |");
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 223);
  } else if (myData.recData == 5) {
    saveValueToEEPROM(5);
    lcd.setCursor(0, 3);
    lcd.print("|     Very High    |");
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 255);
  } else if (myData.recData == 0) {
    saveValueToEEPROM(0);
    lcd.setCursor(0, 3);
    lcd.print("|       Stop       |");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
  }
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  lcd.begin(20, 4);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("|    Effortless    |");
  lcd.setCursor(0, 1);
  lcd.print("| Speed Controller |");
  lcd.setCursor(0, 2);
  lcd.print("|------------------|");
  lcd.setCursor(0, 3);
  lcd.print("|       Stop       |");

  savedValue = EEPROM.read(eepromAddress);
  if (savedValue == -1) savedValue = 0;
  else {
    switch (savedValue) {
      case 0:
        lcd.setCursor(0, 3);
        lcd.print("|       Stop       |");
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        analogWrite(ENA, 0);
        break;
      case 1:
        lcd.setCursor(0, 3);
        lcd.print("|     Very Low     |");
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(ENA, 159);
        delay(100);
        analogWrite(ENA, 127);
        break;
      case 2:
        lcd.setCursor(0, 3);
        lcd.print("|       Low        |");
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(ENA, 159);
        break;
      case 3:
        lcd.setCursor(0, 3);
        lcd.print("|      Medium      |");
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(ENA, 191);
        break;
      case 4:
        lcd.setCursor(0, 3);
        lcd.print("|       High       |");
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(ENA, 223);
        break;
      case 5:
        lcd.setCursor(0, 3);
        lcd.print("|     Very High    |");
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(ENA, 255);
        break;
    }
  }

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

  if (esp_now_init() != ESP_OK) {
    return;
  }

  esp_now_register_recv_cb(onDataRecv);
}

void loop() {
}

void saveValueToEEPROM(int x) {
  EEPROM.write(eepromAddress, x);
  EEPROM.commit();
}