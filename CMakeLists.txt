cmake_minimum_required(VERSION 3.16)
project(Chippu_8u)

set(CMAKE_CXX_STANDARD 20)

#   Adds our includes to the project so we can find the files needed
include_directories(includes)

#   Setting up a library to link into our executable
#message(STATUS "####        Adding Static libs subdir       ####")
#add_subdirectory(lib)

message(STATUS "####        Building project executable     ####")
#   Sets up the main app instance with all the source files to be included
#   differing source files would be good for different OS' as they could all use the same includes
#   but different source defs
add_executable(Chippu_8u
			   includes/ChipCPU.hpp
			   source/ChipCPU.cpp
			   includes/Gfx.hpp
			   source/Gfx.cpp
			   includes/Input.hpp
			   source/Input.cpp
			   main.cpp
        )

#   Linking the lib to the executable. Notice how the source files aren't in exe but can still be used
#message(STATUS "####        Linking libs                    ####")
#target_link_libraries(Chippu_8u PRIVATE calc_lib)







