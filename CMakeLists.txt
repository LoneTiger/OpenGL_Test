# CMakeList.txt : CMake project for OpenGL_Test, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("OpenGL_Test")

# SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} glad)

# Output dir
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Add source to this project's executable.
add_executable (OpenGL_Test OpenGL_Test.cpp glad.c "shader.h" "shader.cpp" "stb_image.cpp")
target_link_libraries(OpenGL_Test ${SDL2_LIBRARIES})


# Copy resources to bin directory after building
#configure_file(${CMAKE_SOURCE_DIR}/Trollface.bmp ${CMAKE_SOURCE_DIR}/bin/Trollface.bmp COPYONLY)