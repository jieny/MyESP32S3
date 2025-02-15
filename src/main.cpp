#include "web_api.h"
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFi.h>
// https://github.com/tzapu/WiFiManager
#include <WiFiManager.h>

using namespace std;

// led引脚
int led_pin = 18;
#define LED_BUILTIN 48
#define led 13 // 定义板载led等的控制引脚是13号

// 替换为你的 Wi-Fi 名称和密码
const char* ssid = "Excellence";
const char* password = "Pa22446689";

WiFiManager wm;
WiFiManagerParameter custom_mqtt_server("server", "mqtt server", "", 40);

void saveParamsCallback () {
    Serial0.println("Get Params:");
    Serial0.print(custom_mqtt_server.getID());
    Serial0.print(" : ");
    Serial0.println(custom_mqtt_server.getValue());
}

void setup()
{
    // 初始化串口通信
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

    //reset settings - wipe credentials for testing
    //wm.resetSettings();
    wm.addParameter(&custom_mqtt_server);
    wm.setConfigPortalBlocking(false);
    wm.setSaveParamsCallback(saveParamsCallback);

    //automatically connect using saved credentials if they exist
    //If connection fails it starts an access point with the specified name
    if(wm.autoConnect("AutoConnectAP")){
        Serial0.println("connected...yeey :)");
    }
    else {
        Serial0.println("Configportal running");
    }
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
