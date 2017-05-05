cmake_minimum_required(VERSION 3.5.0)
project(test_jansson)

include_directories(inc)
set(LINK_LIBS libjansson.so libuuid.so)

file(GLOB SRC src/*.cpp)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")
endif()

add_executable(test_jansson ${SRC})
target_link_libraries(test_jansson ${LINK_LIBS})