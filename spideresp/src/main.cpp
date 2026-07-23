#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "spider_con.h"

#ifndef PIN_TFT_SCK
    #define PIN_TFT_SCK 12
#endif
#ifndef PIN_TFT_MOSI
    #define PIN_TFT_MOSI 11
#endif
#ifndef PIN_TFT_DC
    #define PIN_TFT_DC 21
#endif
#ifndef PIN_TFT_CS
    #define PIN_TFT_CS 47
#endif
#ifndef PIN_TFT_RST
    #define PIN_TFT_RST 20
#endif
#ifndef PIN_TFT_BL
    #define PIN_TFT_BL 10
#endif

// ================================================================
// 舵机 GPIO 配置（ESP32-S3 关键：避开 strapping / flash / TFT 占用
//
//   旧版本 {4,5,6,7,15,16,17,18} —— 问题：
//     GPIO4/GPIO5 是 strapping；GPIO6/GPIO7 常连接模组内部 FLASH
//     GPIO12/TFT_SCK 等已被 SPI 占用；不建议
//   新的一组 —— 通用、稳妥（远离 strapping、不与 TFT/USB 冲突）：
//     {1, 2, 6, 7, 13, 14, 15, 17}
//   如果你的模组上某些 GPIO 仍被其它外设占用，按实际换成其他空闲 GPIO。
// ================================================================
static const int kServoPins[8] = {1, 2, 6, 7, 13, 14, 15, 17};
static const int kServoCh[8]   = {0, 1, 2, 3, 4, 5, 6, 7};

#define SERVO_MIN_US     500UL
#define SERVO_MAX_US    2500UL
#define SERVO_PERIOD_US 20000UL   // 50Hz 周期 20ms
#define SERVO_FREQ_HZ    50

// 10 bit 分辨率：2^10 = 1024。对 50Hz S3 稳定；
// 若想更高分辨率可用 12 bit (4096) 也可，保持 10 bit 更稳。
#define LEDC_RES        10
#define LEDC_MAX_VAL    (1UL << LEDC_RES)

// TFT
Adafruit_ST7735 tft(&SPI, PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);

// 外部声明
void setServo(int index, int angle);

void setup()
{
    Serial.begin(115200);
    delay(200);
    Serial.println();
    Serial.println("===== 8-servo spider init =====");

    // 初始化 8 路 LEDC 舵机
    for (int i = 0; i < 8; ++i) {
        // ESP32-S3：推荐用 10 bit + 50Hz，timer 自动分配。
        bool ok = ledcSetup(kServoCh[i], SERVO_FREQ_HZ, LEDC_RES);
        ledcAttachPin(kServoPins[i], kServoCh[i]);
        Serial.printf("Servo[%d] GPIO %d -> LEDC ch %d  setup=%d\r\n",
                    i, kServoPins[i], kServoCh[i], ok ? 1 : 0);
        // 先给每个通道一个居中 90°，避免“上电爆冲”
        setServo(i, 90);
    }
    delay(500);

    // TFT 背光
    pinMode(PIN_TFT_BL, OUTPUT);
    digitalWrite(PIN_TFT_BL, LOW);

    // 推荐：使用ESP32 HSPI/VSPI，避免和LEDC、GPIO冲突
    SPI.begin(PIN_TFT_SCK, -1, PIN_TFT_MOSI);
    tft.initR(INITR_BLACKTAB);
    tft.setRotation(1);
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);
    tft.setCursor(10, 20);
    tft.print("TFT OK");
    digitalWrite(PIN_TFT_BL, HIGH);

    Serial.println("Init done. Power-check your  外部5V>=2A + GND 共地！");
    Serial.println("若舵机没动，请参考：先单路舵机调试；再逐路验证。");
    Serial.println("==================================");
}

// 标准角度转 LEDC 计数值
void setServo(int index, int angle)
{
    if (index < 0 || index >= 8) return;
    angle = constrain(angle, 0, 180);

    // 角度 -> 脉冲宽度 us
    uint32_t pulseUs = map((uint32_t)angle, (uint32_t)0, (uint32_t)180,
                           SERVO_MIN_US, SERVO_MAX_US);
    // 脉冲宽度 -> LEDC 10bit 计数值
    uint32_t duty = pulseUs * LEDC_MAX_VAL / SERVO_PERIOD_US;

    ledcWrite(kServoCh[index], (uint32_t)duty);
}

// ---------------------------------------------------------------
// 最小化单舵机测试函数：逐路来回扫描，方便硬件排查
//   建议先注释掉 spider_check()，只跑这个，观察串口 + 舵机动作
// ---------------------------------------------------------------
static void singleServoSweep(int ch)
{
    Serial.printf("[single test] ch=%d sweep 45->135\r\n", ch);
    for (int ang = 45; ang <= 135; ang += 3) {
        setServo(ch, ang);
        delay(30);
    }
    for (int ang = 135; ang >= 45; ang -= 3) {
        setServo(ch, ang);
        delay(30);
    }
    setServo(ch, 90);
}

// ================================================================
// 主循环：建议按 3 步来调试
//   阶段 1：单舵机 sweep（把下面的 #if 1 保留）
//   阶段 2：8 路同时扫一遍（loop 里只跑两组姿态）
//   阶段 3：调用 spider_forward / spider_backward
// ================================================================
void loop()
{
    //spider_check();
    
    // 姿态1 / 姿态2（阶段 2 用，上面单路扫描跑完再启用）
    setServo(0, 90);  setServo(1, 90); setServo(2, 90); setServo(3, 90);
    setServo(4, 90);  setServo(5, 90);  setServo(6, 90);  setServo(7, 90);
    delay(500);
    //
    // setServo(0, 90);  setServo(1, 90);  setServo(2, 90);  setServo(3, 90);
    // setServo(4, 0);   setServo(5, 180); setServo(6, 180); setServo(7, 0);
    // delay(500);
}