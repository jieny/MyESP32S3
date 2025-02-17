#include <Arduino.h>
#include "WiFi.h"
#include <esp-ai.h>
#include <WebServer.h>
#include <Preferences.h> // 用于保存WiFi配置到NVS

using namespace std;

// led引脚
int led_pin = 18;
#define LED_BUILTIN 48
#define led 13 // 定义板载led等的控制引脚是13号

ESP_AI esp_ai;
WebServer server(80);
Preferences prefs;

// 替换为你的 Wi-Fi 名称和密码
const char *ssid = "Excellence";
const char *password = "Pa22446689";


// NTP服务器配置
const char *ntpServer1 = "ntp.aliyun.com";
const char *ntpServer2 = "pool.ntp.org";
const long gmtOffset_sec = 8 * 3600; // 北京时间UTC+8
const int daylightOffset_sec = 0;    // 无夏令时

String getTimestamp() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return "[时间未同步]";
    }
    char buffer[30];
    strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H:%M:%S] ", &timeinfo);
    return String(buffer);
}


void setup1() {
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


//    // 检查是否有保存的Wi-Fi配置信息
//    if (WiFi.status() != WL_CONNECTED) {
//        WiFi.begin();  // 尝试自动连接上次保存的Wi-Fi
//        Serial0.println("尝试连接已保存的WiFi...");
//
//        // 等待连接成功
//        int attempts = 0;
//        while (WiFi.status() != WL_CONNECTED && attempts < 20) {
//            delay(500);
//            Serial0.print(".");
//            attempts++;
//        }
//    }
//
//    // 如果无法自动连接Wi-Fi，则进入SmartConfig模式
//    if (WiFi.status() != WL_CONNECTED) {
//        Serial0.println("\n开始配网...");
//        WiFi.beginSmartConfig();  // 启动SmartConfig
//
//        while (!WiFi.smartConfigDone()) {
//            delay(500);
//            Serial0.print(".");
//        }
//        Serial0.println("配网成功");
//    }
//
//    // 确保连接成功
//    while (WiFi.status() != WL_CONNECTED) {
//        delay(500);
//        Serial0.print(".");
//    }
//    Serial0.println("\nWiFi已连接.");
//    Serial0.print("IP地址: ");
//    Serial0.println(WiFi.localIP());

    // [必  填] 是否调试模式， 会输出更多信息
    bool debug = true;
    // [必  填] wifi 配置： { wifi 账号， wifi 密码, "热点名字" } 可不设置，连不上wifi时会打开热点：ESP-AI，连接wifi后打开地址： 192.168.4.1 进行配网(控制台会输出地址，或者在ap回调中也能拿到信息)
    ESP_AI_wifi_config wifi_config = {"", "", "ESP-AI"};
    // [可 填] 服务配置： { 服务协议, 服务IP， 服务端口, "[可选] 请求参数" }
    ESP_AI_server_config server_config = {"http", "192.168.3.23", 8088};
    // [必  填] 唤醒方案： { 方案, 语音唤醒用的阈值(本方案忽略即可), 引脚唤醒方案(本方案忽略), 发送的字符串 }
    ESP_AI_wake_up_config wake_up_config = {"asrpro", 1, 10, "start"};
    // 启动
    esp_ai.begin({debug, wifi_config, server_config, wake_up_config});
}

// 配网页面HTML
const char *configPage = R"rawliteral(
<html><head><meta charset="UTF-8"><title>WiFi配置</title></head>
<body>
  <h1>选择WiFi网络</h1>
  <form action="/connect" method="POST">
    <input type="text" name="ssid" placeholder="WiFi名称"><br>
    <input type="password" name="pass" placeholder="密码"><br>
    <input type="submit" value="连接">
  </form>
</body></html>
)rawliteral";

void handleRoot() {
    server.send(200, "text/html", configPage); // 发送配网页面
}

