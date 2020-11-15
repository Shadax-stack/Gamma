file(GLOB Gamma_SRC
    "**.h"
    "**.cpp"
)

add_library(Gamma SHARED ${Gamma_SRC})

target_link_libraries(Gamma PRIVATE SDL2-static)
target_include_directories(Gamma PUBLIC vendor/SDL/include)