#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

uint8_t espAddress[] = { 0x54, 0x43, 0xB2, 0xC3, 0x1C, 0x84 };  //ESP32 Micro ku

typedef struct struct_message {
  int sensorData;
} struct_message;

struct_message myData;

int waitingTime;

// Callback when data is sent
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // Serial.print("\r\nLast Packet Send Status: ");
  // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Track Your Hand!");
  lcd.setCursor(0, 1);
  lcd.print("     Ready!     ");

  WiFi.mode(WIFI_STA);

  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

  if (esp_now_init() != ESP_OK) {
    return;
  }

  esp_now_register_send_cb(onDataSent);

  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, espAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    return;
  }
}

void loop() {
  if (Serial.available()) {
    String receivedData = Serial.readString();
    int numRecv = receivedData.toInt();
    lcd.setCursor(0, 1);
    lcd.print("       ");
    lcd.setCursor(2, 1);
    lcd.print(numRecv);
    if (numRecv < 6) {
      waitingTime = 0;
      myData.sensorData = numRecv;  // Membaca sensor
      esp_err_t result = esp_now_send(espAddress, (uint8_t *)&myData, sizeof(myData));
      if (result == ESP_OK) {
        lcd.setCursor(7, 1);
        lcd.print("Success   ");
      } else {
        lcd.setCursor(7, 1);
        lcd.print("Failed    ");
      }
    }
    else waitingTime = 300;
    delay(waitingTime);
  }
}
