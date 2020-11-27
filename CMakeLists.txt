cmake_minimum_required(VERSION 3.6)

project(Gamma)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(GAMMA_USE_CXX17 "Use the C++ 17 language standard" ON)
if(GAMMA_USE_CXX17)
	add_definitions("-DCXX17" "-DGAMMA_CXX17")
	set(CMAKE_CXX_STANDARD 17)
else(GAMMA_USE_CXX17)
	# Just use C++ 14 then
	# If your compiler does not support C++ 14, you need to update it
	add_definitions("-DCXX14" "-DGAMMA_CXX14")
	set(CMAKE_CXX_STANDARD 14)
endif(GAMMA_USE_CXX17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(GAMMA_DISABLE_WARNINGS "Disable warnings while building" OFF)

if(GAMMA_DISABLE_WARNINGS)
	if ( CMAKE_COMPILER_IS_GNUCC )
		set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")
	elseif( MSVC )
		set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
	endif()
endif(GAMMA_DISABLE_WARNINGS)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(vendor)
add_subdirectory(Gamma)
add_subdirectory(Demo)



