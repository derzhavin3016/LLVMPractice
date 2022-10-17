find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

llvm_map_components_to_libnames(LLVM_LIBS core)

add_executable(irgen main.cc gen.cc)

target_compile_options(irgen PUBLIC -Wall -Wextra -Wpedantic -Wno-unused-parameter -O2)
target_compile_features(irgen PRIVATE cxx_std_20)

target_include_directories(irgen PRIVATE ${LLVM_INCLUDE_DIRS})
target_link_libraries(irgen PRIVATE ${LLVM_LIBS})
target_compile_definitions(irgen PRIVATE ${LLVM_DEFINITIONS})

set_target_properties(irgen
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
