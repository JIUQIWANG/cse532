set(SOURCE_FILES client.cpp)
add_executable(reactor_client ${SOURCE_FILES})

target_link_libraries(reactor_client -lACE)