void handleConnect() {
    String ssid = server.arg("ssid");
    String pass = server.arg("pass");

    if (ssid.length() == 0) {
        server.send(400, "text/plain", "SSID不能为空");
        return;
    }

    // 尝试连接WiFi
    WiFi.begin(ssid.c_str(), pass.c_str());
    int retry = 0;
    while (WiFi.status() != WL_CONNECTED && retry < 20) {
        delay(500);
        retry++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        // 保存到NVS
        prefs.begin("wifi-config");
        prefs.putString("ssid", ssid);
        prefs.putString("pass", pass);
        prefs.end();

        server.send(200, "text/plain", "连接成功! IP: " + WiFi.localIP().toString());
        WiFi.softAPdisconnect(true); // 关闭AP模式
        // 配置NTP时间服务
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
    } else {
        server.send(500, "text/plain", "连接失败");
    }
}

void setup() {
    // 初始化串口通信
    Serial0.begin(115200);
    Serial0.println("Serial0 Hello world by Serial 中文");
    printf("Hello world by printf 中文\n");

    // 读取已保存的WiFi配置
    prefs.begin("wifi-config");
    String savedSSID = prefs.getString("ssid", "");
    String savedPass = prefs.getString("pass", "");
    prefs.end();

    Serial0.println("Serial0 savedSSID " + savedSSID);
    Serial0.println("Serial0 savedPass " + savedPass);

    // 尝试自动连接
    if (savedSSID.length() > 0) {
        WiFi.begin(savedSSID.c_str(), savedPass.c_str());
        WiFi.waitForConnectResult();

        Serial0.println(" ");
        Serial0.println("Serial0 WiFi.status() --- " + WiFi.status());
        Serial0.println("Serial0 WiFi.status() --- " + WL_IDLE_STATUS);
        Serial0.println("Serial0 WiFi.status() --- " + WL_CONNECTED);

        if (WiFi.status() == WL_IDLE_STATUS) {

        }

        Serial.println("自动连接成功");
        Serial.println("IP: " + WiFi.localIP().toString());

        // 配置NTP时间服务
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

        return;
    }

    // 自定义 AP 名称（SSID）
    const char *apSSID = "ESP-AI";

    // 定义网段参数
    IPAddress local_ip(192, 168, 2, 1);    // AP 的 IP 地址
    IPAddress gateway(192, 168, 2, 1);     // 网关地址（通常与 AP IP 一致）
    IPAddress subnet(255, 255, 255, 0);    // 子网掩码

    // 配置 AP 的 IP 地址、网关和子网掩码
    WiFi.softAPConfig(local_ip, gateway, subnet);
    // 启动 AP 模式（不设置密码）
    // 启动配网AP
    WiFi.softAP(apSSID);

    Serial.println("AP模式已启动");
    Serial.println("AP IP: " + WiFi.softAPIP().toString());

    // 注册Web路由
    server.on("/", handleRoot);
    server.on("/connect", HTTP_POST, handleConnect);
    server.begin();

    // 打印 AP 的 IP 地址
    Serial0.println("AP 已启动");
    Serial0.print("AP SSID: ");
    Serial0.println(apSSID);
    Serial0.print("AP IP 地址: ");
    Serial0.println(WiFi.softAPIP());
}

void loop1() {
    server.handleClient();

    Serial0.println("loop Hello world by Serial 中文 要用 Serial0 111");
    Serial0.println(WiFi.localIP());
    printf("loop Hello world by printf 中文\n");

    delay(2000);

    digitalWrite(led, HIGH); // 设置led灯输出高电平
    delay(1000);             // 延时一秒
    digitalWrite(led, LOW);  // 设置led灯输出低电平
    delay(1000);             // 延时一秒
}

void loop() {
    server.handleClient();

    Serial0.println(WiFi.localIP());
    Serial0.println("loop Hello world by Serial 中文 要用 Serial0 111 " + getTimestamp());

    printf("loop Hello world by printf 中文\n");

    delay(2000);
}
