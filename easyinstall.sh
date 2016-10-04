#!/bin/bash

# set up environment
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"
mkdir -p $HOME/opt/cross
mkdir -p $HOME/src

# build binutils
cd $HOME/src
mkdir build-binutils
wget https://ftp.gnu.org/gnu/binutils/binutils-2.27.tar.gz
tar xzvf ./binutils-2.27.tar.gz
rm ./binutils-2.27.tar.gz
cd build-binutils
../binutils-2.27/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install

# build gcc
cd $HOME/src
mkdir build-gcc
wget https://ftp.gnu.org/gnu/gcc/gcc-6.2.0/gcc-6.2.0.tar.gz
tar xzvf ./gcc-6.2.0.tar.gz
rm ./gcc-6.2.0.tar.gz
cd build-gcc
../gcc-6.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc

printf "\nexport PATH=\"\$HOME/opt/cross/bin:\$PATH\"\n" >> $HOME/.bashrc
