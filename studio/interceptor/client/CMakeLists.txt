project(ac_client)

set(SOURCE_FILES client_cli.cpp client.cpp ../interceptor.cpp)
set(HEADER_FILES client.h ../signal_handler.h ../interceptor.h)
add_executable(ac_client ${SOURCE_FILES} ${HEADER_FILES})

if(WIN32)
  target_link_libraries(ac_client ACEd)
else(WIN32)
  target_link_libraries(ac_client ACE)
endif(WIN32)

if(${CMAKE_SYSTEM} MATCHES "Linux")
  target_link_libraries(ac_client pthread)
endif(${CMAKE_SYSTEM} MATCHES "Linux")

