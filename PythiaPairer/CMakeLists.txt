#cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(PythiaPairer)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "-std=c++1y ${CMAKE_CXX_FLAGS}")
  set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -undefined dynamic_lookup")
endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${binFolder}/PythiaPairer)

set(COORD_PATH "${CMAKE_SOURCE_DIR}/Coordinates")
set(COORD_LIB_PATH "${CMAKE_CURRENT_BINARY_DIR}/../Coordinates")

set(PART_PATH "${CMAKE_SOURCE_DIR}/Particle")
set(PART_LIB_PATH "${CMAKE_CURRENT_BINARY_DIR}/../Particle")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
set(CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)

find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist Tree Net EG)
include(${ROOT_USE_FILE})
add_definitions(${ROOT_CXX_FLAGS})

include_directories(${CMAKE_SOURCE_DIR} ${PROJECT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src ${ROOT_INCLUDE_DIRS} ${COORD_PATH} ${PART_PATH} $ENV{PYTHIA8_INCLUDE_DIR})

add_executable(PythiaRun ${PROJECT_SOURCE_DIR}/PythiaRun.C)
target_link_libraries(PythiaRun -L${COORD_LIB_PATH} Coordinates -L${PART_LIB_PATH} Particle -L$ENV{PYTHIA8_LIBRARY} pythia8)


