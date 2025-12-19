cmake_minimum_required(VERSION 4.0)

project(click VERSION 1.0)
find_package(SFML 3 COMPONENTS Graphics Audio Window System REQUIRED)

set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/output) #set location where final binary and resources are located
set(RESOURCE_FILES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data) 
file(MAKE_DIRECTORY ${OUTPUT_DIR}) #makedir

add_executable(click src/main.cpp src/Engine.cpp)

set_target_properties(click PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
)
set(RESOURCE_FILES
    ${RESOURCE_FILES_DIR}/fonts
)

add_custom_command(TARGET click POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${RESOURCE_FILES}
        ${OUTPUT_DIR}
)

target_include_directories(click PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(click PUBLIC SFML::Graphics SFML::Window SFML::System)
#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})