cmake_minimum_required(VERSION 3.0)

project(producer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES producer_cli.cpp producer.cpp producer_ace.cpp ../outputHandler.cpp playlist.cpp)
set(HEADER_FILES producer.h producer_ace.h playlist.h ../outputHandler.h ../signal_handler.h ../common.h)

add_executable(producer ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(producer ACE)
if(${CMAKE_SYSTEM} MATCHES "Linux")
  target_link_libraries(producer pthread)
endif(${CMAKE_SYSTEM} MATCHES "Linux")
