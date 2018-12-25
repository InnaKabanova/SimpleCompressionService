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
    "src/sender.c"
    "src/config.c"
    )
target_link_libraries(
    ${PROJECT_NAME}
    pthread
    scscommon
    )
