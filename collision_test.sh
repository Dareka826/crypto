#!/bin/sh

truncate -s0 hashes.txt

tr -cd 'a-zA-Z0-9,.?! ' </dev/random | fold -w 1000 | head -n 1000 | \
    xargs -d'\n' -L1 dash -c 'printf "%s" "$1" | ./cchash' '' >>hashes.txt
