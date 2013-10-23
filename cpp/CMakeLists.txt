cmake_minimum_required (VERSION 2.6) 
project (PLOT) 

find_library(LIBSERIAL NAMES serial)
find_library(LIBSERIALBUF NAMES serial)

add_executable (plot main.cpp)
target_link_libraries(plot ${LIBSERIAL}) 
