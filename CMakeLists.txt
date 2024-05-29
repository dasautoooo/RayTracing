cmake_minimum_required(VERSION 3.28)
project(PecanAppTemplate)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Pecan/Pecan)

add_executable(PecanAppTemplate PecanAppTemplate/src/PecanApp.cpp)

target_include_directories(PecanAppTemplate PRIVATE
    ${CMAKE_SOURCE_DIR}/Pecan/Pecan/src
)

target_link_libraries(PecanAppTemplate PRIVATE Pecan)

# macOS-specific settings
if(APPLE)
    target_compile_definitions(PecanAppTemplate PRIVATE PE_PLATFORM_MACOS)
    target_compile_options(PecanAppTemplate PRIVATE -Wall -Wextra -pedantic)  # Enable common warnings
endif()

# Debug configuration
if (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(PecanAppTemplate PRIVATE PE_DEBUG)
    target_compile_options(PecanAppTemplate PRIVATE -O0)    # Disable optimization
    target_compile_options(PecanAppTemplate PRIVATE -g)     # Generate debug information
endif()

# Release configuration
if (CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(PecanAppTemplate PRIVATE PE_RELEASE)
    target_compile_options(PecanAppTemplate PRIVATE -O3)    # Full optimization
    target_compile_options(PecanAppTemplate PRIVATE -g)     # Generate debug information
endif()

# Dist configuration
if (CMAKE_BUILD_TYPE MATCHES Dist)
    target_compile_definitions(PecanAppTemplate PRIVATE PE_DIST)
    target_compile_options(PecanAppTemplate PRIVATE -O3)    # Full optimization
    target_compile_options(PecanAppTemplate PRIVATE -s)     # Strip debug information
endif()

