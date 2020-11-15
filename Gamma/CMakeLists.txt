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