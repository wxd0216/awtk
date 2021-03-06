## 将AWTK移植到STM32f103ze裸系统

AWTK的可移植性很高，在移植时只需要实现平台初始化、lcd和mainloop三个方面的东西。本文以STM32f103ze为例介绍移植AWTK到新平台的过程。

### 一、平台初始化

除了基本的libc函数外，AWTK对平台没有特别要求，在stm32f103ze上没有函数gettimeofday，所以要实现一个获取当前时间的函数get\_time\_ms。另外需要给GUI分配一块内存空间，并调用mem\_init。

```
uint32_t get_time_ms() {
  return RTC_GetCounter();
}

void sleep_ms(uint32_t ms) { delay_ms(ms); }

static uint32_t s_heam_mem[2048];

ret_t platform_prepare(void) {
  timer_init(get_time_ms);
  mem_init(s_heam_mem, sizeof(s_heam_mem));

  return RET_OK;
}
```

> 参考：awtk-port/platform.c

### 二、实现lcd

lcd\_t接口提供基本的显示功能，AWTK提供基于寄存器和基于framebuffer两种缺省实现，在此基础上实现自己的lcd\_t接口非常方便。

stm32f103ze使用基于寄存器的lcd的缺省实现，只需要提供set\_window\_func和write\_data\_func两个函数/宏即可。这里直接使用了TFT\_SetWindow和TFT\_WriteData两个函数。

```
#include "gui.h"
#include "lcd_driver.h"
#include "lcd/lcd_reg.h"

#define set_window_func TFT_SetWindow
#define write_data_func TFT_WriteData

#include "base/color.h"
#include "base/mem.h"
#include "lcd/rgb565.h"
#include "lcd/lcd_reg.inc"
```

> 参考 awtk-port/lcd\_stm32\_raw.c

### 三、实现main\_loop

main\_loop主要负责事件分发和绘制这个不断循环的过程。main\_loop\_raw.inc里实现了裸系统main\_loop的基本功能，在移植时加上输入事件的的分发即可：

```
static ret_t post_touch_events(main_loop_t* l, bool_t pressed, xy_t x, xy_t y); 

ret_t platform_disaptch_input(main_loop_t* l) { 
  /*TODO：按键处理*/
  return RET_OK; 
}

static lcd_t* platform_create_lcd(wh_t w, wh_t h) { return lcd_reg_create(w, h); }

void TIM3_IRQHandler(void) {
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

    if (TOUCH_Scan() == 0) {
      post_touch_events(main_loop(), TRUE, TouchData.lcdx, TouchData.lcdy);
    } else {
      post_touch_events(main_loop(), FALSE, TouchData.lcdx, TouchData.lcdy);
    }   
  }
}

#include "main_loop/main_loop_raw.inc"
```

> 参考 main\_loop/main\_loop\_stm32\_raw.c


注：目前以[普中科技STM32F103ZET6开发实验板](https://item.taobao.com/item.htm?spm=a230r.1.14.1.50a130e8TMKYMC&id=558855281660&ns=1&abbucket=5#detail) 为载体移植，其它开发板应该差不多。

完整项目和源码请参考：[awtk-stm32f103ze-raw](https://github.com/xianjimli/awtk-stm32f103ze-raw)

