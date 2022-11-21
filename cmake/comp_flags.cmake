set(SANITIZERS
-fsanitize=address
-fsanitize=alignment
-fsanitize=bool
-fsanitize=bounds
-fsanitize=enum
-fsanitize=float-cast-overflow
-fsanitize=float-divide-by-zero
-fsanitize=integer-divide-by-zero
-fsanitize=leak
-fsanitize=nonnull-attribute
-fsanitize=null
# -fsanitize=object-size
-fsanitize=return
-fsanitize=returns-nonnull-attribute
-fsanitize=shift
-fsanitize=signed-integer-overflow
-fsanitize=undefined
-fno-sanitize-recover
-fsanitize=unreachable
-fsanitize=vla-bound
-fsanitize=vptr
-fsized-deallocation
)

set(COMMON_WARNINGS
-Wall
-Wextra
-Wpedantic
-Wno-gnu-zero-variadic-macro-arguments
)

set(GCC_WARNINGS
-Weffc++
-Waggressive-loop-optimizations
-Wc++0x-compat
-Wc++11-compat
-Wc++14-compat
-Wcast-align
-Wcast-qual
-Wchar-subscripts
-Wconditionally-supported
-Wconversion
-Wctor-dtor-privacy
-Wempty-body
-Wfloat-equal
-Wformat-nonliteral
-Wformat-security
-Wformat-signedness
-Wformat=2
-Winline
# -Wlarger-than=8192
-Wlogical-op
# -Wmissing-declarations
-Wnon-virtual-dtor
-Wopenmp-simd
-Woverloaded-virtual
-Wpacked
-Wpointer-arith
-Wredundant-decls
-Wshadow
-Wsign-conversion
-Wsign-promo
-Wstack-usage=8192
-Wstrict-null-sentinel
-Wstrict-overflow=2
-Wsuggest-attribute=noreturn
-Wsuggest-final-methods
-Wsuggest-final-types
-Wsuggest-override
-Wswitch-default
-Wswitch-enum
-Wsync-nand
-Wundef
-Wunreachable-code
-Wunused
-Wuseless-cast
-Wvariadic-macros
-Wno-literal-suffix
-Wno-missing-field-initializers
-Wno-narrowing
-Wno-old-style-cast
-Wno-varargs
-fcheck-new
-fstack-protector
-fstrict-overflow
-flto-odr-type-merging
-fno-omit-frame-pointer
)

function(apply_compiler_flags TARGET VISIBILIY)
  # Add sanitizers
  target_link_options(${TARGET} ${VISIBILIY} "$<$<CONFIG:Debug>:${SANITIZERS}>")
  target_compile_options(${TARGET} ${VISIBILIY} "$<$<CONFIG:Debug>:${SANITIZERS}>")

  # Compile stuff
  target_compile_options(${TARGET} ${VISIBILIY} "$<$<CONFIG:Debug>:${COMMON_WARNINGS}>")
  target_compile_options(${TARGET} ${VISIBILIY} "$<$<CXX_COMPILER_ID:GNU>:$<$<CONFIG:Debug>:${GCC_WARNINGS}>>")
  target_compile_options(${TARGET} ${VISIBILIY} "$<$<CONFIG:Release>:-O2")
endfunction()

string(REPLACE " " ";" DED_SAN_LST "${SANITIZERS}")
string(REPLACE " " ";" DED_GCC_WARNS_LST "${COMMON_WARNINGS}")
string(REPLACE " " ";" DED_GCC_WARNS_LST "${GCC_WARNINGS}")
