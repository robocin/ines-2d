cmake_minimum_required( VERSION 3.8 )
project(sim_2d_sm)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-pthread")

find_package(Boost COMPONENTS system filesystem REQUIRED)

file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "*.h" "*.hh" "*.h++" "*.hxx" "*.H" "*.hpp")
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "*.c" "*.cc" "*.c++" "*.cxx" "*.C" "*.cpp")

set( SOURCE_FILES src/main.cpp src/machines/kicker/kicker_stm.cpp src/machines/goalie/goalie_stm.cpp)

add_executable( ${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} )