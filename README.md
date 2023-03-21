# Embedded Template Library (etl)

A simple C++ implementation for a linked list of memory fragments.
Perfect for concatenating pieces of memory without unnecessary copying.
For example, message encapsulation in a communication stack.

It's header-only, just copy memorychain.h in your project!

## Get started

### VS code:

Open folder and use `ctrl + shift + b` or `ctrl + shift + p` -> `Tasks: Run Test Task`.

### bash:

```bash
[ ! -d build/ ] && mkdir build/
cd build/
cmake ..
cmake --build . --parallel
ctest -V --stop-on-failure
```
