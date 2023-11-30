#include "common/types.h"
#include "common/die.h"

#include <stdlib.h>

// const u32 lines = 1000 * 1000;
const u32 min_len = 1;
const u32 max_len = 1000;

const char symbols[] = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ',', '.', '?',
    '!', '\0'
};

f32 rand_unit() {
    return (f32)(rand()) / (f32)(RAND_MAX);
}

u32 rand_range(u32 min, u32 max) {
    return (u32)(rand_unit() * (f32)(max - min)) + min;
}

int main() {
    srand(0);
    char line_buf[max_len + 1];
    u32 line_len;

    u32 symbols_len = 0;
    while (symbols[symbols_len] != '\0')
        symbols_len += 1;

    // for (u32 y = 0; y < lines; y += 1) {
    while (1) {
        line_len = rand_range(min_len, max_len);

        for (u32 x = 0; x < line_len; x += 1)
            line_buf[x] = symbols[rand_range(0, symbols_len)];
        line_buf[line_len] = '\0';

        printf("%s\n", line_buf);
    }

    return 0;
}
