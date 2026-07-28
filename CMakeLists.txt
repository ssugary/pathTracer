cmake_minimum_required(VERSION 3.16)
project(PathTracer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native")

find_package(OpenMP REQUIRED)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Libs externas
set(LIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/lodepng/lodepng.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/lodepng/lodepng_util.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/tinyxml2/tinyxml2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/STBI/stb_image.c
)

add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    ${LIB_SOURCES}
)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libs
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra -Wpedantic 
)

# target_link_libraries(${PROJECT_NAME} PRIVATE 
#     OpenMP::OpenMP_CXX
# )