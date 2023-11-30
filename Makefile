CC = gcc -Wall -Wextra -O2

.PHONY: all
all: chacha reduce cchash randgen

chacha: ./chacha.c ./common/types.h ./common/die.h ./common/pack.h ./common/pack.c ./common/str.h ./common/str.c ./libchacha/chacha.h ./libchacha/chacha.c
	$(CC) -o ./chacha ./chacha.c ./common/pack.c ./common/str.c ./libchacha/chacha.c
	strip ./chacha

reduce: ./reduce.c ./common/types.h ./common/die.h ./common/str.h ./common/str.c
	$(CC) -o ./reduce ./reduce.c ./common/str.c
	strip ./reduce

cchash: ./cchash.c ./common/types.h ./common/die.h ./common/pack.h ./common/pack.c ./common/str.h ./common/str.c ./libchacha/chacha.h ./libchacha/chacha.c
	$(CC) -o ./cchash ./cchash.c ./common/pack.c ./common/str.c ./libchacha/chacha.c
	strip ./cchash

randgen: ./randgen.c ./common/types.h ./common/die.h
	$(CC) -o ./randgen ./randgen.c
	strip ./randgen
