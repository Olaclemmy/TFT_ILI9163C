
/*******************************************************************************
* name: spkOn
*
* preset name: sumotoy_icon
* data block size: 8 bit(s), uint8_t
* RLE compression enabled: no
* conversion type: Monochrome, Edge 144
* bits per pixel: 1
*
* preprocess:
*  main scan direction: top_to_bottom
*  line scan direction: forward
*  inverse: no
* ------------------------------------------------------------------------------
* Created by a custom template of LCD-Image-Converter for .s.u.m.o.t.o.y.
* Icon template version: 1.3
* Note: Used for B&W images as icons
* ------------------------------------------------------------------------------
*******************************************************************************/
#if !defined(SUMO_ICN_LIB_spkOn)
#define SUMO_ICN_LIB_spkOn

#include <stdlib.h>





static const uint8_t image_data_spkOn[94]
#if defined(_FORCE_PROGMEM__)
 PROGMEM 
#endif
= {0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xfc, 0x0f, 0xcf, 0xc1, 0xe0, 0x3f, 0x1e, 0x06, 0x00, 0xfe, 0x38, 0x18, 0x03, 0x3c, 0xe0, 0x60, 0x0e, 0x71, 0x81, 0x80, 0x3c, 0xe6, 0x06, 0x00, 0xf1, 0x98, 0x18, 0x06, 0xe6, 0x60, 0x60, 0x19, 0x99, 0x81, 0x80, 0x66, 0x66, 0x06, 0x01, 0xb9, 0x98, 0x18, 0x03, 0xc6, 0x60, 0x60, 0x0f, 0x39, 0x81, 0x80, 0x39, 0xe6, 0x06, 0x00, 0xcf, 0x38, 0x18, 0x03, 0xf8, 0xf0, 0x70, 0x0f, 0xc7, 0xff, 0xf0, 0x3f, 0x3f, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc};

static const tIcon spkOn = { image_data_spkOn, 30, 25, 94, RLE_no }; 



#endif



