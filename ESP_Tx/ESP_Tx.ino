#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <esp_now.h>
#include <WiFi.h>
#include "soc/soc.h"
#include "soc/rtc.h"

int rand_int;
float test_float;
bool bool_value = true;

uint8_t broadcastAddress[] = {0x8C,0x4B,0x14,0x15,0xDC,0x54};

//WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;

struct_message myData;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *max_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA);
  //Serial.println(WiFi.macAddress());

  if(esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  rand_int = random(1, 20);
  test_float = 1.3 * rand_int;
  bool_value = !bool_value;
  strcpy(myData.a, "Welcome to the Workshop!");
  myData.b = rand_int;
  myData.c = test_float;
  myData.d = bool_value;

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sending confirmed");
  }
  else {
    Serial.println("Sending error");
  }
  delay(2000);
}
