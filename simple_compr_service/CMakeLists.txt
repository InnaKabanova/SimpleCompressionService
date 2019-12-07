cmake_minimum_required(VERSION 2.8)

project(simple_compr_service LANGUAGES C)
include_directories(
    "../lib_common/include/"
    "src/")
link_directories(
    "../../lib_common/build/"
    )
add_compile_options(
    "-Wall"
)
add_executable(
    ${PROJECT_NAME}
    "src/main.c"
    "src/queue.c"
    "src/requests_queue.c"
    "src/responses_queue.c"
    "src/processor.c"
    "src/receiver.c"
    "src/sender.c"
    "src/logging_utilities.c"
    )
target_link_libraries(
    ${PROJECT_NAME}
    pthread
    scscommon
    )
