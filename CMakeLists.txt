cmake_minimum_required(VERSION 3.16)

project(LLVMPractive)

include(cmake/comp_flags.cmake)

add_subdirectory(Life)
add_subdirectory(IRGen)
add_subdirectory(Lang)
