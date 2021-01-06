# CMakeList.txt : CMake project for lower-bound-inserter, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("lower-bound-inserter")

# Add source to this project's executable.
add_executable (lower-bound-inserter "main.cpp" "lower_bound_inserter.h")

# TODO: Add tests and install targets if needed.
