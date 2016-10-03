#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/zealous.kernel isodir/boot/zealous.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "zealous" {
	multiboot /boot/zealous.kernel
}
EOF
grub-mkrescue -o zealous.iso isodir
