cmake_minimum_required(VERSION 3.0)

project(director)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES director_cli.cpp director.cpp play.cpp player.cpp string_util.cpp)
set(HEADER_FILES director.h play.h player.h string_util.h ../common.h ../signal_handler.h)

add_executable(director ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(director ACE)

if(${CMAKE_SYSTEM} MATCHES "Linux")
  target_link_libraries(director pthread)
endif(${CMAKE_SYSTEM} MATCHES "Linux")
