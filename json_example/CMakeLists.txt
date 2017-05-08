cmake_minimum_required(VERSION 3.5.0)
project(test_json)

if(${CMAKE_CXX_COMPILER} STREQUAL "/usr/bin/c++")
    find_package(ZeroMQ REQUIRED)
else()

endif()

include_directories(inc)
set(LINK_LIBS libjansson.so libuuid.so)

file(GLOB SRC src/*.cpp)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")
endif()

add_executable(test_json ${SRC})

if(${CMAKE_CXX_COMPILER} STREQUAL "/usr/bin/c++")
    target_link_libraries(test_json ${LINK_LIBS} libzmq)
else()
    target_link_libraries(test_json ${LINK_LIBS} zmq)
endif()

