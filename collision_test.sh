#!/bin/sh
set -eu

truncate -s0 hashes.txt
LINES="10000"

for i in $(seq ${LINES}); do
    if [ "$(( i % 100 ))" = 0 ]; then
        printf "[%d/%d = %d%%]\n" "${i}" "${LINES}" "$(( 100 * i / LINES ))"
    fi

    RAND="$(dd if=/dev/random count=4 bs=1 2>/dev/null | od -A n -t u)"
    LEN="$(( RAND % 999 + 1 ))"

    tr -cd 'a-zA-Z0-9,.?! ' </dev/random | fold -w "${LEN}" | head -n 1 | \
        xargs -d'\n' -L1 dash -c 'printf "%s" "$1" | ./cchash' '' >>hashes.txt
done

# Check collisions
printf "Collisions: %s\n" "$(sort <hashes.txt | uniq -c | grep -v '^\s*1 ' | sort -n | wc -l)"
