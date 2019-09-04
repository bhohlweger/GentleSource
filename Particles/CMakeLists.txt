#cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(Particles)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "-std=c++1y ${CMAKE_CXX_FLAGS}")
  set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -undefined dynamic_lookup")
endif()
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
set(CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)
find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist Tree Net EG)
include(${ROOT_USE_FILE})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${binFolder}/Particle)
include_directories(${CMAKE_SOURCE_DIR} ${PROJECT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src ${ROOT_INCLUDE_DIRS})
add_definitions(${ROOT_CXX_FLAGS})

set(SRCS
  Particle.cxx
  Pair.cxx
  )

set(HEADERS
  Particle.h
  Pair.cxx
  )
add_library(libParticle STATIC ${SRCS})
set_target_properties(libParticle PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
target_link_libraries(libParticle ${ROOT_LIBRARIES})

add_executable(BoostTest ${PROJECT_SOURCE_DIR}/Scripts/BoostTest.C)
target_link_libraries(BoostTest libParticle)


