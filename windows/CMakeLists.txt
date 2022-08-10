cmake_minimum_required(VERSION 3.20)
project (3dViewer)

SET(CMAKE_BUILD_TYPE "Release")

add_subdirectory(../base ./build_release/base)
add_subdirectory(../base/vmath ./build_release/vmath)

aux_source_directory(. SORCES)

add_executable(${PROJECT_NAME} ${SORCES})

target_link_libraries(${PROJECT_NAME} base vmath)
