project(proactor_server)
set(SOURCE_FILES server_cli.cpp server_acceptor.cpp ../signal_handler.cpp)
set(HEADER_FILES server_acceptor.h ../signal_handler.h)

add_executable(proactor_server ${SOURCE_FILES} ${HEADER_FILES})

if(WIN32)
  target_link_libraries(proactor_server ACEd)
else(WIN32)
  target_link_libraries(proactor_server ACE)
endif(WIN32)

if(${CMAKE_SYSTEM} MATCHES "Linux")
  target_link_libraries(proactor_server pthread)
endif(${CMAKE_SYSTEM} MATCHES "Linux")
