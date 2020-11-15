file(GLOB Gamma_SRC
    "src/*.h"
    "src/*.cpp"
    "src/Core/*.h"
    "src/Core/*.cpp"
    "src/Window/*.h"
    "src/Window/*.cpp"
) 

add_library(Gamma SHARED ${Gamma_SRC})

target_link_libraries(Gamma PRIVATE SDL2-static)
target_include_directories(Gamma PUBLIC vendor/SDL/include)
target_compile_definitions(Gamma PRIVATE GAMMA_BULD=1)

target_compile_definitions(Gamma PUBLIC GAMMA_GRAPHICS_API_VULKAN GAMMA_GRAPHICS_API_OPENGL)

if(WIN32)
	target_compile_definitions(Gamma PUBLIC GAMMA_PLATFORM_WINDOWS 
				GAMMA_GRAPHICS_API_DIRECT_X 
				GAMMA_GRAPHICS_API_DIRECT_X11
				GAMMA_GRAPHICS_API_DIRECT_X12)
endif(WIN32)

if (UNIX)
    target_compile_definitions(Gamma PUBLIC GAMMA_PLATFORM_UNIX)
endif (UNIX)