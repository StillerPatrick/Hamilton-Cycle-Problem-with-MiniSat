cmake_minimum_required(VERSION 2.8)
project( SATSolver )

#
set(MINISAT_DIR
    "/home/richard/Projekte/Hamilton-Kreis/minisat"
)
set(EXECUTABLE_OUTPUT_PATH build)
set(GCC_COVERAGE_COMPILE_FLAGS "-std=c++11 -O3")
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

include_directories("minisat")

# For YouCompleteMe autocompleter
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# App
add_definitions(-D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -g)

add_executable( SATSolver
  src/main.cpp
)

target_include_directories( SATSolver
  PRIVATE .
          ${MINISAT_DIR}
)

target_link_libraries( SATSolver
    ${MINISAT_DIR}/build/release/lib/libminisat.a
)
