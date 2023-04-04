# Parallel Matrix Multiplication using HPX


## Build
```
mkdir build
cd build
cmake ..
make
```
If you have HPX added to path you can just `find_package(HPX QUIET)` in camke file and remove the `set(CMAKE_PREFIX_PATH ...)` line
## Run
```
./matrix_mul
```
output
