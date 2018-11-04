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

# -----------------------------------------------------------------
# TODO -- November 4
# Code review
# Write test client
# Write requests parsing & error handling

# -----------------------------------------------------------------
# TODO -- General
# Queue of internal requests
# Signal handling (SIGINT). Cleanup when SIGKILL, SIGSTOP.

# -----------------------------------------------------------------
# TODO: avoid spurious wakeups
# TODO: pthread_cleanup_push for each worker thread
# TODO: disable default reaction to SIGINT
# TODO: cleanup function call as a reaction to SIGKILL, SIGSTOP
