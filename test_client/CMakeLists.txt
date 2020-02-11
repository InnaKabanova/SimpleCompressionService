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
    "src/connection.c"
    "src/workers_manager.c"
    "src/sender.c"
    "src/receiver.c"
    "src/requests_import.c"
    "src/requests_generation.c"
    "src/internal_request.c"
    "src/uuid.c"
    )
add_compile_options(
    "-O0"
    "-Wall"
    "-Wextra"
    )
target_link_libraries(
    ${PROJECT_NAME}
    pthread
    scscommon
    )
