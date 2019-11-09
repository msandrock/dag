cmake_minimum_required(VERSION 3.14.5)
project (dag)

add_definitions(-DVERSION="1.1.3")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory (src) 

enable_testing()
add_subdirectory (test)
