#ifndef GAMMA_INCLUDE_RENDERER_H
#define GAMMA_INCLUDE_RENDERER_H

#include "../Gamma.h"
#if defined(GAMMA_BUILD_OPENGL)
#include "OpenGL/Renderer.h"
#elif defined(GAMMA_BUILD_VULKAN)
#include "Vulkan/Renderer.h"
#elif defined(GAMMA_BUILD_D3D9)
#include "Direct3D9/Renderer.h"
#elif defined(GAMMA_BUILD_D3D10)
#include "Direct3D10/Renderer.h"
#elif defined(GAMMA_BUILD_D3D11)
#include "Direct3D11/Renderer.h"
#elif defined(GAMMA_BUILD_D3D12)
#include "Direct3D12/Renderer.h"
#else
#error Please select a graphics API in CMake
#endif

#endif
