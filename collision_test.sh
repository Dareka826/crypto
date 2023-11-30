#!/bin/sh

truncate -s0 hashes.txt

tclsh ./seq.tcl | \
    xargs -d'\n' -L1 dash -c 'printf "%s" "$1" | ./cchash' '' | \
    tee hashes.txt
