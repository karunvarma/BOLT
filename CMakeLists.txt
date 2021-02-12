# CMakeLists.txt for Uri
#
# © 2018 by Richard Walters

cmake_minimum_required(VERSION 3.8)
set(This Uri)

set(Headers
    include/Uri/Uri.hpp
)

set(Sources
    src/Uri.cpp
)

# creating a static lib from soucrs and header files
add_library(${This} STATIC ${Sources} ${Headers})


set_target_properties(${This} PROPERTIES
    FOLDER Libraries
)

# to determin header files
target_include_directories(${This} PUBLIC include)


add_subdirectory(googletest)
add_subdirectory(test)
