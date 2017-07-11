cmake_minimum_required( VERSION 3.5.0)

project(iothub_mbtcp_mqtt)

#compileAsC99()


# Search OpenSSL
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)
find_package(CURL REQUIRED)


set(LINK_LIBS libjansson.so libuuid.so libzmq.so)

set(iothub_client_sample_mqtt_c_files
    src/iothub_client_NB301_mqtt.c src/nb_zmq_mbtcp.c
)

set(iothub_client_sample_mqtt_h_files
    inc/iothub_client_sample_mqtt.h
)


include_directories(inc)
link_directories(libs)

set(AZURE_LIBS libiothub_client_mqtt_transport.a  
    libiothub_client.a 
    libaziotsharedutil.a
    libumqtt.a )

add_executable(iothub_mbtcp_mqtt ${iothub_client_sample_mqtt_c_files} ${iothub_client_sample_mqtt_h_files})

target_link_libraries(iothub_mbtcp_mqtt  
    ${LINK_LIBS}
    ${AZURE_LIBS}
    ${CMAKE_THREAD_LIBS_INIT} 
    ${CURL_LIBRARIES}
    ${OPENSSL_LIBRARIES} 
    m
)

