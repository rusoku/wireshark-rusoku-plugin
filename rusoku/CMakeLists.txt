cmake_minimum_required(VERSION 3.30.5)
project(wireshark_rusoku_extcap VERSION 0.0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(PROJECT_NAME RusokuExtcapPlugin)

if(WIN32)
    set(SOURCES
            src/comm_rusoku_win.c
            inc/comm_rusoku_win.h
    )
elseif(APPLE)
    set(SOURCES
            inc/CANAPI_Types.h
            src/comm_rusoku_apple.c
            inc/comm_rusoku_apple.h
    )
endif()

add_executable(${PROJECT_NAME} ${SOURCES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} RusokuExtcapLib)

if(WIN32)
    message("OS:win32")
    set(CMAKE_EXE_LINKER_FLAGS "-static") # for C
    #set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static") # for C++ //dependency walker
    target_link_libraries(${PROJECT_NAME} -lpthread -ldl)
elseif(APPLE)
    message("OS:apple")
    add_definitions(-DOPTION_CAN_2_0_ONLY)
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

