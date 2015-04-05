CPU cycle counter for ARM11
===========================

Usage
-----

    make
    insmod cpucycles4ns.ko
    dmesg | grep cpucycles4ns

This should print something like `mknod /dev/cpucycles4ns c 253 0`. Run that command.

Credits
-------
Based on http://bench.cr.yp.to/cpucycles/n810.html which appears to have been
written by D.J. Bernstein (according to the paper "SHA3 on ARM11 processors" by
Peters Schwabe, Bo-Yin Yang and Shang-Yi Yang).
