#cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(Coordinates)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "-std=c++1y ${CMAKE_CXX_FLAGS}")
  set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -undefined dynamic_lookup")
endif()
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
set(CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)
set(PARTICLE_PATH "${CMAKE_SOURCE_DIR}/Particles")#where are all CATS related .h files
set(PARTICLE_LIB_PATH "${CMAKE_CURRENT_BINARY_DIR}/../Particles")
find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist Tree Net EG)
include(${ROOT_USE_FILE})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${binFolder}/Coordinates)
include_directories(${CMAKE_SOURCE_DIR} ${PROJECT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src ${ROOT_INCLUDE_DIRS} ${PARTICLE_PATH})
add_definitions(${ROOT_CXX_FLAGS})

set(SRCS
  CoordSampler.cxx
  )

set(HEADERS
  CoordSampler.h
  )
add_library(Coordinates STATIC ${SRCS})
set_target_properties(Coordinates PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
target_link_libraries(Coordinates ${ROOT_LIBRARIES} -L${PARTICLE_LIB_PATH} Particle)

add_executable(SourceSampler ${PROJECT_SOURCE_DIR}/Scripts/Coordinator.C)
target_link_libraries(SourceSampler Coordinates)

