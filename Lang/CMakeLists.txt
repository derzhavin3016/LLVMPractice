find_package(BISON 3.7 REQUIRED)
find_package(FLEX REQUIRED)
find_package(LLVM 14.0 REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

execute_process(COMMAND llvm-config-14 --libs all
                OUTPUT_VARIABLE LLVM_LIBS
                OUTPUT_STRIP_TRAILING_WHITESPACE)

add_executable(langI main.cc)

apply_compiler_flags(langI PUBLIC)
target_compile_features(langI PRIVATE cxx_std_20)

target_include_directories(langI SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS})
target_link_libraries(irgen PRIVATE ${LLVM_LIBS})

set_target_properties(langI
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
