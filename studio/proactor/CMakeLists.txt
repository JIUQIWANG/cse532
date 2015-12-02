cmake_minimum_required(VERSION 3.0)

project(proactor)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(/usr/include)
link_directories(/usr/local/lib)
link_directories(/usr/lib/x86_64-gnu-linux)

add_subdirectory(client)
add_subdirectory(server)
