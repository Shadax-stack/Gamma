file(GLOB Demo_SRC
    "src/**.h"
    "src/**.cpp"
)

add_executable(Demo ${Demo_SRC})
target_link_libraries(Demo PRIVATE Gamma)
target_include_directories(Demo PRIVATE Gamma/)