cmake_minimum_required(VERSION 2.8)

project(utilities LANGUAGES C)
include_directories(
    "include/"
    )
add_library(
    ${PROJECT_NAME} SHARED
    "src/utilities.c"
    )
