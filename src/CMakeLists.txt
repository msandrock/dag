add_library (dagdep stdafx.hpp dag.cpp dag.hpp svg.cpp svg.hpp)
add_executable (dag main.cpp)
target_link_libraries (dag dagdep)