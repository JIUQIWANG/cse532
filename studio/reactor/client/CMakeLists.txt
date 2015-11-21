set(SOURCE_FILES client_cli.cpp client.cpp)
set(HEADER_FILES client.h)
add_executable(reactor_client ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(reactor_client -lACE)
