set(SOURCE_FILES server_cli.cpp server.cpp ../ReadHandler.cpp)
set(HEADER_FILES server.h ../ReadHandler.h)

add_executable(reactor_server ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(reactor_server -lACE)
