#include "common/types.h"
#include "common/die.h"
#include "common/pack.h"
#include "common/str.h"

#include "libchacha/chacha.h"

#define CC_ROUNDS 20

int main(int argc, char *argv[]) {
    if (argc != 1 && argc != 2)
        die("Usage: %s [file]\n", argv[0]);

    FILE *fh = stdin;

    if (argc == 2) {
        fh = fopen(argv[1], "rb");

        if (fh == NULL)
            die("[E] Failed to open file: %s\n", argv[1]);

    } else
        freopen(NULL, "rb", stdin);

    usize i;

    // First pass + reduce
    const usize enc0_len = (8 + 2) * 4;
    u8 enc0_data[enc0_len];
    usize enc0_idx = 0;

    for (i = 0; i < enc0_len; i += 1)
        enc0_data[i] = 0;

    // Encrypt with key = 0, nonce = 0
    {
        u32 key[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        u32 nonce[2] = {0, 0};
        u32 counter[2] = {0, 0};

        u32 data[16];
        u8 data_bytes[16 * 4];
        usize read_count = 1;

        u32 cc_state[16];
        u32 cc_block[16];

        while (read_count > 0) {
            // Read bytes
            for (i = 0; i < 16 * 4; i += 1) data_bytes[i] = 0;
            read_count = fread(data_bytes, 1, 16 * 4, fh);
            if (read_count == 0) break;

            // Pack into u32
            for (i = 0; i < 16; i += 1) {
                data[i] = u32_pack(
                    data_bytes[i*4 + 0],
                    data_bytes[i*4 + 1],
                    data_bytes[i*4 + 2],
                    data_bytes[i*4 + 3]
                );
            }

            // Calculate chacha block
            setup_chacha_block(key, counter, nonce, cc_state);
            chacha_block(cc_state, cc_block, CC_ROUNDS);

            // Encrypt
            for (i = 0; i < 16; i += 1)
                data[i] ^= cc_block[i];

            // Unpack
            for (i = 0; i < 16; i += 1) {
                u32_unpack(
                    data[i],
                    &data_bytes[i*4 + 0],
                    &data_bytes[i*4 + 1],
                    &data_bytes[i*4 + 2],
                    &data_bytes[i*4 + 3]
                );
            }

            // Reduce
            for (i = 0; i < 16 * 4; i += 1) {
                enc0_data[enc0_idx % enc0_len] ^= data_bytes[i];
                enc0_idx += 1;

                if (enc0_idx >= enc0_len)
                    enc0_idx = enc0_idx % enc0_len;
            }

            // Increase counter
            if (counter[1] == 0xFFFFFFFF) {
                if (counter[0] == 0xFFFFFFFF) die("[E]: Counter range end\n");

                counter[0] += 1;
                counter[1] = 0;
            } else counter[1] += 1;
        }
    }

    u8 hash[16 * 4];

    // Now use the resulting data as a new key and nonce
    {
        u32 key[8];
        u32 nonce[2];
        u32 counter[2] = {0, 0};

        for (i = 0; i < 8; i += 1)
            key[i] = u32_pack(
                enc0_data[i*4 + 0],
                enc0_data[i*4 + 1],
                enc0_data[i*4 + 2],
                enc0_data[i*4 + 3]
            );

        for (i = 8; i < 8+2; i += 1)
            nonce[i - 8] = u32_pack(
                enc0_data[i*4 + 0],
                enc0_data[i*4 + 1],
                enc0_data[i*4 + 2],
                enc0_data[i*4 + 3]
            );

        u8 data_bytes[16 * 4];
        for (i = 0; i < 16 * 4; i += 1)
            data_bytes[i] = 0;
        for (i = 0; i < enc0_len; i += 1)
            data_bytes[i] = enc0_data[i];

        // Pack
        u32 data[16];
        for (i = 0; i < 16; i += 1)
            data[i] = u32_pack(
                data_bytes[i*4 + 0],
                data_bytes[i*4 + 1],
                data_bytes[i*4 + 2],
                data_bytes[i*4 + 3]
            );

        u32 cc_state[16];
        u32 cc_block[16];

        // Calculate chacha block
        setup_chacha_block(key, counter, nonce, cc_state);
        chacha_block(cc_state, cc_block, CC_ROUNDS);

        // Encrypt
        for (i = 0; i < 16; i += 1)
            data[i] ^= cc_block[i];

        // Unpack
        for (i = 0; i < 16; i += 1)
            u32_unpack(
                data[i],
                &hash[i*4 + 0],
                &hash[i*4 + 1],
                &hash[i*4 + 2],
                &hash[i*4 + 3]
            );
    }

    // Print as hex
    {
        char hex[2];

        for (i = 0; i < 16 * 4; i += 1) {
            to_hex_str(hash[i], hex);

            putchar(hex[0]);
            putchar(hex[1]);
        }

        putchar('\n');
    }

    return 0;
}
