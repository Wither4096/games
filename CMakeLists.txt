cmake_minimum_required(VERSION 4.0)

project(click VERSION 1.0)
find_package(SFML 3 COMPONENTS Graphics Audio Window System REQUIRED)

add_executable(click src/main.cpp src/Engine.cpp)

target_include_directories(click PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(click PUBLIC SFML::Graphics SFML::Window SFML::System)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})