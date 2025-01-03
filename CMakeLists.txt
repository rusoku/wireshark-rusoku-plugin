cmake_minimum_required(VERSION 3.6.1)
project(wireshark_rusoku_extcap C)

set(CMAKE_C_STANDARD 23)
set(PROJECT_NAME rusoku-wirechark-plugin )
#set(RPATH "/usr/local/lib")
#list(APPEND CMAKE_BUILD_RPATH ${RPATH})

# List all source files
set(SOURCES
        src/gopt.c
        src/main.c
        include/gopt.h
        src/main.c
        include/main.h
        include/pcap.h
        include/control_packet_headers.h
        src/pcap.c
        src/capture.c
        include/capture.h
)

#find_library(LIBRT rt)
#if(LIBRT)
#    target_link_libraries(${PROJECT_NAME} ${LIBRT})
#endif()

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} -lpthread)

#set_target_properties(${PROJECT_NAME}
#       PROPERTIES
#       LINK_FLAGS "-Wl,-rpath,${RPATH} -lpthread -lrt"
#)

