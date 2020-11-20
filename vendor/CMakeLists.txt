set(SDL_SHARED OFF CACHE BOOL "" FORCE)
add_subdirectory(SDL2)
set_target_properties(SDL2main PROPERTIES FOLDER vendor/SDL2)
set_target_properties(SDL2-static PROPERTIES FOLDER vendor/SDL2)

add_subdirectory(Krypton)
set_target_properties(Krypton_STATIC PROPERTIES FOLDER vendor/Krypton)
set_target_properties(Krypton_SHARED PROPERTIES FOLDER vendor/Krypton)

add_subdirectory(GLEW/build/cmake)
set_target_properties(libglew_static PROPERTIES FOLDER vendor/GLEW)
set_target_properties(libglew_shared PROPERTIES FOLDER vendor/GLEW)
target_compile_definitions(libglew_static PUBLIC "GLEW_BUILD")
target_compile_definitions(libglew_shared PUBLIC "GLEW_BUILD")