#cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(Coordinates)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "-std=c++1y ${CMAKE_CXX_FLAGS}")
  set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -undefined dynamic_lookup")
endif()
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
set(CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)
SET(PARTICLE_LIB_PATH "${CMAKE_CURRENT_BINARY_DIR}/../DreamFunction")
find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist Tree Net EG)
include(${ROOT_USE_FILE})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${binFolder}/Coordinates)
include_directories(${CMAKE_SOURCE_DIR} ${PROJECT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src ${ROOT_INCLUDE_DIRS})
add_definitions(${ROOT_CXX_FLAGS})

set(SRCS
  CoordSampler.cxx
  )

set(HEADERS
  CoordSampler.h
  )
add_library(libCoordinates STATIC ${SRCS})
set_target_properties(libCoordinates PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
target_link_libraries(libCoordinates ${ROOT_LIBRARIES})

add_executable(SourceSampler ${PROJECT_SOURCE_DIR}/Scripts/Coordinator.C)
target_link_libraries(SourceSampler libCoordinates -L${PARTICLE_LIB_PATH} libParticle)

