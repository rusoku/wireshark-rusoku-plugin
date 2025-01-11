cmake_minimum_required(VERSION 3.30.5)
project(wireshark_rusoku_extcap VERSION 0.0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(PROJECT_NAME RusokuExtcapLib)

set(SOURCES
        src/gopt.c
        src/main.c
        inc/gopt.h
        inc/main.h
        inc/pcap.h
        inc/control_packet_headers.h
        src/pcap.c
        src/capture.c
        inc/capture.h
        src/comm_base.c
        inc/comm_base.h
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
#add_executable(${PROJECT_NAME} ${SOURCES})
#TARGET_LINK_LIBRARIES(${PACKAGE_NAME} RusokuExtcapPlugin)

if(WIN32)
    message("OS:win32")
    target_link_libraries(${PROJECT_NAME} -lpthread -ldl)
elseif(APPLE)
    message("OS:apple")
    target_link_libraries(${PROJECT_NAME} -lpthread)
#    list(APPEND CMAKE_BUILD_RPATH ${RPATH})
    set(RPATH /usr/local/lib)
    set_target_properties(${PROJECT_NAME}
            PROPERTIES
            LINK_FLAGS "-Wl,-rpath ${RPATH}"
    )
elseif(UNIX)
    message("OS:unix")
endif()

