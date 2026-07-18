# 按键控制led灯实现不同的点亮效果 #

1.封装led引脚

2.设置流水灯流动速度

3.定义变量（标志位）设置流水灯流动模式（流动模式在中断函数中修改）

4.for循环批量控制全部 LED（批量点亮、熄灭）

![image-20260718204608131](C:\Users\34756\AppData\Roaming\Typora\typora-user-images\image-20260718204608131.png)

1.主循环计时到点后，根据模式变量调用对应函数，实现不同流水效果。

2.辅助函数作用：

static void all_led_off(void);                  // 全部LED熄灭 

static void led_on(uint8_t index);              // 单独点亮指定序号LED 

static void set_leds_by_mask(uint8_t mask);     // 批量控制多个LED亮灭

![](C:\Users\34756\Pictures\Screenshots\屏幕截图 2026-07-18 205210.png)

1.在循环中根据不同事件执行不同模式的流水灯

![image-20260718205817723](C:\Users\34756\AppData\Roaming\Typora\typora-user-images\image-20260718205817723.png)

1.在中断函数中检测更改标志位函数的模式，并将信息传递到while函数中

![image-20260718205937677](C:\Users\34756\AppData\Roaming\Typora\typora-user-images\image-20260718205937677.png)