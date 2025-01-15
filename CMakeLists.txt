cmake_minimum_required(VERSION 3.30.5)
project(wireshark_rusoku_extcap C)

set(CMAKE_C_STANDARD 17)
set(PROJECT_NAME RusokuWiresharkPlugins)

ADD_SUBDIRECTORY(main)
ADD_SUBDIRECTORY(rusoku)
#ADD_SUBDIRECTORY(kvaser)
