cmake_minimum_required(VERSION 2.8)

project(SimpleCompressionService LANGUAGES C)
include_directories(include/)
add_executable(${PROJECT_NAME} "src/main.c")

# TODO
# 1. Parsing the input parameters
# 2. Server initialization
# 2. Pool of worker threads
# 3. Boss thread -- does the listening and puts the requests into requests queue
# 4. Requests queue
# 4. Responses queue
# 5. Worker thread -- waits for requests queue to be non-empty
# (CAUTION: avoid spurious wakeups here) and processes one request at a time
# with help of the following functions:
# -- parse request packet
# -- do work: Ping, Get Stats, Reset Stats, Compress
# -- make a resulting response package
# -- put the package into responses queue
# 6. Sender thread -- waits for responses queue to be non-empty, sends the responses
# back to clients.
