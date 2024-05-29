cmake_minimum_required(VERSION 3.28)
project(RayTracing)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Pecan/Pecan)

add_executable(RayTracing RayTracing/src/PecanApp.cpp)

target_include_directories(RayTracing PRIVATE
    ${CMAKE_SOURCE_DIR}/Pecan/Pecan/src
)

target_link_libraries(RayTracing PRIVATE Pecan)

# macOS-specific settings
if(APPLE)
    target_compile_definitions(RayTracing PRIVATE PE_PLATFORM_MACOS)
    target_compile_options(RayTracing PRIVATE -Wall -Wextra -pedantic)  # Enable common warnings
endif()

# Debug configuration
if (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(RayTracing PRIVATE PE_DEBUG)
    target_compile_options(RayTracing PRIVATE -O0)    # Disable optimization
    target_compile_options(RayTracing PRIVATE -g)     # Generate debug information
endif()

# Release configuration
if (CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(RayTracing PRIVATE PE_RELEASE)
    target_compile_options(RayTracing PRIVATE -O3)    # Full optimization
    target_compile_options(RayTracing PRIVATE -g)     # Generate debug information
endif()

# Dist configuration
if (CMAKE_BUILD_TYPE MATCHES Dist)
    target_compile_definitions(RayTracing PRIVATE PE_DIST)
    target_compile_options(RayTracing PRIVATE -O3)    # Full optimization
    target_compile_options(RayTracing PRIVATE -s)     # Strip debug information
endif()

