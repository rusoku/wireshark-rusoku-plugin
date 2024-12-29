cmake_minimum_required(VERSION 3.1)
project(wireshark_rusoku_extcap C)

set(CMAKE_C_STANDARD 11)
set(PROJECT_NAME rusoku-wirechark-plugin )
#set(RPATH "/usr/local/lib")
#list(APPEND CMAKE_BUILD_RPATH ${RPATH})

# List all source files
set(SOURCES
        src/gopt.c
        src/gopt-errors.c
        src/main.c
        include/gopt.h
        src/main.c
        include/main.h
        include/pcap_headers.h
        include/control_packet_headers.h
)

add_executable(${PROJECT_NAME} ${SOURCES})
#set_target_properties(${PROJECT_NAME}
#       PROPERTIES
#       LINK_FLAGS "-Wl,-rpath,${RPATH} -lpthread"
#)