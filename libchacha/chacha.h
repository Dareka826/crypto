#ifndef __CHACHA_H_eGeAnZJbkHBc7CFh
#define __CHACHA_H_eGeAnZJbkHBc7CFh

#include "../common/types.h"

void rotate_left(u32 *num, u32 places);
void quarter_round(u32 *a, u32 *b, u32 *c, u32 *d);

void chacha_block(const u32 in[16], u32 out[16], u32 rounds);
void setup_chacha_block(const u32 key[8], const u32 counter[2], const u32 nonce[2], u32 out[16]);

#endif
