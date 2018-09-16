cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

message(STATUS "CMAKE_BUILD_TYPE           = " ${CMAKE_BUILD_TYPE})

if (CMAKE_BUILD_TYPE STREQUAL Debug)
  message(STATUS "This is a DEBUG build!")
elseif (CMAKE_BUILD_TYPE STREQUAL Release)
  message(STATUS "This is a RELEASE build!")
endif()

project(derid)

set(PROJECT_VERSION "0.0.1")

message(STATUS "CMAKE_ROOT: " ${CMAKE_ROOT} )
message(STATUS "CMAKE_SYSTEM_NAME          = ${CMAKE_SYSTEM_NAME}"         )
message(STATUS "CMAKE_SYSTEM: " ${CMAKE_SYSTEM} )
message(STATUS "CMAKE_SYSTEM_VERSION: " ${CMAKE_SYSTEM_VERSION} )
message(STATUS "CMAKE_SYSTEM_PROCESSOR: " ${CMAKE_SYSTEM_PROCESSOR} )
message(STATUS "CMAKE_CXX_COMPILER_ID      = ${CMAKE_CXX_COMPILER_ID}"     )
message(STATUS "CMAKE_CXX_COMPILER_VERSION = ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "CMAKE_COMPILER_IS_GNUCXX   = ${CMAKE_COMPILER_IS_GNUCXX}"  )
message(STATUS "CMAKE_INCLUDE_PATH: " ${CMAKE_INCLUDE_PATH} )
message(STATUS "CMAKE_LIBRARY_PATH: " ${CMAKE_LIBRARY_PATH} )

set(EXECUTABLE_NAME "derid")

set(CMAKE_CXX_STANDARD 14)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
  message(STATUS "Setting definition -O0")
  add_definitions(-O0)
  set(CMAKE_CXX_FLAGS "-g -Wall -pedantic")
else()
  message(STATUS "Setting definition -O3")
  add_definitions(-O3)
  set(CMAKE_CXX_FLAGS "-Wall -pedantic")
endif()

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost REQUIRED COMPONENTS filesystem log)
find_package(Curses REQUIRED)

find_package(PkgConfig)

pkg_check_modules(GTEST "gtest")

message(STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS} )
message(STATUS "BUILD_SHARED_LIBS: " ${BUILD_SHARED_LIBS} )

add_executable(${EXECUTABLE_NAME}
    src/derid.cpp
    src/buffer.cpp
    src/list.cpp
    src/curses.cpp
)

if ( CMAKE_COMPILER_IS_GNUCC )
  message(STATUS "#warning extension is disabled")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-cpp")
endif ( CMAKE_COMPILER_IS_GNUCC )

include_directories(${Boost_INCLUDE_DIRS} ${CURSES_INCLUDE_DIRS})
target_link_libraries(${EXECUTABLE_NAME} ${Boost_LIBRARIES} ${CURSES_LIBRARIES} -lpthread -ldl)

# Tests

if (GTEST_FOUND)
  if (BUILD_TESTS)
    message(STATUS "Building tests")
  enable_testing()

  add_executable(test_readdir
    tests/test_readdir.cpp
    src/buffer.cpp
    )

  add_test(NAME test_readdir COMMAND ./test_readdir)

  target_link_libraries(test_readdir ${GTEST_LIBRARIES} ${Boost_LIBRARIES} -lpthread -ldl)

  else ()
  message(STATUS "Not building tests (BUILD_TESTS flag)")
  endif ()
else ()
  message(WARNING "GTEST not found")
endif ()
