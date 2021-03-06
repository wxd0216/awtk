﻿/**
 * File:   window_animator_fb
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  fb implemented window animator
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
 * 2018-04-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/window_animator.h"

static ret_t window_animator_open_destroy(window_animator_t* wa) {
  bitmap_destroy(&(wa->prev_img));
  bitmap_destroy(&(wa->curr_img));

  memset(wa, 0x00, sizeof(window_animator_t));
  TKMEM_FREE(wa);

  return RET_OK;
}

static ret_t window_animator_close_destroy(window_animator_t* wa) {
  widget_destroy(wa->curr_win);

  return window_animator_open_destroy(wa);
}

static ret_t window_animator_prepare(window_animator_t* wa, canvas_t* c, widget_t* prev_win,
                                     widget_t* curr_win, bool_t open) {
  rect_t r;
  lcd_t* lcd = c->lcd;
  widget_t* wm = prev_win->parent;

  wa->ratio = 1;
  wa->canvas = c;
  wa->open = open;
  wa->duration = 400;
  wa->prev_win = prev_win;
  wa->curr_win = curr_win;
  rect_init(r, 0, 0, wm->w, wm->h);

  ENSURE(canvas_begin_frame(c, &r, LCD_DRAW_OFFLINE) == RET_OK);
  ENSURE(widget_paint(prev_win, c) == RET_OK);
  ENSURE(lcd_take_snapshot(lcd, &(wa->prev_img)) == RET_OK);
  ENSURE(canvas_end_frame(c) == RET_OK);

  ENSURE(canvas_begin_frame(c, &r, LCD_DRAW_OFFLINE) == RET_OK);
  ENSURE(widget_paint(curr_win, c) == RET_OK);
  ENSURE(lcd_take_snapshot(lcd, &(wa->curr_img)) == RET_OK);
  ENSURE(canvas_end_frame(c) == RET_OK);

  wa->prev_img.flags = BITMAP_FLAG_OPAQUE;
  wa->curr_img.flags = BITMAP_FLAG_OPAQUE;

  return RET_OK;
}

#include "fade.inc"
#include "htranslate.inc"
#include "vtranslate.inc"
#include "center_scale.inc"
#include "bottom_to_top.inc"
#include "top_to_bottom.inc"

#include "common.inc"
