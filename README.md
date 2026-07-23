# 四足机器人前进步态研究

## 项目介绍
本项目主控已升级为 ESP32-S3，使用 8 路舵机驱动实现四足机器人（蜘蛛形态）的基础步态控制，当前代码重点实现了前进与后退动作序列，并提供TFT屏幕状态显示。项目使用 PlatformIO + Arduino 框架开发。

项目位于 spideresp 目录，可直接在 vscode 中打开工程文件进行编译和烧录。

<img src="assets/spider.jpg" width="600" height="400" alt="实际图像">

## 项目特性

- **主控**: ESP32-S3 (Arduino 框架)
- **舵机控制**: 8 路 SG90 舵机，使用 ESP32 LEDC 硬件 PWM
- **显示**: 支持 TFT 屏幕显示 (Adafruit ST7735/ST7789)
- **步态控制**: 前进、后退、自检等基础步态
- **供电**: 5V/2A+ 外置电源（关键！见下文说明）


## 硬件清单

| 组件 | 数量 | 说明 |
|---|---|---|
| ESP32-S3 DevKit | 1 | 主控板 |
| SG90 舵机 | 8 | 每足 2 个（胯部+肩部） |
| 5V 稳压电源 | 1 | **必须 2A 以上**（推荐 3A） |
| TFT 屏幕 | 1 | ST7735 或 ST7789 (可选) |
| 杜邦线 | 若干 | 信号线、电源线 |

## 接线说明

### 舵机引脚分配

```
Servo[0] -> GPIO1   (左肩)
Servo[1] -> GPIO2   (右肩)
Servo[2] -> GPIO6   (左肩)
Servo[3] -> GPIO7   (右肩)
Servo[4] -> GPIO13  (左胯)
Servo[5] -> GPIO14  (右胯)
Servo[6] -> GPIO15  (左胯)
Servo[7] -> GPIO17  (右胯)
```

**重要提示**: ESP32-S3 的 GPIO4/GPIO5 是 strapping 引脚，GPIO6/GPIO7 在部分模组上连接内部 Flash，请避免使用这些引脚！

## 软件依赖

| 库 | 版本 | 说明 |
|---|---|---|
| Adafruit ST7735 and ST7789 Library | ^1.10.4 | TFT 屏幕驱动 |
| Adafruit GFX Library | ^1.11.9 | 图形库 |

## 开发环境

- **IDE**: PlatformIO (VSCode)
- **框架**: Arduino
- **目标板**: esp32-s3-devkitc-1

## 目前问题
多个舵机同时运动时可能出现供电不足导致的抖动或停转

蓝牙通讯未完善

### 联系我们
点击 [我的博客](https://mingchuangyinye.shop/personalpage) 与我们取得联系

