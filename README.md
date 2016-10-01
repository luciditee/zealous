# Zealous
## About
Zealous is a hobbyist operating system designed to run in kernel and user-mode that is roughly POSIX.1 compliant, allowing many Linux and BSD programs to be compiled and run as they would on a more mature OS.  Its goal is to become freestanding and eventually self-hosting, and in the long-term, its goal is to remain modular enough that it can be scaled to the needs of the smallest embedded systems to the highest-end servers (read more [here](http://wiki.osdev.org/Microkernel) about some of the concepts utilized by the Zealous kernel), so long as they run on an Intel processor.  Support for other processor architectures, such as ARM or SPARC, is not planned at this time.

The purpose of the OS is to firstly teach its designers about the concepts found in kernel design, and secondly streamline its ABI to be as low-footprint as possible, with the goal of performance and modularity.

Zealous is named for its creator's well-intended (but often ill-planned) ambition.

## License
Zealous is released as [free software](https://en.wikipedia.org/wiki/Free_software) under the [Apache 2.0 License](https://github.com/sigtau/zealous/blob/master/LICENSE), permitting you to use, distribute, and adapt this software free of charge, provided you release all warranty and **document changes made** in your released adaptations of the software.
