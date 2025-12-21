cmake_minimum_required(VERSION 4.0)

project(click VERSION 1.0)
find_package(SFML 3 COMPONENTS Graphics Audio Window System REQUIRED)

set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/output) #set location where final binary and resources are located
set(RESOURCE_FILES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data) 

add_executable(click src/main.cpp src/Engine.cpp)

set_target_properties(click PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
)

file(COPY ${RESOURCE_FILES_DIR} DESTINATION ${OUTPUT_DIR})

target_include_directories(click PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(click PUBLIC SFML::Graphics SFML::Window SFML::System)