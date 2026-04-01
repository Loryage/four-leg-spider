# 四足蜘蛛机器人 main 与 Hardware 资源分析

## 1. 分析范围
- 主函数: `spider/User/main.c`
- 硬件驱动目录: `spider/Hardware/`
  - `PWM.c/.h`
  - `Servo.c/.h`
  - `Usart1.c/.h`
  - `OLED.c/.h`
  - `LED.c/.h`
  - `Key.c/.h`

## 2. main.c 功能流程解析

### 2.1 初始化阶段
在 `main()` 中依次执行:
1. `OLED_Init()`
2. `LED_Init()`
3. `Key_Init()`
4. `BW_Init()`
5. `Servo_Init()`
6. OLED 显示固定字符串 `=------usart1-----23=`

这说明系统启动后，先完成显示、状态灯、按键、蓝牙串口和舵机 PWM 输出通道的准备。

### 2.2 开机自检/姿态动作阶段
初始化后，主函数执行一段固定动作序列(每步间隔 `Delay_ms(1000)`):
1. 竖直归位: 8 路舵机全部打到 90°
2. 展开形态: 1~4 路关节分别到 45/135 等角度，5~8 路保持 90°
3. 坐下动作: 5~8 路切换到 0/180 组合
4. 再回到展开形态

该段代码可视为上电姿态检查和机械机构动作确认。

### 2.3 主循环阶段(前进步态 + 通信)
`while(1)` 中存在两部分核心逻辑:
1. 步态控制:
- 两组舵机角度交替输出，每组之间 `Delay_ms(400)`
- 构成周期性前进动作

2. 串口蓝牙协议处理:
- 检查 `BW_GetRxFlag()==1`
- 若收到完整一帧:
  - OLED 显示 `BW_RxPacket[0..9]`
  - 回传 `BW_TxPacket[0..9] = BW_RxPacket[0..9]`
  - `BW_TxPacket[10]` 设为前 10 字节和(校验位)
  - 调用 `BW_SendPacket()` 发回
  - 根据 `BW_RxPacket[0]` 控制 LED:
    - `0x00` -> `LEDtest_OFF()`
    - `0x01` -> `LEDtest_ON()`
- 每轮循环末尾执行 `OLED_Update()`

## 3. Hardware 目录硬件资源配置清单

## 3.1 PWM 与舵机资源(`PWM.c`, `Servo.c`)

### PWM 定时基础参数
- 两组定时器都配置为:
  - 预分频 `PSC = 72 - 1`
  - 自动重装 `ARR = 20000 - 1`
  - 计数频率 1 MHz (1 us)
  - PWM 周期约 20 ms (约 50 Hz，适配舵机)

### 通道与引脚映射
1. `PWM_Init()` -> `TIM3` 4 通道
- 时钟: `RCC_APB1Periph_TIM3`
- GPIO 时钟: `GPIOA`, `GPIOB`
- 引脚:
  - `PA6` -> TIM3_CH1 -> `Servo_SetAngle1()`
  - `PA7` -> TIM3_CH2 -> `Servo_SetAngle2()`
  - `PB0` -> TIM3_CH3 -> `Servo_SetAngle3()`
  - `PB1` -> TIM3_CH4 -> `Servo_SetAngle4()`

2. `PWM_Init1()` -> `TIM1` 4 通道
- 时钟: `RCC_APB2Periph_TIM1`
- GPIO 时钟: `GPIOA`
- 引脚:
  - `PA8`  -> TIM1_CH1 -> `Servo_SetAngle5()`
  - `PA9`  -> TIM1_CH2 -> `Servo_SetAngle6()`
  - `PA10` -> TIM1_CH3 -> `Servo_SetAngle7()`
  - `PA11` -> TIM1_CH4 -> `Servo_SetAngle8()`
- 高级定时器额外使能: `TIM_CtrlPWMOutputs(TIM1, ENABLE)`

