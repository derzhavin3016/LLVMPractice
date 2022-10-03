find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

set(SRCS life.cc main.cc)

add_executable(Life ${SRCS})

set(GRAPH_SRCS graph.cc)
add_library(GraphLib STATIC ${GRAPH_SRCS})

target_link_libraries(GraphLib PRIVATE sfml-window sfml-system sfml-graphics)
target_compile_options(GraphLib PUBLIC -Wall -Wextra -Wpedantic -O2)
target_compile_features(GraphLib PUBLIC cxx_std_17)


target_link_libraries(Life PRIVATE GraphLib)
