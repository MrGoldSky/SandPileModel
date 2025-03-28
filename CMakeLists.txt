cmake_minimum_required(VERSION 3.5.0)

project(
    SandPileModel
    VERSION 1.0
    DESCRIPTION "This project implements a simplified version of the [Abelian sandpile model](https://en.wikipedia.org/wiki/Abelian_sandpile_model). It simulates the dynamics of a sandpile based on an initial configuration provided in a TSV file and generates images in BMP format to visually represent the state of the model over time."
    LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

configure_file(${CMAKE_SOURCE_DIR}/data/input.tsv ${CMAKE_BINARY_DIR}/data/input.tsv COPYONLY)

add_subdirectory(lib)
add_subdirectory(bin)