### 舵机角度到脉宽关系
`Servo_SetAngleX(float Angle)` 内部统一公式:
- `CCR = Angle / 180 * 2000 + 500`
- 对应脉宽约 `500us ~ 2500us`

## 3.2 蓝牙串口资源(`Usart1.c`)

### 实际外设
- 代码文件名为 `Usart1.c`，但实际初始化的是 `USART2`
- 波特率: `9600`
- 配置: `8N1`, 无流控

### 引脚与中断
- GPIO 时钟: `GPIOA`
- USART 时钟: `RCC_APB1Periph_USART2`
- 引脚设置:
  - `PA2`: 复用推挽(AF_PP)
  - `PA3`: 上拉输入(IPU)
- 使能中断: `USART_IT_RXNE`
- 中断向量: `USART2_IRQn`
- 中断优先级组: `NVIC_PriorityGroup_2`

### 协议格式
接收状态机在 `USART2_IRQHandler()` 中实现:
- 帧头: `0xA5`
- 数据: 11 字节存入 `BW_RxPacket[0..10]`
- 帧尾: `0x5A`
- 收到完整帧后置位 `BW_RxFlag=1`

发送 `BW_SendPacket()` 格式:
- `0xA5 + 11字节数据 + 0x5A`

## 3.3 OLED 显示资源(`OLED.c`)

### 接口方式
- 软件模拟 I2C(bit-bang)
- 引脚:
  - `PB8` -> SCL
  - `PB9` -> SDA
- GPIO 模式: 开漏输出 `GPIO_Mode_Out_OD`
- OLED 从机地址字节: `0x78`

### 显示机制
- 显存: `OLED_DisplayBuf[8][128]`
- 先写显存，后调用 `OLED_Update()` 刷新到屏幕

## 3.4 LED 资源(`LED.c`)
- 引脚: `PC13`
- 模式: 推挽输出
- 逻辑:
  - `LEDtest_ON()` -> 拉低点亮
  - `LEDtest_OFF()` -> 拉高熄灭

## 3.5 按键资源(`Key.c`)
- 引脚: `PC14`
- 模式: 上拉输入
- 按下判定: 读到低电平
- 消抖方式: `Delay_ms(300)` 级别阻塞式消抖

## 4. main.c 对 Hardware 的调用列表

### 4.1 初始化调用
- `OLED_Init()`
- `LED_Init()`
- `Key_Init()`
- `BW_Init()`
- `Servo_Init()`

### 4.2 循环中的周期调用
- 舵机动作:
  - `Servo_SetAngle1()` ... `Servo_SetAngle8()`
- 通信:
  - `BW_GetRxFlag()`
  - `BW_SendPacket()`
  - 读写全局缓冲: `BW_RxPacket[]`, `BW_TxPacket[]`
- 显示:
  - `OLED_ShowString()`
  - `OLED_ShowNum()`
  - `OLED_ShowImage()`
  - `OLED_Update()`
- 指示灯:
  - `LEDtest_ON()`
  - `LEDtest_OFF()`

## 5. 关键观察与注意点
1. `Usart1` 命名与实际 `USART2` 外设不一致，后续维护时容易混淆。
2. `PA2/PA3` 的 TX/RX 注释与标准习惯存在表述混乱，建议在文档或注释中再确认线序。
3. `main.c` 中步态与通信都使用阻塞延时，动作控制实时性和通信响应会互相影响。
4. 按键驱动已初始化但主循环未实际使用 `Key_GetNum()`。
5. 主循环每次都 `OLED_Update()`，屏幕刷新频率受步态延时限制。

## 6. 结论
该项目已形成完整的四足蜘蛛机器人控制闭环:
- `Servo/PWM` 提供 8 路舵机关节控制
- `USART2` + 协议状态机提供蓝牙遥控通道
- `OLED` 提供状态与数据可视化
- `LED` 提供最基本远程命令反馈

主函数目前以“前进步态示例 + 串口回包联调”为核心，适合继续扩展为多步态、多指令的遥控控制框架。
