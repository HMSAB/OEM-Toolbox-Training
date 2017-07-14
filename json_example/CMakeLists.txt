cmake_minimum_required(VERSION 3.5.0)
project(json_example)

include_directories(inc)
set(LINK_LIBS libjansson.so libuuid.so libzmq.so)

file(GLOB SRC src/*.cpp)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")
endif()

add_executable(json_example ${SRC})
target_link_libraries(json_example ${LINK_LIBS})