cmake_minimum_required( VERSION 3.5.0)

project(mb_init)

#add CURL package
find_package(CURL REQUIRED)
set(SDK_LINK_LIBS libnbsystem.so libnbconfig.so)


#Headers
include_directories(include)

#sources
file( GLOB SOURCES "src/*.c")

add_executable(mb_init ${SOURCES})
target_link_libraries(mb_init ${CURL_LIBRARIES} ${SDK_LINK_LIBS} )