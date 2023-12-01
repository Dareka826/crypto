#!/bin/sh
set -eu

ALGO="./cchash"
[ "$#" -ge 1 ] && ALGO="${1}" || :
command -v "${ALGO}" >/dev/null 2>&1 || {
    printf "[E]: No such command: %s\n" "${ALGO}"
    exit 1
}

FNAME="hashes_$(printf "%s" "${ALGO}" | tr -c 'a-zA-Z0-9' '_' | sed 's/_\+/_/g')"

truncate -s0 "${FNAME}"
LINES="2000000"

WAIT_FILE="$(mktemp "${TMPDIR:-/tmp}/wait.XXXX")"
touch "${WAIT_FILE}"

{
    ./randgen | \
        head -n "${LINES}" | \
        sort | uniq | \
        xargs -d'\n' -L1 dash -c 'printf "%s" "$1" | '"'${ALGO}'"' | awk '\''{ print $1 }'\' '' >"${FNAME}"

    rm "${WAIT_FILE}"
} &

while [ -e "${WAIT_FILE}" ]; do
    I="$(wc -l <"${FNAME}")"
    printf "[%d/%d = %d%%]\n" "${I}" "${LINES}" "$(( I * 100 / LINES ))"
    sleep 1
done

# Check collisions
printf "Collisions (%s): %s\n" "${ALGO}" "$(sort <"${FNAME}" | uniq -c | grep -v '^\s*1 ' | sort -n | wc -l)"
