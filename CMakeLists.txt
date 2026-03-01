cmake_minimum_required(VERSION 3.15)
project(GeometryPatterns)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Flags de debug
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")

# Añadir Clipper2
set(CLIPPER2_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CLIPPER2_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(external/Clipper2/CPP)

# Tu ejecutable
add_executable(geometry_patterns 
    src/main.cpp
    src/GeometryProcessor.cpp
    src/GCodeGenerator.cpp
    src/patterns/IPattern.cpp
    src/patterns/RectilinearPattern.cpp
    src/patterns/HoneycombPattern.cpp
    src/patterns/SpiralPattern.cpp
    src/patterns/ConcentricPattern.cpp
)

# Incluir headers
target_include_directories(geometry_patterns PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/Clipper2/CPP/Clipper2Lib/include
)

# Enlazar biblioteca
target_link_libraries(geometry_patterns PRIVATE Clipper2)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)