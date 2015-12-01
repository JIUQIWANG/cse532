set(SOURCE_FILES client_cli.cpp client.cpp)
set(HEADER_FILES client.h ../signal_handler.h)
add_executable(ac_client ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(ac_client -lACE)
