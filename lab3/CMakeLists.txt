cmake_minimum_required(VERSION 3.0)

project(Lab3)

include_directories(/usr/include)
link_directories(/usr/lib/x86_64-gnu-linux)

add_subdirectory(producer)
add_subdirectory(director)