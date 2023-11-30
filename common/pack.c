#include "pack.h"

u32 u32_pack(u8 a, u8 b, u8 c, u8 d) {
    return (a << 24) | (b << 16) | (c << 8) | d;
}

void u32_unpack(u32 n, u8 *a, u8 *b, u8 *c, u8 *d) {
    *a = (n & 0xFF000000) >> 24;
    *b = (n & 0x00FF0000) >> 16;
    *c = (n & 0x0000FF00) >> 8;
    *d = (n & 0x000000FF);
}
