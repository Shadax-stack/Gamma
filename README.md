# Gamma
A C++ Game/Rendering engine
## Getting Started
First clone the repo into a directory
Then open CMake, and use the directory you just cloned as the binary and source directory. Turn on GAMMA_SHARED_LIBRARY to build Gamma as a shared library. Select a graphics API as well. GAMMA_BUILD_OPENGL is used by default. Make sure only one graphics API is selected. Then click "generate" and select your platform. 
## Supported Graphics APIs
- OpenGL 
- Direct3D 9
- Direct3D 10
- Direct3D 11
## TODO list
- Add basic vertex/index buffers to OpenGL and D3D9
- Add Direct3D 12 support
- Add vulkan support
- Add basic vertex/index buffers to D3D10, D3D11, D3D12, and Vulkan
- Add 2D textures
- Add transforms and camera
- Add a lighting system
- Transition to PBR
- Create a scene system
