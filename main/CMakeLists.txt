cmake_minimum_required(VERSION 3.29.6)
project(wireshark_rusoku_extcap VERSION 0.0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(PROJECT_NAME RusokuExtcapLib)

set(SOURCES
        src/gopt.c
        inc/gopt.h
        inc/main.h
        inc/pcap.h
        src/pcap.c
        src/capture.c
        inc/capture.h
        src/comm_base.c
        inc/comm_base.h
        src/main.c
        src/pcap_debug.c
        inc/pcap_debug.h
        src/control_packet.c
        inc/control_packet.h
        inc/pcap_param.h
        src/pcap_param.c
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
#add_executable(${PROJECT_NAME} ${SOURCES})
#TARGET_LINK_LIBRARIES(${PACKAGE_NAME} RusokuExtcapPlugin)

#target_link_libraries(main -Wl,-Bstatic bingitup -Wl,-Bdynamic)

if(WIN32)
    message("OS:win32")
    #target_link_libraries(${PROJECT_NAME} -nodefaultlibs -lpthread -ldl)
    target_link_libraries(${PROJECT_NAME} -static -lpthread -dl)

elseif(APPLE)
    message("OS:apple")
    target_link_libraries(${PROJECT_NAME} -lpthread)
#    list(APPEND CMAKE_BUILD_RPATH ${RPATH})
    set(RPATH /usr/local/lib)
    set_target_properties(${PROJECT_NAME}
            PROPERTIES
            LINK_FLAGS "-Wl,-rpath ${RPATH} -Wunknown-escape-sequence"
    )
elseif(UNIX)
    message("OS:unix")
endif()

