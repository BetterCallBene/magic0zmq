cmake_minimum_required(VERSION 3.22.1)

# Handled by bump2version
project(server VERSION 0.0.1
  DESCRIPTION "zmq server"
  LANGUAGES CXX C)

# Initialize the Conan build system in CMake.
# This file is generated via the `conan install -g cmake .` command
if (EXISTS ${PROJECT_BINARY_DIR}/conanbuildinfo.cmake)
    include(${PROJECT_BINARY_DIR}/conanbuildinfo.cmake)
    # Use TARGETS option here, to avoid having globally defined variables
    conan_basic_setup(TARGETS)
else ()
    message(STATUS "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif ()

find_package(ar-dev REQUIRED)

# Set various project-specific variables
ar_configure_project()

# Initialize build flags. Apply them on your target with ar_target()
ar_configure_build()

add_subdirectory(src)

# At the end, after defining all your targets place the following:
# This configures necessary packaging, e.g., CMake Package Config
# or CPack for debian packages

ar_setup_packages()
