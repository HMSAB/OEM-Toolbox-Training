cmake_minimum_required( VERSION 3.5.0)

project(update_time)

#add CURL package
find_package(CURL REQUIRED)


#Headers
include_directories(include)

#sources
file( GLOB SOURCES "src/*.c")

add_executable(update_time ${SOURCES})
target_link_libraries(update_time ${CURL_LIBRARIES} )