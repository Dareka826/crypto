#include "chacha.h"
#include "../common/pack.h"

#define ROTL rotate_left
void rotate_left(u32 *num, u32 places) {
    places %= 32;
    *num = (*num << places) | (*num >> (32 - places));
}

#define QR quarter_round
void quarter_round(u32 *a, u32 *b, u32 *c, u32 *d) {
    *a += *b; *d ^= *a; ROTL(d, 16);
    *c += *d; *b ^= *c; ROTL(b, 12);
    *a += *b; *d ^= *a; ROTL(d,  8);
    *c += *d; *b ^= *c; ROTL(b,  7);
}

void chacha_block(const u32 in[16], u32 out[16], u32 rounds) {
    u32 x[16];
    u32 i;

    for (i = 0; i < 16; i += 1)
        x[i] = in[i];

    u8 do_extra_half = (rounds %2);
    rounds -= do_extra_half;

    for (i = 0; i < rounds; i += 2) {
        // Even round
        QR(&x[ 0], &x[ 4], &x[ 8], &x[12]);
        QR(&x[ 1], &x[ 5], &x[ 9], &x[13]);
        QR(&x[ 2], &x[ 6], &x[10], &x[14]);
        QR(&x[ 3], &x[ 7], &x[11], &x[15]);

        // Odd round
        QR(&x[ 0], &x[ 5], &x[10], &x[15]);
        QR(&x[ 1], &x[ 6], &x[11], &x[12]);
        QR(&x[ 2], &x[ 7], &x[ 8], &x[13]);
        QR(&x[ 3], &x[ 4], &x[ 9], &x[14]);
    }

    if (do_extra_half) {
        QR(&x[ 0], &x[ 4], &x[ 8], &x[12]);
        QR(&x[ 1], &x[ 5], &x[ 9], &x[13]);
        QR(&x[ 2], &x[ 6], &x[10], &x[14]);
        QR(&x[ 3], &x[ 7], &x[11], &x[15]);
    }

    for (i = 0; i < 16; i += 1)
        out[i] = x[i] + in[i];
}

void setup_chacha_block(const u32 key[8], const u32 counter[2], const u32 nonce[2], u32 out[16]) {
    const char c[] = "expand 32-byte k";
    u32 i;

    for (i = 0; i < 4; i += 1)
        out[i] = u32_pack(c[i*4 + 0], c[i*4 + 1], c[i*4 + 2], c[i*4 + 3]);

    for (i = 0; i < 8; i += 1)
        out[4 + i] = key[i];

    for (i = 0; i < 2; i += 1)
        out[12 + i] = counter[i];

    for (i = 0; i < 2; i += 1)
        out[14 + i] = nonce[i];
}
