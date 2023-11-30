#!/bin/sh
# \
exec jimsh "$0" "$@"

proc test {num chars} {
    set positions {}
    for {set i 0} {$i < $num} {incr i} \
        {lappend positions 0}

    set lchars   [split $chars {}]
    set lenchars [llength $lchars]

    while {[lindex $positions end] < $lenchars} {
        foreach p [lreverse $positions] {
            puts -nonewline [lindex $lchars $p]
        }
        puts {}

        # Increment
        set positions [lreplace $positions 0 0 [expr {[lindex $positions 0] + 1}]]
        for {set i 0} {$i < ($num-1)} {incr i} {
            if {[lindex $positions $i] >= $lenchars} {
                set c [expr {([lindex $positions $i] % $lenchars) + 1}]
                set j $i
                incr j
                set positions [lreplace $positions $j $j [expr {[lindex $positions $j] + 1}]]
                set positions [lreplace $positions $i $i $c]
            }
        }
    }
}

test 10000 {abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.?! }
