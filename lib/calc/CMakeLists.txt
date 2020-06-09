#   Sets up the library under the alias calc_lib
add_library(
        calc_lib
        STATIC
        src/calc/calc.h
        src/calc/calc.cpp
)

#   Give a link to where to find the header files, PUBLIC means everything that uses this lib NEEDS that include
target_include_directories(calc_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

#   Preprocessor definitions can be queried elsewhere, check main.cpp
#   main.cpp can use it and anyone who links to it cause its public, PRIVATE means it stops just in the lib and doesnt propagate to linked files
#   INTERFACE is the opposite of PRIVATE, meaning everything but the lib itself gets access
target_compile_definitions(calc_lib PUBLIC CALC_LIB_VERSION=3)
target_compile_definitions(calc_lib PUBLIC CALC_LIB_PREPROCESSOR_WORKING=1)
