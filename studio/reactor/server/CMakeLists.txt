set(SOURCE_FILES server_cli.cpp server_acceptor.cpp ../socket_handler.cpp)
set(HEADER_FILES server_acceptor.h ../socket_handler.h)

add_executable(reactor_server ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(reactor_server -lACE)
