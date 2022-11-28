find_package(BISON 3.7 REQUIRED)
find_package(FLEX REQUIRED)
find_package(LLVM 14.0 REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(GRAMMAR_PATH "${CMAKE_CURRENT_SOURCE_DIR}/grammar")
set(DRIVER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/driver")
set(AST_PATH "${CMAKE_CURRENT_SOURCE_DIR}/ast")
set(STD_PATH "${CMAKE_CURRENT_SOURCE_DIR}/std")

bison_target(parser grammar/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cc
             DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.hh)

message(STATUS "bison output: ${BISON_parser_OUTPUTS}")

flex_target(lexer grammar/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc)
message(STATUS "flex output: ${FLEX_lexer_OUTPUTS}")

add_flex_bison_dependency(lexer parser)

execute_process(
  COMMAND llvm-config-14 --libs all
  OUTPUT_VARIABLE LLVM_LIBS
  OUTPUT_STRIP_TRAILING_WHITESPACE)

add_library(lex ${FLEX_lexer_OUTPUTS})

add_library(par ${CMAKE_CURRENT_BINARY_DIR}/parser.cc)
target_include_directories(lex PUBLIC ${AST_PATH} ${STD_PATH})

target_include_directories(
  par
  PUBLIC ${DRIVER_PATH} ${STD_PATH}
  PUBLIC ${AST_PATH} SYSTEM
  PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
  PUBLIC ${CMAKE_SOURCE_DIR})

add_executable(langI main.cc ${DRIVER_PATH}/driver.cc ${AST_PATH}/ast.cc)

apply_compiler_flags(langI PUBLIC)
target_compile_features(langI PRIVATE cxx_std_20)

target_include_directories(langI SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS}
                                                ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(langI PUBLIC ${LLVM_LIBS} lex par)

set_target_properties(langI PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                       "${CMAKE_BINARY_DIR}/bin")
