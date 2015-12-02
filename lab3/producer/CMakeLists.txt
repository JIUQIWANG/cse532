cmake_minimum_required(VERSION 3.0)

project(producer)

set(SOURCE_FILES producer_cli.cpp producer.cpp producer_acceptor.cpp  playlist.cpp liveness_checker.cpp ../signal_handler.cpp ../sender.cpp ../string_util.cpp ../common.cpp)
set(HEADER_FILES producer.h producer_acceptor.h playlist.h liveness_checker.h ../sender.h ../signal_handler.h ../common.h ../string_util.h)

add_executable(producer ${SOURCE_FILES} ${HEADER_FILES})

if(WIN32)
	target_link_libraries(producer ACEd)
else(WIN32)
	target_link_libraries(producer ACE)
endif(WIN32)

if(${CMAKE_SYSTEM} MATCHES "Linux")
  target_link_libraries(producer pthread)
endif(${CMAKE_SYSTEM} MATCHES "Linux")
