cmake_minimum_required(VERSION 3.0.0)

# Name our project
project(hazel)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Enable warnings
add_compile_options(-Wall -Wextra -Wpedantic)

#
# Hazel
#

# Define the hazel library
add_library(hazel SHARED
  src/hazel/application.cpp
  src/hazel/log.cpp
)

# Define HAZEL_SHARED_LIBRARY so it builds as a shared library
target_compile_definitions(hazel PRIVATE HAZEL_SHARED_LIBRARY)

# Include directories for hazel
target_include_directories(hazel PUBLIC "${PROJECT_SOURCE_DIR}/src"
                                        "${PROJECT_SOURCE_DIR}/src/hazel/vendor/spdlog/include")

#
# Sandbox
#

# Define sandbox application
add_executable(sandbox
  src/sandbox/app.cpp
)

# Include directories for the sandbox application
target_include_directories(sandbox PUBLIC "${PROJECT_SOURCE_DIR}/src")

# Link sandbox with the hazel library
target_link_libraries(sandbox PUBLIC hazel)
