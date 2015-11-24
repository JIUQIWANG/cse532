cmake_minimum_required(VERSION 3.0)

project(Lab3)

include_directories(/usr/include)
include_directories(/usr/local/include)
link_directories(/usr/lib/x86_64-gnu-linux /usr/local/lib)

add_subdirectory(producer)
add_subdirectory(director)