cmake_minimum_required(VERSION 3.8)

option(GAMMA_BUILD_OPENGL "Use the OpenGL API for renderering" ON)
option(GAMMA_BUILD_VULKAN "Use the OpenGL API for renderering" OFF)

if(WIN32)
	option(GAMMA_BUILD_D3D9 "Use the D3D9 API for renderering" OFF)
	option(GAMMA_BUILD_D3D10 "Use the D3D10 API for renderering" OFF)
	option(GAMMA_BUILD_D3D11 "Use the D3D11 API for renderering" OFF)
	option(GAMMA_BUILD_D3D12 "Use the D3D12 API for renderering" OFF)
endif(WIN32)

if(GAMMA_BUILD_OPENGL)
	file(GLOB Gamma_Renderer_SRC "src/Gamma/Renderer/OpenGL/*.h" "src/Gamma/Renderer/OpenGL/*.cpp")
elseif(GAMMA_BUILD_VULKAN)
	file(GLOB Gamma_Renderer_SRC "src/Gamma/Renderer/Vulkan/*.h" "src/Gamma/Renderer/Vulkan/*.cpp")
elseif(GAMMA_BUILD_D3D9)
	file(GLOB Gamma_Renderer_SRC "src/Gamma/Renderer/Direct3D9/*.h" "src/Gamma/Renderer/Direct3D9/*.cpp")
elseif(GAMMA_BUILD_D3D10)
	file(GLOB Gamma_Renderer_SRC "src/Gamma/Renderer/Direct3D10/*.h" "src/Gamma/Renderer/Direct3D10/*.cpp")
elseif(GAMMA_BUILD_D3D11)
	file(GLOB Gamma_Renderer_SRC "src/Gamma/Renderer/Direct3D11/*.h" "src/Gamma/Renderer/Direct3D11/*.cpp")
elseif(GAMMA_BUILD_D3D12)
	file(GLOB Gamma_Renderer_SRC "src/Gamma/Renderer/Direct3D12/*.h" "src/Gamma/Renderer/Direct3D12/*.cpp")
endif(GAMMA_BUILD_OPENGL)
	
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

option(GAMMA_SHARED_LIBRARY "Build Gamma as a shared library" OFF)
if(GAMMA_SHARED_LIBRARY)
	add_library(Gamma SHARED ${Gamma_SRC} ${Gamma_Renderer_SRC})
	target_compile_definitions(Gamma PUBLIC GAMMA_SHARED_LIBRARY)
else(GAMMA_SHARED_LIBRARY)
	add_library(Gamma STATIC ${Gamma_SRC} ${Gamma_Renderer_SRC})
endif(GAMMA_SHARED_LIBRARY)


target_link_libraries(Gamma PRIVATE SDL2-static Krypton_STATIC)
target_include_directories(Gamma PUBLIC "${CMAKE_SOURCE_DIR}/vendor/SDL2/include" "${CMAKE_SOURCE_DIR}/vendor/Krypton/src" "${CMAKE_SOURCE_DIR}/vendor/GLEW/include" "${CMAKE_SOURCE_DIR}/vendor/portable-snippets")
target_compile_definitions(Gamma PUBLIC "-DGAMMA_BUILD=1")

target_compile_definitions(Gamma PUBLIC "$<$<CONFIG:DEBUG>:GAMMA_DEBUG>")

if(WIN32)
	target_compile_definitions(Gamma PUBLIC GAMMA_PLATFORM_WINDOWS)
	if(MSVC)
		set(GAMMA_DEBUG_LIBS "vcruntimed" "ucrtd" "msvcrtd")
		set(GAMMA_RELEASE_LIBS "vcruntime" "ucrt" "msvcrt")
		target_link_libraries(Gamma PRIVATE "$<$<CONFIG:DEBUG>:${GAMMA_DEBUG_LIBS}>")
		target_link_libraries(Gamma PRIVATE "$<$<CONFIG:RELWITHDEBINFO>:${GAMMA_DEBUG_LIBS}>")
		target_link_libraries(Gamma PRIVATE "$<$<CONFIG:RELEASE>:${GAMMA_RELEASE_LIBS}>")
		target_link_libraries(Gamma PRIVATE "$<$<CONFIG:MINSIZEREL>:${GAMMA_RELEASE_LIBS}>")
	endif(MSVC)
endif(WIN32)

if (UNIX)
    target_compile_definitions(Gamma PUBLIC GAMMA_PLATFORM_UNIX)
endif (UNIX)

if(MSVC)
	 target_compile_definitions(Gamma PUBLIC GAMMA_MSVC)
endif(MSVC)

if(GAMMA_BUILD_OPENGL)
	target_compile_definitions(Gamma PUBLIC "-DGAMMA_BUILD_OPENGL")
	if(WIN32)
		target_link_libraries(Gamma PRIVATE "opengl32")
	endif(WIN32)
	target_link_libraries(Gamma PRIVATE libglew_static)
	target_compile_definitions(Gamma PUBLIC "-DGLEW_STATIC=1")
elseif(GAMMA_BUILD_VULKAN)
	#Do nothing, for now
elseif(GAMMA_BUILD_D3D9)
	target_compile_definitions(Gamma PUBLIC "-DGAMMA_BUILD_D3D9")
	target_link_libraries(Gamma PRIVATE "d3d9")
	target_link_libraries(Gamma PRIVATE "dxgi" "d3dcompiler")
	target_compile_definitions(Gamma PUBLIC GAMMA_GRAPHICS_API_DIRECT_X GAMMA_GRAPHICS_API_DIRECT_X_9)
elseif(GAMMA_BUILD_D3D10)
	target_compile_definitions(Gamma PUBLIC "-DGAMMA_BUILD_D310")
	target_link_libraries(Gamma PRIVATE "d3d10")
	target_link_libraries(Gamma PRIVATE "dxgi" "d3dcompiler")
	target_compile_definitions(Gamma PUBLIC GAMMA_GRAPHICS_API_DIRECT_X GAMMA_GRAPHICS_API_DIRECT_X_10)
elseif(GAMMA_BUILD_D3D11)
	CreateGamma(${Gamma_SRC})
	target_compile_definitions(Gamma PUBLIC "-DGAMMA_BUILD_D3D11")
	target_link_libraries(Gamma PRIVATE "d3d11")
	target_link_libraries(Gamma PRIVATE "dxgi" "d3dcompiler")
	target_compile_definitions(Gamma PUBLIC GAMMA_GRAPHICS_API_DIRECT_X GAMMA_GRAPHICS_API_DIRECT_X_11)
elseif(GAMMA_BUILD_D3D12)
	target_compile_definitions(Gamma PUBLIC "-DGAMMA_BUILD_D3D12")
	target_link_libraries(Gamma PRIVATE "d3d12") 
	target_link_libraries(Gamma PRIVATE "dxgi" "d3dcompiler")
	target_compile_definitions(Gamma PUBLIC GAMMA_GRAPHICS_API_DIRECT_X GAMMA_GRAPHICS_API_DIRECT_X_12)
endif(GAMMA_BUILD_OPENGL)
