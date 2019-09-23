#cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(Particles)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "-std=c++1y ${CMAKE_CXX_FLAGS}")
  set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -undefined dynamic_lookup")
endif()
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
set(CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)
set(COORD_PATH "${CMAKE_SOURCE_DIR}/Coordinates")#where are all CATS related .h files
set(COORD_LIB_PATH "${CMAKE_CURRENT_BINARY_DIR}/../Coordinates")
find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist Tree Net EG)
include(${ROOT_USE_FILE})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${binFolder}/Particle)
include_directories(${CMAKE_SOURCE_DIR} ${PROJECT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src ${ROOT_INCLUDE_DIRS} ${COORD_PATH})
add_definitions(${ROOT_CXX_FLAGS})

set(SRCS
  Particle.cxx
  Pair.cxx
  )

set(HEADERS
  Particle.h
  Pair.cxx
  )
add_library(Particle STATIC ${SRCS})
set_target_properties(Particle PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
target_link_libraries(Particle ${ROOT_LIBRARIES} -L${COORD_LIB_PATH} Coordinates)

add_executable(BoostTest ${PROJECT_SOURCE_DIR}/Scripts/BoostTest.C)
target_link_libraries(BoostTest Particle)


