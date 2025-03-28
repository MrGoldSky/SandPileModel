add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} parser getdata sandpile imagegenerator)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})