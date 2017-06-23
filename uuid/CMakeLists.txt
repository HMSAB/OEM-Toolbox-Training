cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++0x")

find_package(OpenSSL)
message(OPENSSL_FOUND)

include_directories("./inc")

set (SDK_LINK_LIBS  libnbsystem.so )

add_executable(guid_gen
    src/main.c
    src/uuid.c
)

target_link_libraries(guid_gen
     ${OPENSSL_LIBRARIES}
)


add_executable(test_uuid_gen
    src/uuid_test.c
    src/uuid.c
)

target_link_libraries(test_uuid_gen
     ${OPENSSL_LIBRARIES}
)

add_test(
    NAME test_uuid 
    COMMAND test_uuid_gen 
)