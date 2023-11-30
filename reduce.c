#include "common/types.h"
#include "common/str.h"
#include "common/die.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 2)
        die("Usage: %s max_size\n", argv[0]);

    usize i;

    usize max_size = 0;
    {
        usize l = str_nlen(argv[1], 256);

        u8 e;
        max_size = from_str(&e, l, argv[1]);

        if (e == from_str_INVALID_CHAR)
            die("[E] Invalid char in max_size\n");

        if (e == from_str_OVERFLOW)
            die("[E] max_size too big, usize overflow\n");
    }

    freopen(NULL, "rb", stdin);
    freopen(NULL, "wb", stdout);

    u8 data[max_size];
    for (i = 0; i < max_size; i += 1)
        data[i] = 0;

    u8 bytes[max_size];
    usize read_count;
    while (1) {
        read_count = fread(bytes, 1, max_size, stdin);

        if (read_count == 0) break;

        for (i = 0; i < read_count; i++)
            data[i] ^= bytes[i];
    }

    fwrite(data, 1, max_size, stdout);

    return 0;
}
