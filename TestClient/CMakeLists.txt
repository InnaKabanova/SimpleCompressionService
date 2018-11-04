cmake_minimum_required(VERSION 2.8)

project(test_client LANGUAGES C)
include_directories(
    "../SimpleCompressionService/api/"
    "src/")
add_executable(
    ${PROJECT_NAME}
    "src/main.c")

# TODO: November 4
# Receive & parse service IP addr and port as input parameters
# Connect to the service
# Send several simple requests (VALID ones)
# Send several simple requests (INVALID ones)

# TODO: General
# 1. Option to read compression data from a file; split that data into
# packages; send it to service; receive the compressed data back as a
# response; assemble and deserialize the data back to its initial state.
# 2. Option to bombard the service with requests (load testing).
# 3. A script to spawn multiple TestClient processes to load test the service.

