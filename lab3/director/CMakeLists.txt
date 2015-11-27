cmake_minimum_required(VERSION 3.0)

project(director)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES director_cli.cpp director.cpp director_acceptor.cpp liveness_sender.cpp ../signal_handler.cpp ../sender.cpp play.cpp player.cpp ../string_util.cpp ../common.cpp)
set(HEADER_FILES director_acceptor.h director.h liveness_sender.h ../sender.h play.h player.h ../string_util.h ../common.h ../signal_handler.h)

add_executable(director ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(director ACE)

if(${CMAKE_SYSTEM} MATCHES "Linux")
  target_link_libraries(director pthread)
endif(${CMAKE_SYSTEM} MATCHES "Linux")
