#include <esp_now.h>
#include <WiFi.h>

//WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;

struct_message myData;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Data received: ");
  Serial.println(len);
  Serial.print("Character Value: ");
  Serial.println(myData.a);
  Serial.print("Integer Value: ");
  Serial.println(myData.b);
  Serial.print("Float Value: ");
  Serial.println(myData.c);
  Serial.print("Boolean Value: ");
  Serial.println(myData.d);
  Serial.println();
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if(esp_now_init() != ESP_OK) {
    Serial.println("Error initiating ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {

}
