find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

add_library(GraphLib STATIC graph.cc)

target_link_libraries(GraphLib PRIVATE sfml-window sfml-system sfml-graphics)
target_compile_options(GraphLib PUBLIC -Wall -Wextra -Wpedantic -O2)
target_compile_features(GraphLib PRIVATE cxx_std_17)

add_executable(life main.c)
target_link_libraries(life PRIVATE GraphLib)
target_compile_features(life PRIVATE c_std_11)

set_target_properties(life
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
