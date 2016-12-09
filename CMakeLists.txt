cmake_minimum_required(VERSION 2.8)

project(papipp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++14 -g -Wall -Wextra")

add_subdirectory(tests)

install(DIRECTORY DESTINATION include)
install(FILES "papipp.h" DESTINATION include)

