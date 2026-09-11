cmake_minimum_required(VERSION 3.21)
project(APP)

file(GLOB sources src/*.cpp)
file(GLOB headers include/*.hpp)

include_directories(include/)

add_executable(${PROJECT_NAME} ${sources} ${headers})

# Povezivanje sa engine-om i dodavanje include putanja
target_link_libraries(${PROJECT_NAME} PRIVATE matf-rg-engine)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../engine/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../engine/libs/glad/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../engine/libs/stb/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../engine/libs/glm
        ${CMAKE_CURRENT_SOURCE_DIR}/../engine/libs/spdlog/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../engine/libs/imgui/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../engine/libs/json/include
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
