cmake_minimum_required(VERSION 3.8)

if(WIN32)
	option(GAMMA_GRAPHICS_API_DIRECT3D12 "Use the D3D12 API for renderering" ON)
	option(GAMMA_GRAPHICS_API_VULKAN "Use the Vulkan API for renderering" OFF)
else(WIN32)
	option(GAMMA_GRAPHICS_API_DIRECT3D12 "Use the D3D12 API for renderering" OFF)
	option(GAMMA_GRAPHICS_API_VULKAN "Use the Vulkan API for renderering" ON)
endif(WIN32)

if(GAMMA_GRAPHICS_API_VULKAN)
	file(GLOB Gamma_Renderer_SRC "src/Gamma/Renderer/Vulkan/*.h" "src/Gamma/Renderer/Vulkan/*.cpp")
	find_package(Vulkan REQUIRED)
elseif(GAMMA_GRAPHICS_API_DIRECT3D12)
	file(GLOB Gamma_Renderer_SRC "src/Gamma/Renderer/Direct3D12/*.h" "src/Gamma/Renderer/Direct3D12/*.cpp" "src/Gamma/Renderer/DirectXGraphicsInfrastructure/*.h" "src/Gamma/Renderer/DirectXGraphicsInfrastructure/*.cpp") 
endif(GAMMA_GRAPHICS_API_VULKAN)

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

if(WIN32)
	file(GLOB_RECURSE Gamma_Platform_SRC "src/Gamma/Win32/**.h" "src/Gamma/Win32/**.cpp")
endif(WIN32)

option(GAMMA_SHARED_LIBRARY "Build Gamma as a shared library" OFF)
if(GAMMA_SHARED_LIBRARY)
	add_library(Gamma SHARED ${Gamma_SRC} ${Gamma_Renderer_SRC} ${Gamma_Platform_SRC})
	target_compile_definitions(Gamma PUBLIC GAMMA_SHARED_LIBRARY)
else(GAMMA_SHARED_LIBRARY)
	add_library(Gamma STATIC ${Gamma_SRC} ${Gamma_Renderer_SRC} ${Gamma_Platform_SRC})
endif(GAMMA_SHARED_LIBRARY)


target_link_libraries(Gamma PRIVATE SDL2-static Krypton_STATIC)
target_include_directories(Gamma PUBLIC "${CMAKE_SOURCE_DIR}/vendor/SDL2/include" "${CMAKE_SOURCE_DIR}/vendor/Krypton/src" "${CMAKE_SOURCE_DIR}/vendor/portable-snippets")
target_compile_definitions(Gamma PUBLIC "-DGAMMA_BUILD=1")

target_compile_definitions(Gamma PUBLIC "$<$<CONFIG:DEBUG>:GAMMA_DEBUG>")

if(WIN32)
	target_compile_definitions(Gamma PUBLIC GAMMA_PLATFORM_WINDOWS)
endif(WIN32)

if (UNIX)
    target_compile_definitions(Gamma PUBLIC GAMMA_PLATFORM_UNIX)
endif (UNIX)

if(MSVC)
	target_compile_definitions(Gamma PUBLIC GAMMA_MSVC)
	set(GAMMA_DEBUG_LIBS "vcruntimed" "ucrtd" "msvcrtd")
	set(GAMMA_RELEASE_LIBS "vcruntime" "ucrt" "msvcrt")
	target_link_libraries(Gamma PRIVATE "$<$<CONFIG:DEBUG>:${GAMMA_DEBUG_LIBS}>")
	target_link_libraries(Gamma PRIVATE "$<$<CONFIG:RELWITHDEBINFO>:${GAMMA_DEBUG_LIBS}>")
	target_link_libraries(Gamma PRIVATE "$<$<CONFIG:RELEASE>:${GAMMA_RELEASE_LIBS}>")
	target_link_libraries(Gamma PRIVATE "$<$<CONFIG:MINSIZEREL>:${GAMMA_RELEASE_LIBS}>")
endif(MSVC)

if(GAMMA_GRAPHICS_API_VULKAN)
	target_compile_definitions(Gamma PUBLIC "GAMMA_GRAPHICS_API_VULKAN")
	if(UNIX)
		find_package(X11 REQUIRED)
		link_libraries(${X11_LIBRARIES})
		add_definitions(-DVK_USE_PLATFORM_XLIB_KHR)
	endif(UNIX)
	target_link_libraries(Gamma PUBLIC Vulkan::Vulkan)
elseif(GAMMA_GRAPHICS_API_DIRECT3D12)
	target_link_libraries(Gamma PRIVATE "d3d12" "dxgi" "d3dcompiler")
	target_compile_definitions(Gamma PUBLIC "GAMMA_GRAPHICS_API_DIRECT3D12")
endif(GAMMA_GRAPHICS_API_VULKAN)

# Disable warnings that nobody cares about
if(MSVC)
	add_compile_options(/w26812 /w6385 /w26451 /w4996)
	target_compile_definitions(Gamma PUBLIC /wd26812 /wd6385 /wd26451 /wd4996)
endif(MSVC)
