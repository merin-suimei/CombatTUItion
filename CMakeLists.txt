cmake_minimum_required(VERSION 4.1)

project(CombatTUItion)

set(CMAKE_CXX_STANDARD 23)
set(source_dir "${PROJECT_SOURCE_DIR}/src/")

file(GLOB_RECURSE source_files "${source_dir}/*.cpp")

include_directories("include")

find_package(Curses REQUIRED)

add_executable(CombatTUItion ${source_files})
target_link_libraries(CombatTUItion ${NCURSES_LIBRARIES})
