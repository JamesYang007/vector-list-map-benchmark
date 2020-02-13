cmake_minimum_required (VERSION 3.1)
project (VLTest)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -Wextra -Werror")

add_subdirectory(src)
