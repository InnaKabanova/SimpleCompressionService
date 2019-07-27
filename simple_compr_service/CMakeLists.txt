cmake_minimum_required(VERSION 2.8)

project(simple_compr_service LANGUAGES C)
include_directories(
    "../lib_common/include/"
    "src/")
link_directories(
    "../../lib_common/build/"
    )
add_executable(
    ${PROJECT_NAME}
    "src/main.c"
    "src/server.c"
    "src/requests_queue.c"
    "src/acceptor.c"
    "src/processor.c"
    "src/sender.c"
    )
target_link_libraries(
    ${PROJECT_NAME}
    pthread
    scscommon
    )

