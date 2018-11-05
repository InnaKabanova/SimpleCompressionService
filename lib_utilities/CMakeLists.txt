cmake_minimum_required(VERSION 2.8)

project(utilities LANGUAGES C)
add_library(
    ${PROJECT_NAME} SHARED
    "utilities.c"
    )

