#include <Arduino.h>
#include "WiFi.h"
#include <esp-ai.h>

using namespace std;

// led引脚
int led_pin = 18;
#define LED_BUILTIN 48
#define led 13 // 定义板载led等的控制引脚是13号

ESP_AI esp_ai;

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

    // [必  填] 是否调试模式， 会输出更多信息
    bool debug = true;
    // [必  填] wifi 配置： { wifi 账号， wifi 密码, "热点名字" } 可不设置，连不上wifi时会打开热点：ESP-AI，连接wifi后打开地址： 192.168.4.1 进行配网(控制台会输出地址，或者在ap回调中也能拿到信息)
    ESP_AI_wifi_config wifi_config = {"test2", "12345678", "ESP-AI"};
    // [可 填] 服务配置： { 服务协议, 服务IP， 服务端口, "[可选] 请求参数" }
    ESP_AI_server_config server_config = {"http", "192.168.3.23", 8088};
    // [必  填] 唤醒方案： { 方案, 语音唤醒用的阈值(本方案忽略即可), 引脚唤醒方案(本方案忽略), 发送的字符串 }
    ESP_AI_wake_up_config wake_up_config = { "asrpro", 1, 10, "start" };
    // 启动
    esp_ai.begin({debug, wifi_config, server_config, wake_up_config });
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
