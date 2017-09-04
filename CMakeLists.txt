project(SolarSystem)

cmake_minimum_required(VERSION 3.5)
#only compile the library
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw-3.2.1)

#Make Source/ and libraries(GLFW,ImGui,GLAD) available
include_directories(SolarSystem/ SolarSystem/External glfw-3.2.1/include)
#.c uses glad library
file(GLOB_RECURSE SOURCE_FILES "SolarSystem/*.cpp" "SolarSystem/*.c")
file(GLOB_RECURSE HEADER_FILES "SolarSystem/*.h")
# use c++14, it's maybe used in lambdas, not sure
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -g -O3")
add_executable (SolarSystem ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(SolarSystem glfw)
