project(02_Translation C)
cmake_minimum_required(VERSION 2.8)
add_executable(${PROJECT_NAME} main.c ../rei/rei.c)
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/../glfw/lib/libglfw.so.3 -lGL)
