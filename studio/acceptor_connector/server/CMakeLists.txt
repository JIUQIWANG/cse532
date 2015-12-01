set(SOURCE_FILES server_cli.cpp server_acceptor.cpp ../socket_handler.cpp)
set(HEADER_FILES server_acceptor.h ../socket_handler.h ../signal_handler.h)

add_executable(ac_server ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(ac_server -lACE)
if(${CMAKE_SYSTEM} MATCHES "Linux")
  target_link_libraries(producer pthread)
endif(${CMAKE_SYSTEM} MATCHES "Linux")
