#include <stdint.h>

#ifndef __GALLIFREY_V3A_SEQ_CODE__
#define __GALLIFREY_V3A_SEQ_CODE__

static const uint8_t GALLIFREY_V3A_SEQ_0_ADDR = 0x00;
static const uint8_t GALLIFREY_V3A_SEQ_1_ADDR = 0x0d;
static const uint8_t GALLIFREY_V3A_SEQ_2_ADDR = 0x13;
static const uint8_t GALLIFREY_V3A_SEQ_3_ADDR = 0x3b;
static const uint8_t GALLIFREY_V3A_SEQ_4_ADDR = 0x70;
static const uint8_t GALLIFREY_V3A_SEQ_5_ADDR = 0x00;

static const uint8_t GALLIFREY_V3A_SEQ_CODE[] = {
     0x6e, 0x32, 0x01, 0x0b,     0x50, 0x93, 0x52, 0x32,     0x2c, 0x72, 0x7a, 0x40,     0x00, 0x62, 0x32, 0x2c,   // 0x0x 
     0x0b, 0x5c, 0x00, 0x70,     0x0c, 0x01, 0x62, 0x46,     0x7f, 0x32, 0x0a, 0x67,     0x31, 0x0a, 0x22, 0x30,   // 0x1x 
     0x0a, 0x64, 0x62, 0x41,     0x04, 0x52, 0x46, 0x5d,     0x5e, 0x32, 0x01, 0x52,     0x41, 0x58, 0x52, 0x52,   // 0x2x 
     0x13, 0x0b, 0x50, 0x52,     0x32, 0x12, 0xa7, 0x72,     0x7a, 0x42, 0x00, 0x72,     0x7a, 0x43, 0x70, 0x0c,   // 0x3x 
     0x01, 0x0b, 0x5c, 0x62,     0x32, 0x12, 0x6e, 0x09,     0x01, 0x62, 0x41, 0x58,     0x62, 0x52, 0x13, 0x00,   // 0x4x 
     0x5e, 0x61, 0x5f, 0x93,     0x5e, 0x61, 0x80, 0x85,     0x5e, 0x31, 0x10, 0x0d,     0x6e, 0x31, 0x10, 0x6e,   // 0x5x 
     0x61, 0xdf, 0x81, 0x0d,     0x5e, 0x09, 0x01, 0x52,     0x41, 0x04, 0x52, 0x46,     0x6d, 0x80, 0x09, 0x28,   // 0x6x 
     0x83, 0x32, 0x0a, 0x7a,     0x31, 0x0a, 0x7a, 0x30,     0x0a, 0x7b, 0x82, 0x70,     0x0c, 0x02, 0x00
};

static const unsigned int GALLIFREY_V3A_SEQ_CODE_LEN = sizeof(GALLIFREY_V3A_SEQ_CODE);

#endif
