#include "str.h"

usize str_nlen(char const * const s, usize max) {
    usize l = 0;

    while (s[l] != '\0' && l < max)
        l += 1;

    return l;
}

u8 from_hex_str(u8 * const err, const char s[2]) {
    u8 b = 0;
    *err = from_hex_str_OK;

    for (u8 i = 0; i < 2; i += 1) {
        char c = s[i];

        b <<= 4;
        if (c >= '0' && c <= '9')
            b += c - '0';
        else if (c >= 'a' && c <= 'f')
            b += c - 'a' + 10;
        else if (c >= 'A' && c <= 'F')
            b += c - 'A' + 10;
        else {
            *err = from_hex_str_INVALID_CHAR;
            return 0;
        }
    }

    return b;
}

usize from_str(u8 * const err, usize len, char const * const s) {
    usize n = 0;
    *err = from_str_OK;

    char c;
    usize prev;

    for (usize i = 0; i < len; i += 1) {
        c = s[i];

        if (c < '0' || c > '9') {
            *err = from_str_INVALID_CHAR;
            return 0;
        }

        prev = n;

        n *= 10;
        n += c - '0';

        if ( (n / 10) != prev ) {
            *err = from_str_OVERFLOW;
            return 0;
        }
    }

    return n;
}

void to_hex_str(u8 b, char s[2]) {
    u8 bh = (b & 0xF0) >> 4;
    u8 bl =  b & 0x0F;

    if (bh <= 9) s[0] = bh + '0';
    else         s[0] = bh - 10 + 'a';

    if (bl <= 9) s[1] = bl + '0';
    else         s[1] = bl - 10 + 'a';
}
