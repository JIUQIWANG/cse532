cmake_minimum_required(VERSION 3.0)

project(Lab3)

if (WIN32)
	include_directories("H:\\ACE-6.2.1\\ACE_wrappers")
	link_directories("H:\\ACE-6.2.1\\ACE_wrappers\\lib")	
else (WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	include_directories(/usr/include)
	include_directories(/usr/local/include)
	link_directories(/usr/lib/x86_64-gnu-linux /usr/local/lib)
endif(WIN32)

add_subdirectory(producer)
add_subdirectory(director)