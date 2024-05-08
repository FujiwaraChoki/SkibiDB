# CMakeLists.txt
cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(CPACK_GENERATOR "DEB")

set(CPACK_PACKAGE_NAME "SkibiDB")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION "A secure, lightweight, and fast DBMS.")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "SkibiDB Team")

project(SkibiDB)

# Add include directories
include_directories(include)

# Add libs
add_subdirectory(lib/json)
# add_subdirectory(lib/cryptopp-cmake)

add_executable(SkibiDB src/main.cpp)

# target_link_libraries(SkibiDB cryptopp-static)

# Done
message("SkibiDB built successfully!")

# Install
install(TARGETS SkibiDB DESTINATION bin)
