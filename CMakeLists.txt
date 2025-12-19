cmake_minimum_required(VERSION 4.0)

project(click VERSION 1.0)
add_executable(click src/Engine.cpp src/main.cpp)
target_include_directories(click PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
find_package(SFML 3 REQUIRED Graphics Audio Window System)
target_include_directories(click PUBLIC ${SFML_INCLUDE_DIR})
target_link_libraries(click PUBLIC ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})