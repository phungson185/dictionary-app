cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(dict)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-Wall")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

add_definitions(${GTK3_CFLAGS_OTHER} -rdynamic)

include_directories(${GTK3_INCLUDE_DIRS} include btree-lib/inc)
link_directories(${PROJECT_SOURCE_DIR}/btree-lib/)

file(GLOB SOURCES "src/*.c")

add_executable(init src/init/init.c)
target_link_libraries(init bt)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${GTK3_LIBRARIES} bt)