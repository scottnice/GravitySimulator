cmake_minimum_required(VERSION 3.6)
project (GravitySimulator)
SET(CMAKE_LIBRARY_PATH  ${PROJECT_SOURCE_DIR}/../lib)
SET(CMAKE_INCLUDE_PATH  ${PROJECT_SOURCE_DIR}/../include)

if (MSVC)
# /wd4005  macro-redefinition
# /wd4068  unknown pragma
# /wd4244  conversion from 'type1' to 'type2'
# /wd4267  conversion from 'size_t' to 'type2'
# /wd4800  force value to bool 'true' or 'false' (performance warning)
add_compile_options(/wd4005 /wd4068 /wd4244 /wd4267 /wd4800)
add_definitions(/DNOMINMAX /DWIN32_LEAN_AND_MEAN=1 /D_CRT_SECURE_NO_WARNINGS)
endif()

file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/*.h
    ${PROJECT_SOURCE_DIR}/*.cpp
)
add_executable(GravitySimulator ${SOURCES})
include_directories (${CMAKE_INCLUDE_PATH})
find_library(GLUT32_LIBRARY_PATH glut32)
find_library(GLU32_LIBRARY_PATH glu32)
target_link_libraries(GravitySimulator ${GLUT32_LIBRARY_PATH})
target_link_libraries(GravitySimulator ${GLU32_LIBRARY_PATH})

add_subdirectory(${PROJECT_SOURCE_DIR}/googletest)
add_subdirectory(${PROJECT_SOURCE_DIR}/cctz)
add_subdirectory(${PROJECT_SOURCE_DIR}/abseil-cpp)

target_link_libraries(GravitySimulator absl::base absl::synchronization absl::strings)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GravitySimulator)
