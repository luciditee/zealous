# Zealous

## About
Zealous is a hobbyist operating system designed to run in kernel and user-mode that is roughly POSIX.1 compliant, allowing many Linux and BSD programs to be compiled and run as they would on a more mature OS.  Its goal is to become freestanding and eventually self-hosting, and in the long-term, its goal is to remain modular enough that it can be scaled to the needs of the smallest embedded systems to the highest-end servers (read more [here](http://wiki.osdev.org/Microkernel) about some of the concepts utilized by the Zealous kernel), so long as they run on an Intel processor.  Support for other processor architectures, such as ARM or SPARC, is not planned at this time.

The purpose of the OS is to firstly teach its designers about the concepts found in kernel design, and secondly streamline its ABI to be as low-footprint as possible, with the goal of performance and modularity.

Zealous is named for its creator's well-intended (but often ill-planned) ambition.

## License
Zealous is released as [free software](https://en.wikipedia.org/wiki/Free_software) under the [Apache 2.0 License](https://github.com/sigtau/zealous/blob/master/LICENSE), permitting you to use, distribute, and adapt this software free of charge, provided you release all warranty and **document changes made** in your released adaptations of the software.

## More Information

Information on the architecture of Zealous will be added to other files within this repository as needed.  For now, the kernel needs major expansion to justify further documentation.

## How to Build/Run
**It is not presently recommended to run Zealous on bare-metal hardware.**  Doing so is likely possible on a standard ix86 compatible PC, but may result in unexpected behavior.  *Until further notice, Zealous is designed to be run in virtual machines such as QEMU, VirtualBox, Bochs, or VMWare.*

Despite the above warning, the source repository here provides a shell script `iso.sh`to generate an ISO image.

Compiling the Zealous kernel requires the use of a UNIX-based shell (Linux, \*BSD, macOS, Cygwin, Windows Subsystem for Linux) and a *cross-compiler*, or a compiler built to run on one system but produce binaries for another.  There are two easy ways to do this: use the provided shell script to automatically acquire and install the needed software, or build the cross-compiler yourself.

### Prerequisites
No matter which method of installation you choose, you'll need to acquire the **latest versions** of the following packages in your distro of choice:

* [binutils](https://www.gnu.org/software/binutils/)
* [texinfo](https://www.gnu.org/software/texinfo/)
* [gcc](https://gcc.gnu.org/)
* [make](https://www.gnu.org/software/make/) (version 4.0 or later, see `make --version` for yours)
* [qemu](http://wiki.qemu.org/Main_Page)
* [wget](https://www.gnu.org/software/wget/)

For Debian-based distros, this command will install gcc, binutils, and make:

`sudo apt-get install build-essential`

If you wish to create a bootable ISO, you need the following packages:

* [grub-common](https://packages.debian.org/sid/grub-common)
* [xorriso](https://www.gnu.org/software/xorriso/)

*Note: For generating an ISO, you'll also need a working grub2 image at `/boot/grub`, which will require additional setup in environments such as Cygwin or WSL.*

**Assuming you have a *nix shell with all the above packages ready to go, you can proceed.**

### The Easy Way

If you trust our shell script (check out [its source code](https://raw.githubusercontent.com/sigtau/zealous/master/easyinstall.sh) if you are paranoid), you can paste the following in your shell.  No root access needed.

`$ wget -O - https://raw.githubusercontent.com/sigtau/zealous/master/easyinstall.sh | bash`

If you're *super* paranoid, you can also download the script simply with

`$ wget https://raw.githubusercontent.com/sigtau/zealous/master/easyinstall.sh`

and then invoke `$ ./easyinstall.sh` to install the cross-compiler setup, not before checking to make sure the contents are sane of course.

This script will automatically install a cross-compiler in `/home/youruser/opt/cross` and add its binary directory to your `PATH` variable.  This takes a while, and you'll see it download several files and lots of compilation messages but what you should end up with is an x86 ELF binary compiler capable of emitting the kernel image.

If you get errors during the compilation process, ensure that you have installed the correct prerequisite packages.  and run

`rm -rf ~/src && rm -rf ~/opt`

before running the shell script again.

### The Old-Fashioned Way

The [OSDev Wiki article](http://wiki.osdev.org/GCC_Cross-Compiler) on building a cross-compiler is your ultimate guide to building a cross-compiler the old fashioned way.  Follow this for your platform, and you should be golden.

If you can successfully invoke

`i686-elf-gcc --version`

and it outputs GCC version information, you have all you need to compile Zealous.

## Building Zealous

So long as you have GNU Make 4.0 or higher and the aforementioned cross-compiler installed, you can build Zealous using

`./build.sh`

from within the root of the Git repository.

## Running Zealous

You can run Zealous in QEMU automatically by invoking

`./qemu.sh`

If you are developing, you can automatically hook GDB into QEMU by running

`./debug.sh`

You may need to install GDB for your distro.  Debian-like distros can use

`sudo apt-get install gdb`