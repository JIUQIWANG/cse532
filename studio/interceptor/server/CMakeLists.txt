project(ac_server)

set(SOURCE_FILES server_cli.cpp server_acceptor.cpp ../socket_handler.cpp)
set(HEADER_FILES server_acceptor.h ../socket_handler.h ../signal_handler.h)

add_executable(ac_server ${SOURCE_FILES} ${HEADER_FILES})

if(WIN32)
  target_link_libraries(ac_server ACEd)
else(WIN32)
  target_link_libraries(ac_server ACE)
endif(WIN32)

if(${CMAKE_SYSTEM} MATCHES "Linux")
  target_link_libraries(ac_server pthread)
endif(${CMAKE_SYSTEM} MATCHES "Linux")
