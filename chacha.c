#include "libchacha/chacha.h"

#include "common/types.h"
#include "common/die.h"
#include "common/pack.h"
#include "common/str.h"

#include <stdio.h>


int main(int argc, char *argv[]) {
    if (argc != 4)
        die("Usage: %s rounds key_hex|keyfile nonce_hex|noncefile\n", argv[0]);

    usize i;

    u32 rounds = 0;
    {
        usize l = str_nlen(argv[1], 256);

        u8 e;
        rounds = from_str(&e, l, argv[1]);

        if (e == from_str_INVALID_CHAR)
            die("[E] Invalid char in rounds\n");

        if (e == from_str_OVERFLOW)
            die("[E] Too many rounds, usize overflow\n");
    }

    u32 key[8];
    {
        u8 key_bytes[8 * 4];

        // Try to open as file
        FILE *keyfile = fopen(argv[2], "rb");

        if (keyfile != NULL) {
            usize read_count = fread(key_bytes, 1, 8 * 4, keyfile);
            fclose(keyfile);
            keyfile = NULL;

            if (read_count != 8 * 4)
                die("Not enough key bytes!\n");

        } else {
            usize l = str_nlen(argv[2], 8 * 8 + 1);

            if (l != 8 * 8)
                die("[E]: Wrong key length: %lu != 64\n", l);

            char hex[2];
            u8 e;

            for (i = 0; i < 8 * 4; i += 1) {
                hex[0] = argv[2][i*2 + 0];
                hex[1] = argv[2][i*2 + 1];

                key_bytes[i] = from_hex_str(&e, hex);

                if (e == from_hex_str_INVALID_CHAR)
                    die("[E] Invalid hex char in key\n");
            }
        }

        for (i = 0; i < 8; i += 1)
            key[i] = u32_pack(
                key_bytes[i*4 + 0],
                key_bytes[i*4 + 1],
                key_bytes[i*4 + 2],
                key_bytes[i*4 + 3]
            );
    }

    u32 nonce[2];
    {
        u8 nonce_bytes[2 * 4];

        FILE *noncefile = fopen(argv[3], "rb");

        if (noncefile != NULL) {
            usize read_count = fread(nonce_bytes, 1, 2 * 4, noncefile);
            fclose(noncefile);
            noncefile = NULL;

            if (read_count != 2 * 4)
                die("Not enough nonce bytes!\n");
        } else {
            usize l = str_nlen(argv[3], 2 * 8 + 1);

            if (l != 2 * 8)
                die("[E]: Wrong nonce length: %lu != 16\n", l);

            char hex[2];
            u8 e;

            for (i = 0; i < 2 * 4; i += 1) {
                hex[0] = argv[2][i*2 + 0];
                hex[1] = argv[2][i*2 + 1];

                nonce_bytes[i] = from_hex_str(&e, hex);

                if (e == from_hex_str_INVALID_CHAR)
                    die("[E] Invalid hex char in nonce\n");
            }
        }

        for (i = 0; i < 2; i += 1)
            nonce[i] = u32_pack(
                nonce_bytes[i*4 + 0],
                nonce_bytes[i*4 + 1],
                nonce_bytes[i*4 + 2],
                nonce_bytes[i*4 + 3]
            );
    }

    u32 counter[2] = {0, 0};

    freopen(NULL, "rb", stdin);
    freopen(NULL, "wb", stdout);

    // Encrpyt the stdin byte stream
    {
        u32 data[16];
        u8 data_bytes[16 * 4];
        usize read_count = 1;

        u32 cc_state[16];
        u32 cc_block[16];

        while (read_count > 0) {
            // Read bytes
            for (i = 0; i < 16 * 4; i += 1) data_bytes[i] = 0;
            read_count = fread(data_bytes, 1, 16 * 4, stdin);
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
            chacha_block(cc_state, cc_block, rounds);

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

            // Write
            fwrite(data_bytes, 1, 16 * 4, stdout);

            // Increase counter
            if (counter[1] == 0xFFFFFFFF) {
                if (counter[0] == 0xFFFFFFFF) die("[E]: Counter range end\n");

                counter[0] += 1;
                counter[1] = 0;
            } else counter[1] += 1;
        }
    }

    return 0;
}
