cmake_minimum_required(VERSION 3.12)

# set the project name
project(openvdb-renderer VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


###############################################################
# PACKAGES
###############################################################

# glfw
find_package(glfw3 REQUIRED)

# opengl
find_package(OpenGL REQUIRED)

# glew
find_package(GLEW REQUIRED)

# glm
find_package(glm REQUIRED)
find_package(stb REQUIRED)

# Imgui
add_subdirectory(imgui)




# OpenVDB
find_package(OpenVDB REQUIRED)

###############################################################
# SOURCES
###############################################################
FILE(GLOB_RECURSE SOURCES src/*.c* src/*.h*)
#FILE(GLOB SOURCES src/main.cpp)


###############################################################
# EXECUTABLE
###############################################################

include_directories(${PROJECT_SOURCE_DIR} src)

# add the executable
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# Warning level
#target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)# -Werror)

target_link_libraries(
  ${CMAKE_PROJECT_NAME}
  ${OPENGL_LIBRARIES}
  glfw glm::glm
  GLEW::GLEW imgui stb::stb
  OpenVDB::OpenVDB boost::boost TBB::tbb Imath::Imath
)
