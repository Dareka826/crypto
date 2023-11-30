#ifndef __STR_H_39GqRzEpnWpxqcqh
#define __STR_H_39GqRzEpnWpxqcqh

#include "types.h"

usize str_nlen(char const * const s, usize max);

enum {
    from_hex_str_OK = 0,
    from_hex_str_INVALID_CHAR = 1,
};

u8 from_hex_str(u8 * const err, const char s[2]);

enum {
    from_str_OK = 0,
    from_str_INVALID_CHAR = 1,
    from_str_OVERFLOW = 2,
};

usize from_str(u8 * const err, usize len, char const * const s);

#endif
