cmake_minimum_required(VERSION 3.10)
project(software_render LANGUAGES CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL REQUIRED sdl3 sdl3-ttf)

add_executable(render render.cpp)
target_link_libraries(render PRIVATE ${SDL_LIBRARIES})
