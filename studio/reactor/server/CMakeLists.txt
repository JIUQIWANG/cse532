set(SOURCE_FILES server.cpp)
add_executable(reactor_server ${SOURCE_FILES})

target_link_libraries(reactor_server -lACE)