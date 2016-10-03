#!/bin/sh
set -e
. ./iso.sh

#TODO: use the iso instead of the kernel image as userland matures
gdb ./sysroot/boot/zealous.kernel \
        -ex 'target remote | exec qemu-system-$(./target-triplet-to-arch.sh $HOST) -kernel ./sysroot/boot/zealous.kernel -no-shutdown -no-reboot -S -gdb stdio' \
        -ex 'layout src' \
        -ex 'layout reg' \
	-ex 'continue'


