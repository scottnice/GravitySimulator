cmake_minimum_required(VERSION 3.6)
project (GravitySimulator)
SET(CMAKE_LIBRARY_PATH  ${PROJECT_SOURCE_DIR}/../lib)
SET(CMAKE_INCLUDE_PATH  ${PROJECT_SOURCE_DIR}/../include)

file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/*.h
    ${PROJECT_SOURCE_DIR}/*.cpp
)
add_executable(GravitySimulator ${SOURCES}
)
include_directories (${CMAKE_INCLUDE_PATH})
find_library(GLUT32_LIBRARY_PATH glut32)
find_library(GLU32_LIBRARY_PATH glu32)


add_subdirectory(${PROJECT_SOURCE_DIR}/googletest)
add_subdirectory(${PROJECT_SOURCE_DIR}/cctz)
add_subdirectory(${PROJECT_SOURCE_DIR}/abseil-cpp)

target_link_libraries(GravitySimulator ${GLUT32_LIBRARY_PATH})
target_link_libraries(GravitySimulator ${GLU32_LIBRARY_PATH})
target_link_libraries(GravitySimulator absl::strings)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GravitySimulator)
