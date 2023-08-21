# objcaller
A program for calling functions in object files.

### ⚠️ The program is currently broken. ⚠️

## Requirements
- [CMake](https://cmake.org)
- [Make](https://www.gnu.org/software/make/)
- [C compiler](https://en.wikipedia.org/wiki/List_of_compilers#C_compilers) (such as [gcc](https://gcc.gnu.org/) or [clang](https://clang.llvm.org/))
- [C standard library](https://en.wikipedia.org/wiki/C_standard_library) (such as [glibc](https://www.gnu.org/software/libc/))

## Compile
You can compile debug or release version by running CMake with `CMAKE_BUILD_TYPE` set to either "Debug" or "Release".
### Debug
```sh
cmake -DCMAKE_BUILD_TYPE=Debug .
make
```
### Release
```sh
cmake -DCMAKE_BUILD_TYPE=Release .
make
```

## Usage
Currently it only takes path to a library as the first command line argument, but it will basically only work with the test library that's included in the project.
```
debug/objcaller ./libtest.so
```
```
release/objcaller ./libtest.so
```
