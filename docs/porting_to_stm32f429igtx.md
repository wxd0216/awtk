## 将AWTK移植到STM32f429igtx裸系统

AWTK的可移植性很高，在移植时只需要实现平台初始化、lcd和mainloop三个方面的东西。本文以STM32f429igtx为例介绍移植AWTK到新平台的过程。

### 一、平台初始化

除了基本的libc函数外，AWTK对平台没有特别要求，在stm32f429igtx上没有函数gettimeofday，所以要实现一个获取当前时间的函数get\_time\_ms。另外需要给GUI分配一块内存空间，并调用mem\_init。

```
uint32_t get_time_ms() {
  return HAL_GetTick();
}

void sleep_ms(uint32_t ms) {
  delay_ms(ms);
}

#define MEM2_MAX_SIZE   28912 * 1024 
#define MEM2_ADDR (uint8_t*)0XC01F4000

ret_t platform_prepare(void) {
  timer_init(get_time_ms);
  mem_init(MEM2_ADDR, MEM2_MAX_SIZE);
  
  return RET_OK;
}
```
 
> 参考：awtk-port/platform.c

### 二、实现lcd

lcd\_t接口提供基本的显示功能，AWTK提供基于寄存器和基于framebuffer两种缺省实现，在此基础上实现自己的lcd\_t接口非常方便。

stm32f429igtx使用基于framebuffer的lcd的缺省实现，只需要调用函数lcd\_mem\_create\_double\_fb即可。


```
extern u32 *ltdc_framebuf[2];
#define online_fb_addr (uint8_t*)ltdc_framebuf[0]
#define offline_fb_addr (uint8_t*)ltdc_framebuf[1]

lcd_t* platform_create_lcd(wh_t w, wh_t h) {
  return lcd_mem_create_double_fb(lcdltdc.pwidth, lcdltdc.pheight, online_fb_addr, offline_fb_addr);
}
```

> 参考 main\_loop/main\_loop\_stm32\_raw.c

### 三、实现main\_loop

main\_loop主要负责事件分发和绘制这个不断循环的过程。main\_loop\_raw.inc里实现了裸系统main\_loop的基本功能，在移植时加上输入事件的的分发即可：

```
static ret_t post_touch_events(main_loop_t* loop, bool_t pressed, xy_t x, xy_t y); 

uint8_t platform_disaptch_input(main_loop_t* loop) {
  int x = 0;
  int y = 0;
  uint8_t key = KEY_Scan(0);
  
  tp_dev.scan(0);    
  
  x = tp_dev.x[0];
  y = tp_dev.y[0]; 

  y = 480 - tp_dev.x[0];
  x = tp_dev.y[0]; 
  
  if(tp_dev.sta&TP_PRES_DOWN){    
    post_touch_events(loop, TRUE, x, y); 
  } else {
    post_touch_events(loop, FALSE, x, y); 
  }
  
  return 0;
}
```

> 参考 main\_loop/main\_loop\_stm32\_raw.c


注：[正点原子的开发实验板](https://item.taobao.com/item.htm?spm=a1z10.1-c-s.w11877762-18401048725.10.145a2276IsywTF&id=534585837612)为载体移植，其它开发板应该差不多。

完整项目和源码请参考：[awtk-stm32f429igtx-raw](https://github.com/xianjimli/awtk-stm32f429igtx-raw)

