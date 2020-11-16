file(GLOB Demo_SRC
    "src/*.h"
    "src/*.cpp"
)

add_executable(Demo ${Demo_SRC})
target_link_libraries(Demo PRIVATE Gamma)
target_include_directories(Demo PRIVATE ${CMAKE_SOURCE_DIR}/Gamma/src)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Demo)