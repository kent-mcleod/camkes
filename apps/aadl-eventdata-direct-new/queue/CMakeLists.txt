
cmake_minimum_required(VERSION 3.7.2)

project(queue C)

add_library(queue EXCLUDE_FROM_ALL src/queue.c)
target_include_directories(queue PUBLIC include)
target_link_libraries(queue muslc)
