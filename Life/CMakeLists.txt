find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

add_library(GraphLib STATIC graph.cc)

target_link_libraries(GraphLib PRIVATE sfml-window sfml-system sfml-graphics)
target_compile_options(GraphLib PUBLIC -Wall -Wextra -Wpedantic -O2)
target_compile_features(GraphLib PRIVATE cxx_std_17)

add_executable(Life main.c)
target_link_libraries(Life PRIVATE GraphLib)
target_compile_features(Life PRIVATE c_std_11)

set_target_properties(Life
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
