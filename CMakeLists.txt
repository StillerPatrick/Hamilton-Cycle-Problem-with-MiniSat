cmake_minimum_required(VERSION 2.8)
project( SATSolver )

#
set(MINISAT_DIR
    "/home/richard/Projekte/Hamilton-Kreis/minisat"
)
set(EXECUTABLE_OUTPUT_PATH build)

include_directories("minisat")

add_definitions(-std=c++11)

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
