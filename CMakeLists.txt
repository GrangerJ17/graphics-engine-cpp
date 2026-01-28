cmake_minimum_required(VERSION 3.20)

project(cppgraphics
    VERSION 0.1
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MACOSX_RPATH ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL3 REQUIRED sdl3 sdl3-ttf)

add_executable(render
    ../render.cpp
)

target_include_directories(render PRIVATE ${SDL3_INCLUDE_DIRS})
target_link_directories(render PRIVATE ${SDL3_LIBRARY_DIRS})
target_link_libraries(render PRIVATE ${SDL3_LIBRARIES})

set_target_properties(render PROPERTIES
    BUILD_RPATH "${SDL3_LIBRARY_DIRS}"
)
