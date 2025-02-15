#include "web_api.h"
#include <Arduino.h>
#include <ArduinoOTA.h>
#include "WiFi.h"

using namespace std;

// led引脚
int led_pin = 18;
#define LED_BUILTIN 48
#define led 13 // 定义板载led等的控制引脚是13号

// 替换为你的 Wi-Fi 名称和密码
const char* ssid = "Excellence";
const char* password = "Pa22446689";

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


    // 检查是否有保存的Wi-Fi配置信息
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.begin();  // 尝试自动连接上次保存的Wi-Fi
        Serial0.println("尝试连接已保存的WiFi...");

        // 等待连接成功
        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < 20) {
            delay(500);
            Serial0.print(".");
            attempts++;
        }
    }

    // 如果无法自动连接Wi-Fi，则进入SmartConfig模式
    if (WiFi.status() != WL_CONNECTED) {
        Serial0.println("\n开始配网...");
        WiFi.beginSmartConfig();  // 启动SmartConfig

        while (!WiFi.smartConfigDone()) {
            delay(500);
            Serial0.print(".");
        }
        Serial0.println("配网成功");
    }

    // 确保连接成功
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial0.print(".");
    }
    Serial0.println("\nWiFi已连接.");
    Serial0.print("IP地址: ");
    Serial0.println(WiFi.localIP());
}

void loop()
{
    Serial0.println("loop Hello world by Serial 中文 要用 Serial0 111");
    Serial0.println(WiFi.localIP());
    printf("loop Hello world by printf 中文\n");

    delay(2000);

    digitalWrite(led, HIGH); // 设置led灯输出高电平
    delay(1000);             // 延时一秒
    digitalWrite(led, LOW);  // 设置led灯输出低电平
    delay(1000);             // 延时一秒
}
