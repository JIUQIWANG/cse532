cmake_minimum_required(VERSION 3.0)

project(producer)

set(SOURCE_FILES producer_cli.cpp)
set(HEADER_FILES)

add_executable(producer ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(producer ACE)
