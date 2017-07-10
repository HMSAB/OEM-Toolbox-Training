cmake_minimum_required( VERSION 3.5.0)

project(iothub_mqtt)

#compileAsC99()


# Search OpenSSL
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)
find_package(CURL REQUIRED)




set(iothub_client_sample_mqtt_c_files
    src/iothub_client_NB301_mqtt.c src/nb_zmq_mbtcp.c
)

set(iothub_client_sample_mqtt_h_files
    inc/iothub_client_sample_mqtt.h
)


include_directories(inc)
link_directories(libs)

add_executable(iothub_mqtt ${iothub_client_sample_mqtt_c_files} ${iothub_client_sample_mqtt_h_files})

target_link_libraries(iothub_mqtt  
    iothub_client_mqtt_transport
    iothub_client 
    aziotsharedutil
    umqtt
    ${CMAKE_THREAD_LIBS_INIT} 
    ${CURL_LIBRARIES}
    ${OPENSSL_LIBRARIES} 
    m
)

