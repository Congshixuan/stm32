# 按键控制LED灯

## 四个按键分别控制led灯的亮灭

![](C:\Users\34756\Pictures\Screenshots\屏幕截图 2026-07-18 165758.png)

**1.设置消抖时间**:记录**每个按键上一次有效触发的时间**，用来实现**非阻塞消抖**

消抖逻辑原理：

本次中断触发时间 - 上次触发时间 < 50ms → 判定为抖动。（不点亮led灯）

*volatile uint8_t flag_ledx*  按键标志位变量



![](C:\Users\34756\Pictures\Screenshots\屏幕截图 2026-07-18 201828.png)

**2. **中断检测到有效按键后，赋值   flag_ledx  =1

![](C:\Users\34756\Pictures\Screenshots\屏幕截图 2026-07-18 165821.png)



![](C:\Users\34756\Pictures\Screenshots\屏幕截图 2026-07-18 165835.png)

**3.**主循环while(1)检测 flag，执行点亮 LED 操作，执行完再把 flag 清 0；

