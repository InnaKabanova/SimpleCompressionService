cmake_minimum_required(VERSION 2.8)

project(simple_compr_service LANGUAGES C)
include_directories(
    "../lib_utilities/"
    "api/"
    "src/")
link_directories(
    "../../lib_utilities/build/"
    )
add_executable(
    ${PROJECT_NAME}
    "src/main.c"
    "src/server.c"
    "src/requests_queue.c"
    "src/worker.c")
target_link_libraries(
    ${PROJECT_NAME}
    pthread
    utilities
    )

