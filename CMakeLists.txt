cmake_minimum_required(VERSION 3.16)

project(Life)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

set(SRCS "life.c" "main.c")
list(TRANSFORM SRCS PREPEND Life/)

add_executable(${CMAKE_PROJECT_NAME} ${SRCS})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -O2)
target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE c_std_11)
