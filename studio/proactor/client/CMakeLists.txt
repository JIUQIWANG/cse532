project(proactor_client)
set(SOURCE_FILES client_cli.cpp client.cpp)
set(HEADER_FILES client.h ../signal_handler.h)
add_executable(proactor_client ${SOURCE_FILES} ${HEADER_FILES})

if(WIN32)
  target_link_libraries(proactor_client ACEd)
else(WIN32)
  target_link_libraries(proactor_client ACE)
endif(WIN32)

if(${CMAKE_SYSTEM} MATCHES "Linux")
  target_link_libraries(proactor_client pthread)
endif(${CMAKE_SYSTEM} MATCHES "Linux")

