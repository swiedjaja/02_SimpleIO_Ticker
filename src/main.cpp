// Solution 1: Use ticker to call function every 1sec, avoid call delay di loop()
#include <Arduino.h>
#include <Ticker.h>
#include "device.h"

const uint8_t arLed[3] = {LED_RED, LED_YELLOW, LED_GREEN};

Ticker timer1Sec;
Ticker ledOff;
int nCount = 0;

void OnTimer1Sec()
{
  Serial.printf("Hello World %d\n", nCount);
  digitalWrite(arLed[nCount%3], LED_ON);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_ON);
#if defined(ESP32)    
  delay(100);
  digitalWrite(arLed[nCount%3], LED_OFF);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_OFF);
  nCount++;
#endif  
#ifdef defined(ESP8266)
  // avoid call delay in ESP8266, nyala led akan redup
  // this better solution for ESP8266 and ESP32
  ledOff.once_ms(100, [](){
    digitalWrite(arLed[nCount%3], LED_OFF);
    digitalWrite(LED_BUILTIN, LED_BUILTIN_OFF);
    nCount++;
  });
#endif  
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
#if defined(LED_FLASH)    
  pinMode(LED_FLASH, OUTPUT);
#endif  
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(PIN_SW, INPUT);

  Serial.printf("Board: %s\n", ARDUINO_BOARD);
  Serial.println("System ready...");  
  timer1Sec.attach(1, OnTimer1Sec);
}

void loop() {
  if (digitalRead(PIN_SW)==0)
  {
#if defined(LED_FLASH)    
    for (uint8_t i=0; i<2; i++)
    {
      digitalWrite(LED_FLASH, LED_ON);
      delay(3);
      digitalWrite(LED_FLASH, LED_OFF);
      delay(200);
    }
#endif    
    nCount=0;
  }
  // yield();
}