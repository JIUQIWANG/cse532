project(proactor_server)
set(SOURCE_FILES server_cli.cpp server_acceptor.cpp ../socket_handler.cpp)
set(HEADER_FILES server_acceptor.h ../socket_handler.h ../signal_handler.h)

add_executable(proactor_server ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(proactor_server -lACE)
if(${CMAKE_SYSTEM} MATCHES "Linux")
  target_link_libraries(proactor_server pthread)
endif(${CMAKE_SYSTEM} MATCHES "Linux")
