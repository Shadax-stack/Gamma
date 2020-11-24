#ifndef GAMMA_MESH_H
#define GAMMA_MESH_H

#include "../Gamma.h"
#if defined(GAMMA_BUILD_OPENGL)
#include "OpenGL/Mesh.h"
#elif defined(GAMMA_BUILD_VULKAN)
#include "Vulkan/Mesh.h"
#elif defined(GAMMA_BUILD_D3D9)
#include "Direct3D9/Mesh.h"
#elif defined(GAMMA_BUILD_D3D10)
#include "Direct3D10/Mesh.h"
#elif defined(GAMMA_BUILD_D3D11)
#include "Direct3D11/Mesh.h"
#elif defined(GAMMA_BUILD_D3D12)
#include "Direct3D12/Mesh.h"
#else
#error Please select a graphics API in CMake
#endif

#endif // !GAMMA_MESH_H
