cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(binFolder "${CMAKE_CURRENT_BINARY_DIR}/../install")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${binFolder}/lib)
project(GentleSource CXX C)
enable_testing()

add_subdirectory(Particles)
add_subdirectory(Coordinates)
add_subdirectory(PythiaPairer)


