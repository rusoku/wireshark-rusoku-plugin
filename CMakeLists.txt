cmake_minimum_required(VERSION 3.30.5)
project(wireshark_rusoku_extcap C)

set(CMAKE_C_STANDARD 11)
set(PROJECT_NAME rusoku-wirechark-plugin)

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
        src/comm_base.c
        include/comm_base.h
        src/comm_rusoku.c
        include/comm_rusoku.h
)

add_executable(${PROJECT_NAME} ${SOURCES})

if(WIN32)
    message("OS:win32")
    target_link_libraries(${PROJECT_NAME} -lpthread -ldl)
elseif(APPLE)
    message("OS:apple")
    target_link_libraries(${PROJECT_NAME} -lpthread)
    list(APPEND CMAKE_BUILD_RPATH ${RPATH})
    set(RPATH /usr/local/lib)
    set_target_properties(${PROJECT_NAME}
            PROPERTIES
            LINK_FLAGS "-Wl,-rpath ${RPATH}"
    )
elseif(UNIX)
    message("OS:unix")
endif()

