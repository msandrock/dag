include_directories(${DAG_SOURCE_DIR}/src)

add_executable(dag_test test.cpp)

target_link_libraries (dag_test
                       dagdep
                       )

add_test (NAME DagTest COMMAND dag_test)