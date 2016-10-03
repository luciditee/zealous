#!/bin/sh
set -e
. ./iso.sh

#TODO: use the iso instead of the kernel image as userland matures
qemu-system-$(./target-triplet-to-arch.sh $HOST) -kernel sysroot/boot/zealous.kernel
