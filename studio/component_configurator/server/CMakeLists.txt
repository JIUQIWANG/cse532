set(SOURCE_FILES server_cli.cpp server_acceptor.cpp ../socket_handler.cpp ../common.cpp)
set(HEADER_FILES server_acceptor.h ../socket_handler.h ../signal_handler.h ../common.h)

add_executable(cc_server ${SOURCE_FILES} ${HEADER_FILES})

if(WIN32)
  target_link_libraries(cc_server ACEd)
else(WIN32)
  target_link_libraries(cc_server ACE)
endif(WIN32)

if(${CMAKE_SYSTEM} MATCHES "Linux")
  target_link_libraries(cc_server pthread)
endif(${CMAKE_SYSTEM} MATCHES "Linux")
