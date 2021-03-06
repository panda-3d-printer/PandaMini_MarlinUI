#include "lvgl/lvgl.h"

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

const LV_ATTRIBUTE_MEM_ALIGN uint8_t right_8_14_map[] = {
  0x80, 0x00, 0x00, 0x00, 
  0xfa, 0x00, 0x00, 0x00, 
  0xff, 0xc1, 0x00, 0x00, 
  0xff, 0xfd, 0x20, 0x00, 
  0xff, 0xff, 0xe3, 0x00, 
  0xff, 0xff, 0xff, 0x50, 
  0xff, 0xff, 0xff, 0xf6, 
  0xff, 0xff, 0xff, 0xf6, 
  0xff, 0xff, 0xff, 0x40, 
  0xff, 0xff, 0xe3, 0x00, 
  0xff, 0xfd, 0x20, 0x00, 
  0xff, 0xc1, 0x00, 0x00, 
  0xfa, 0x00, 0x00, 0x00, 
  0x80, 0x00, 0x00, 0x00, 
};

lv_img_dsc_t right_8_14 = {
  .header.always_zero = 0,
  .header.w = 8,
  .header.h = 14,
  .data_size = 56,
  .header.cf = LV_IMG_CF_ALPHA_4BIT,
  .data = right_8_14_map,
};
