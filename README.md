# Gamma
A C++ Game/Rendering engine
## Getting Started
First clone the repo into a directory. Then, in that directory, run cmake. Here are the options that Gamma provides:
- `GAMMA_SHARED_LIBRARY`: Build Gamma as a shared library. Turned off by default
- `GAMMA_BUILD_EXAMPLES`: Build the example projects. Off by default.
- `GAMMA_GRAPHICS_API_D3D12`: Build Gamma using the D3D12 API. Will be on by default on Windows 10 or 8/8.1
- `GAMMA_GRAPHICS_API_VULKAN`: Build Gamma using the Vulkan API. Will be off by default on platforms in which GAMMA_GRAPHICS_API_D3D12 is turned on by default
## Supported Graphics APIs
Currently there are not any, check the ogl-d3d9-10-11 branch for legacy API support. The engine is transitioning into an API that supports D3D12 and Vulkan. Support for legacy APIs may be added later on top of D3D12 and Vulkan.
Future API support roadmap:
- D3D11 for better Windows 7/Vista support
- OpenGL 3-4 for better support on Mac OS 
- OpenGL 2 and D3D9 for better support on extremely old GPUs
- D3D10 because why not? (This will be added in the far future since D3D10 is a dead API and pretty much no reason to support it besides "just because we can")
## TODO list
Anything graphics API-specific is added to D3D12 before Vulkan and will be done in an abstract manner
- Initialize a device and add screen clearing
- Render a triangle
- Render a cube 
- Basic transformations and a camera 
- Texture rendering (texture loading will be done in SOIL2)
- Object loading (object loading will be done with ASSIMP)
- Scene system and scene loading
- A lighting system
- Normal mapping
- Skybox rendering
- Shadow mapping
- Physically based rendering, using a multiscattering BRDF
- IBL
### Long term goals
- VCT (LPV will not be used because it provides diffuse only global illumination and there are many issues with it anyway)
- Path tracing (Not using Nvidia raytracing extensions since I want anyone to be able to run it, especially me since I do not have an RTX card)
- Support other APIs besides D3D12 and Vulkan

