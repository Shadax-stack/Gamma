set(SDL_SHARED OFF CACHE BOOL "" FORCE)
add_subdirectory(SDL2)
set_target_properties(SDL2main PROPERTIES FOLDER vendor/SDL2)
set_target_properties(SDL2-static PROPERTIES FOLDER vendor/SDL2)

add_subdirectory(spdlog)
set_target_properties(spdlog PROPERTIES FOLDER vendor)