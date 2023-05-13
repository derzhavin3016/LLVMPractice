cmake_minimum_required(VERSION 3.16)

project(LLVMPractive)

option(GLANG_ONLY_LIB "Build glang library only" OFF)

include(cmake/comp_flags.cmake)

add_subdirectory(Life)

if(NOT GLANG_ONLY_LIB)
  add_subdirectory(IRGen)
  add_subdirectory(Lang)
endif()
