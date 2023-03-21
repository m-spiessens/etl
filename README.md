# Embedded Template Library (etl)

Want to develop in C++ on lightweight microcontrollers? This is what you need.

A simple C++ implementation for a linked list of memory fragments, a queue and a pool.

## Get started

It's header-only, just copy the header files in your project.

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
