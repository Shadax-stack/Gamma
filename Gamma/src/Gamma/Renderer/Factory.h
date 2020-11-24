#ifndef GAMMA_MESH_H
#define GAMMA_MESH_H

#include "../Gamma.h"
#if defined(GAMMA_BUILD_OPENGL)
#include "OpenGL/Factory.h"
#elif defined(GAMMA_BUILD_VULKAN)
#include "Vulkan/Factory.h"
#elif defined(GAMMA_BUILD_D3D9)
#include "Direct3D9/Factory.h"
#elif defined(GAMMA_BUILD_D3D10)
#include "Direct3D10/Factory.h"
#elif defined(GAMMA_BUILD_D3D11)
#include "Direct3D11/Factory.h"
#elif defined(GAMMA_BUILD_D3D12)
#include "Direct3D12/Factory.h"
#else
#error Please select a graphics API in CMake
#endif

#endif // !GAMMA_MESH_H
