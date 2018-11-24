cmake_minimum_required(VERSION 2.8)

project(test_client LANGUAGES C)
include_directories(
    "../lib_utilities/"
    "../simple_compr_service/api/"
    "src/")
link_directories(
    "../../lib_utilities/build/"
    )
add_executable(
    ${PROJECT_NAME}
    "src/main.c"
    )
target_link_libraries(
    ${PROJECT_NAME}
    utilities
    )

