/**
 * File:   demo_main.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  demo main
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-02-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tk.h"
#include "base/mem.h"
#include "resource.h"
#include "base/system_info.h"

ret_t application_init(void);

#ifdef USE_GUI_MAIN
int gui_app_start(int lcd_w, int lcd_h) {
	tk_init(lcd_w, lcd_h);
#elif defined(WIN32)
#include <windows.h>
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) {
	tk_init(320, 480);
#else
int main(void) {
  tk_init(320, 480);
#endif

  resource_init();
  application_init();

  tk_run();

  return 0;
}
