cmake_minimum_required(VERSION 3.16)

project(Life)

find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

set(SRCS life.cc main.cc)
list(TRANSFORM SRCS PREPEND Life/)

add_executable(${CMAKE_PROJECT_NAME} ${SRCS})

set(GRAPH_SRCS Life/graph.cc)
add_library(GraphLib STATIC ${GRAPH_SRCS})

target_link_libraries(GraphLib PRIVATE sfml-window sfml-system sfml-graphics)
target_compile_options(GraphLib PUBLIC -Wall -Wextra -Wpedantic -O2)
target_compile_features(GraphLib PUBLIC cxx_std_17)


target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE GraphLib)
