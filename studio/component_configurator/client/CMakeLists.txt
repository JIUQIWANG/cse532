set(SOURCE_FILES client_cli.cpp client.cpp ../common.h ../string_util.h ../thread_guard.h)
set(HEADER_FILES client.h ../signal_handler.h ../common.cpp ../string_util.cpp)

add_executable(cc_client ${SOURCE_FILES} ${HEADER_FILES})

if(WIN32)
  target_link_libraries(cc_client ACEd)
else(WIN32)
  target_link_libraries(cc_client ACE)
endif(WIN32)

if(${CMAKE_SYSTEM} MATCHES "Linux")
  target_link_libraries(cc_client pthread)
endif(${CMAKE_SYSTEM} MATCHES "Linux")

