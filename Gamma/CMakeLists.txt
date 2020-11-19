file(GLOB Gamma_SRC
    "src/Gamma/*.h"
    "src/Gamma/*.cpp"
    "src/Gamma/Core/*.h"
    "src/Gamma/Core/*.cpp"
    "src/Gamma/Window/*.h"
    "src/Gamma/Window/*.cpp"
    "src/Gamma/Renderer/*.h"
    "src/Gamma/Renderer/*.cpp"
) 

add_library(Gamma STATIC ${Gamma_SRC})

option(GAMMA_SHARED_LIBRARY "Build Gamma as a shared library" OFF)
if(GAMMA_SHARED_LIBRARY)
	target_compile_definitions(Gamma PUBLIC GAMMA_SHARED_LIBRARY)
endif(GAMMA_SHARED_LIBRARY)
	

target_link_libraries(Gamma PRIVATE SDL2-static Krypton_STATIC)
target_include_directories(Gamma PUBLIC "${CMAKE_SOURCE_DIR}/vendor/SDL2/include" "${CMAKE_SOURCE_DIR}/vendor/Krypton/src")
target_link_libraries(Gamma PUBLIC spdlog)
target_compile_definitions(Gamma PRIVATE GAMMA_BUILD=1)

target_compile_definitions(Gamma PUBLIC GAMMA_GRAPHICS_API_VULKAN GAMMA_GRAPHICS_API_OPENGL)

if(WIN32)
	target_compile_definitions(Gamma PUBLIC GAMMA_PLATFORM_WINDOWS 
				GAMMA_GRAPHICS_API_DIRECT_X 
				GAMMA_GRAPHICS_API_DIRECT_X9
				GAMMA_GRAPHICS_API_DIRECT_X11
				GAMMA_GRAPHICS_API_DIRECT_X12)
	target_link_libraries(Gamma PRIVATE msvcrtd vcruntimed ucrtd)
endif(WIN32)

if (UNIX)
    target_compile_definitions(Gamma PUBLIC GAMMA_PLATFORM_UNIX)
endif (UNIX)

if(MSVC)
	 target_compile_definitions(Gamma PUBLIC GAMMA_MSVC)
endif(MSVC)