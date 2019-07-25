#include "SpiFlashResources.h"

#ifdef USE_SPI_FLASH_FONT

#include <SerialFlash.h>


static uint32_t glyph_dsc_size = sizeof(lv_font_glyph_dsc_t);

static lv_font_glyph_dsc_t tempGlyphDsc;
static const lv_font_t * tempDscFont = NULL;
static uint32_t TempDscLetter;

static uint8_t tempBitMap[40*40];
static const lv_font_t * tempMapFont = NULL;
static uint32_t tempMapLetter;

static void getTempGlyphDsc(const lv_font_t * font, uint32_t unicode_letter , uint32_t idx)
{
	static uint8_t * temp_addr = (uint8_t*)&tempGlyphDsc;

	if( tempDscFont != font || TempDscLetter != unicode_letter )
	{
		SerialFlash.read((uintptr_t)(font->glyph_dsc + idx) , temp_addr , glyph_dsc_size);
		tempDscFont = font;
		TempDscLetter = unicode_letter;
	}
}

static bool isTempGlyphDsc(const lv_font_t * font, uint32_t unicode_letter)
{
	return tempDscFont == font && TempDscLetter == unicode_letter;
}

static void getTempBitMap(const lv_font_t * font, uint32_t unicode_letter, lv_font_glyph_dsc_t glyph_dsc)
{
	if( tempMapFont != font || tempMapLetter != unicode_letter )
	{
		SerialFlash.read((uintptr_t)(font->glyph_bitmap + glyph_dsc.glyph_index) , tempBitMap , font->h_px * glyph_dsc.w_px);
		tempMapFont = font;
		tempMapLetter = unicode_letter;
	}
}

static bool isTempBitMap(const lv_font_t * font, uint32_t unicode_letter)
{
	return tempMapFont == font && tempMapLetter == unicode_letter;
}


/**
 * Generic bitmap get function used in 'font->get_bitmap' when the font NOT contains all characters in the range (sparse)
 * @param font pointer to font
 * @param unicode_letter an unicode letter which bitmap should be get
 * @return pointer to the bitmap or NULL if not found
 */
const uint8_t * lv_font_get_spiflash_bitmap_sparse(const lv_font_t * font, uint32_t unicode_letter)
{
    /*Check the range*/
    if(unicode_letter < font->unicode_first || unicode_letter > font->unicode_last) return NULL;

    if(isTempBitMap( font, unicode_letter))
    {
    	return tempBitMap;
    }
    else if(isTempGlyphDsc(font,unicode_letter))
    {
    	getTempBitMap(font, unicode_letter,tempGlyphDsc);
    	return tempBitMap;
    }

    uint32_t i;
    for(i = 0; font->unicode_list[i] != 0; i++) {
        if(font->unicode_list[i] == unicode_letter) {
        	getTempGlyphDsc(font, unicode_letter , i);
        	getTempBitMap(font, unicode_letter , tempGlyphDsc);
            return tempBitMap;
        }
    }

    return NULL;
}

/**
 * Generic glyph width get function used in 'font->get_bitmap' when the font NOT contains all characters in the range (sparse)
 * @param font pointer to font
 * @param unicode_letter an unicode letter which width should be get
 * @return width of the glyph or -1 if not found
 */
int16_t lv_font_get_spiflash_width_sparse(const lv_font_t * font, uint32_t unicode_letter)
{
    /*Check the range*/
    if(unicode_letter < font->unicode_first || unicode_letter > font->unicode_last) return -1;

    /* is cache ok */
    if(isTempGlyphDsc(font,unicode_letter))
    	return tempGlyphDsc.w_px;

    uint32_t i;
    for(i = 0; font->unicode_list[i] != 0; i++) {
        if(font->unicode_list[i] == unicode_letter) {
        	getTempGlyphDsc(font, unicode_letter , i);
            return tempGlyphDsc.w_px;
        }
    }

    return -1;
}

#endif
