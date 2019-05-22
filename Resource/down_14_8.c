#include "lvgl/lvgl.h"

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

const LV_ATTRIBUTE_MEM_ALIGN uint8_t down_14_8_map[] = {
  0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 
  0x0a, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa0, 
  0x00, 0xcf, 0xff, 0xff, 0xff, 0xfc, 0x00, 
  0x00, 0x1d, 0xff, 0xff, 0xff, 0xd1, 0x00, 
  0x00, 0x02, 0xef, 0xff, 0xfe, 0x20, 0x00, 
  0x00, 0x00, 0x3f, 0xff, 0xf3, 0x00, 0x00, 
  0x00, 0x00, 0x04, 0xff, 0x40, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x66, 0x00, 0x00, 0x00, 
};

lv_img_dsc_t down_14_8 = {
  .header.always_zero = 0,
  .header.w = 14,
  .header.h = 8,
  .data_size = 56,
  .header.cf = LV_IMG_CF_ALPHA_4BIT,
  .data = down_14_8_map,
};