
#include "base/array.h"
#include "base/canvas.h"
#include "base/font_manager.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include <stdlib.h>

TEST(FontManager, basic) {
  font_manager_t font_manager;
  font_dummy_init();
  font_manager_init(&font_manager);
  font_manager_add(&font_manager, font_dummy_0("demo0", 10));
  font_manager_add(&font_manager, font_dummy_1("demo1", 11));
  font_manager_add(&font_manager, font_dummy_2("demo2", 12));

  ASSERT_EQ(font_manager_find(&font_manager, "demo0", 10), font_dummy_0("demo0", 10));
  ASSERT_EQ(font_manager_find(&font_manager, "demo1", 11), font_dummy_1("demo1", 11));
  ASSERT_EQ(font_manager_find(&font_manager, "demo2", 12), font_dummy_2("demo2", 12));

  ASSERT_EQ(font_manager_find(&font_manager, NULL, 10), font_dummy_0("demo0", 10));
  ASSERT_EQ(font_manager_find(&font_manager, NULL, 11), font_dummy_1("demo1", 11));
  ASSERT_EQ(font_manager_find(&font_manager, NULL, 12), font_dummy_2("demo2", 12));

  ASSERT_EQ(font_manager_find(&font_manager, NULL, 20) != NULL, true);
  ASSERT_EQ(font_manager_find(&font_manager, "demo0", 20) != NULL, true);

  font_manager_deinit(&font_manager);
}
