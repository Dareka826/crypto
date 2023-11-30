#!/bin/sh
set -eu

truncate -s0 hashes.txt
LINES="100000"

WAIT_FILE="$(mktemp "${TMPDIR:-/tmp}/wait.XXXX")"
touch "${WAIT_FILE}"

{
    ./randgen | \
        head -n "${LINES}" | \
        xargs -d'\n' -L1 dash -c 'printf "%s" "$1" | ./cchash' '' >hashes.txt

    rm "${WAIT_FILE}"
} &

while [ -e "${WAIT_FILE}" ]; do
    I="$(wc -l <hashes.txt)"
    printf "[%d/%d = %d%%]\n" "${I}" "${LINES}" "$(( I * 100 / LINES ))"
    sleep 1
done

# Check collisions
printf "Collisions: %s\n" "$(sort <hashes.txt | uniq -c | grep -v '^\s*1 ' | sort -n | wc -l)"
