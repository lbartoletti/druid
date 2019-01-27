# Druid

## Hook

````
$ ln -s ../../scripts/pre-commit-clang-format .git/hooks/pre-commit
````

## Cross-Compiling

````
$ mkdir build
$ cd build
$ cmake -DCMAKE_TOOLCHAIN_FILE=../arm-toolchain.cmake
$ make
````
