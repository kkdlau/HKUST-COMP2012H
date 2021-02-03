cmake_minimum_required(VERSION 3.0.0)
project(PA1 VERSION 0.1.0)

include(CTest)
enable_testing()

file(GLOB pa1_lib
    "*.h"
    "*.cpp"
)

add_executable(PA1 ${pa1_lib})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
include(CPack)

set(CMAKE_CXX_FLAGS_DEBUG "$${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")