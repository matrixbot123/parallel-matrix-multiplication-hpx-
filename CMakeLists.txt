# CMakeLists.txt
cmake_minimum_required(VERSION 3.12)

project(matrix_mul VERSION 0.1.0)
set(CMAKE_PREFIX_PATH "/home/matrix/gsoc/stellar_hpx/spack/opt/spack/linux-ubuntu20.04-skylake/gcc-9.4.0/hpx-1.8.1-mh64vijnz2c4xycgchbnqq2vtk7kilij" ${CMAKE_PREFIX_PATH})
find_package(HPX REQUIRED)

add_executable(matrix_mul  main_hpx.cpp)
target_link_libraries(matrix_mul  PRIVATE HPX::hpx HPX::wrap_main HPX::iostreams_component)
