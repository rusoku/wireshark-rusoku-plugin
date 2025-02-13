cmake_minimum_required(VERSION 3.30.5)
project(wireshark_rusoku_extcap VERSION 0.0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(PROJECT_NAME KvaserExtcapPlugin)

if(WIN32)
    set(SOURCES
        src/comm_kvaser_win.c
        inc/comm_kvaser_win.h
)
elseif(APPLE)
    set(SOURCES
        src/comm_kvaser_apple.c
        inc/comm_kvaser_apple.h
)
endif()

add_executable(${PROJECT_NAME} ${SOURCES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} RusokuExtcapLib)

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

