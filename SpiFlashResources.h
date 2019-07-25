/*
 * SpiFlashResources.h
 *
 *  Created on: 2019年7月1日
 *      Author: MSI-PC
 */

#ifndef MARLINUI_SPIFLASHRESOURCES_H_
#define MARLINUI_SPIFLASHRESOURCES_H_


#include "lvgl.h"


#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_SPI_FLASH_FONT



////////////////// SPI FLASH RES MAP///////////////////////////////////
// spiFlash中资源表
// 数据结构
// uint32_t SIZE; //资源大小在资源入口的前4个字节中
// uint32_t ADDR; //资源在SPIFLASH中的入口地址


// partition map

// APP [0 ~ 2*1024*1024)

// IMAGE [2*1024*1024 ~ 5*1024*1024)

// FONT  [5*1024*1024 ~ 8*1024*1024)
//   [microsoft_yahei_chinese_16_bitmap.bin:5*1024*1024] // (725,184 字节)
//   [microsoft_yahei_chinese_16_dsc.bin:5*1024*1024 + 900*1024] //(14,000 字节)

//struct lv_font_glyph_dsc_t;



//////////////////////// SPI FLASH Font  ///////////////////////////////////////

/**
 * Generic bitmap get function used in 'font->get_bitmap' when the font NOT contains all characters in the range (sparse)
 * @param font pointer to font
 * @param unicode_letter an unicode letter which bitmap should be get
 * @return pointer to the bitmap or NULL if not found
 */
const uint8_t * lv_font_get_spiflash_bitmap_sparse(const lv_font_t * font, uint32_t unicode_letter);

/**
 * Generic glyph width get function used in 'font->get_bitmap' when the font NOT contains all characters in the range (sparse)
 * @param font pointer to font
 * @param unicode_letter an unicode letter which width should be get
 * @return width of the glyph or -1 if not found
 */
int16_t lv_font_get_spiflash_width_sparse(const lv_font_t * font, uint32_t unicode_letter);

#endif

#ifdef __cplusplus
}
#endif

#endif /* MARLINUI_SPIFLASHRESOURCES_H_ */
