#include "web_api.h"
#include <Arduino.h>
#include <ArduinoOTA.h>

using namespace std;

// led引脚
int led_pin = 18;
#define LED_BUILTIN 48
#define led 13 // 定义板载led等的控制引脚是13号

void setup()
{
    Serial.begin(115200);
    Serial.println("Serial Hello world by Serial 中文");
    Serial0.begin(115200);
    Serial0.println("Serial0 Hello world by Serial 中文");
    Serial1.begin(115200);
    Serial1.println("Serial1 Hello world by Serial 中文");
    Serial2.begin(115200);
    Serial2.println("Serial2 Hello world by Serial 中文");
    printf("Hello world by printf 中文\n");
    // led 指示灯
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    pinMode(led, OUTPUT); // 设置led的工作模式为输出模式
}

void loop()
{
    Serial0.println("loop Hello world by Serial 中文 要用 Serial0 111");
    printf("loop Hello world by printf 中文\n");

    delay(2000);

    digitalWrite(led, HIGH); // 设置led灯输出高电平
    delay(1000);             // 延时一秒
    digitalWrite(led, LOW);  // 设置led灯输出低电平
    delay(1000);             // 延时一秒
}
