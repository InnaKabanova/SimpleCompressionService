cmake_minimum_required(VERSION 2.8)

project(test_client LANGUAGES C)
include_directories(
    "../lib_common/include/"
    "src/")
link_directories(
    "../../lib_common/build/"
    )
add_executable(
    ${PROJECT_NAME}
    "src/main.c"
    "src/networking.c"
    )
target_link_libraries(
    ${PROJECT_NAME}
    scscommon
    )
