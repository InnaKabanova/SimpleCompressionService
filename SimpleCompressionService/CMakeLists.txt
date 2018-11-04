cmake_minimum_required(VERSION 2.8)

project(simple_compr_service LANGUAGES C)
include_directories(
    "api/"
    "src/")
add_executable(
    ${PROJECT_NAME}
    "src/main.c"
    "src/utilities.c"
    "src/server.c"
    "src/requests_queue.c"
    "src/processor.c")
target_link_libraries(
    ${PROJECT_NAME}
    pthread)
