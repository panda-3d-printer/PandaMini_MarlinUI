#include "lvgl/lvgl.h"

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

const LV_ATTRIBUTE_MEM_ALIGN uint8_t shutdown_16_map[] = {
  0x00, 0x00, 0x00, 0x0c, 0xd0, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x0f, 0xf2, 0x04, 0x20, 0x00, 
  0x00, 0x6f, 0xa0, 0x0f, 0xf2, 0x3f, 0xf5, 0x00, 
  0x05, 0xff, 0x90, 0x0f, 0xf2, 0x0a, 0xff, 0x50, 
  0x1f, 0xf9, 0x00, 0x0f, 0xf2, 0x00, 0x9f, 0xf1, 
  0x8f, 0xe0, 0x00, 0x0f, 0xf2, 0x00, 0x0d, 0xf8, 
  0xdf, 0x80, 0x00, 0x0f, 0xf2, 0x00, 0x06, 0xfd, 
  0xff, 0x40, 0x00, 0x0f, 0xf2, 0x00, 0x02, 0xff, 
  0xff, 0x30, 0x00, 0x0f, 0xf2, 0x00, 0x02, 0xff, 
  0xef, 0x60, 0x00, 0x0d, 0xf0, 0x00, 0x04, 0xfe, 
  0xaf, 0xb0, 0x00, 0x00, 0x00, 0x00, 0x0a, 0xfb, 
  0x4f, 0xf5, 0x00, 0x00, 0x00, 0x00, 0x4f, 0xf5, 
  0x0b, 0xff, 0x50, 0x00, 0x00, 0x04, 0xff, 0xc0, 
  0x01, 0xdf, 0xfb, 0x52, 0x25, 0xbf, 0xfe, 0x20, 
  0x00, 0x1a, 0xff, 0xff, 0xff, 0xff, 0xc1, 0x00, 
  0x00, 0x00, 0x39, 0xdf, 0xfe, 0xa4, 0x00, 0x00, 
};

lv_img_dsc_t shutdown_16 = {
  .header.always_zero = 0,
  .header.w = 16,
  .header.h = 16,
  .data_size = 128,
  .header.cf = LV_IMG_CF_ALPHA_4BIT,
  .data = shutdown_16_map,
};