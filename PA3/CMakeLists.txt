cmake_minimum_required(VERSION 3.1.0)
project(PA3 VERSION 0.1.0)

include(CTest)
enable_testing()

file(GLOB PA3_lib
    "*.h"
    "*.cpp"
)

add_executable(PA3 ${PA3_lib})

if (POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif ()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
include(CPack)

set(CMAKE_CXX_FLAGS_DEBUG "$${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE "$${CMAKE_CXX_FLAGS_RELEASE} -std=c++11")