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

add_executable(SkibiDB
    src/main.cpp
    src/utilities.cpp
    src/skibidb/SkibiDB.cpp
    src/data_structs/Table.cpp
    src/data_structs/Attribute.cpp
    src/file_manager/FileManager.cpp
)

# Add nlohmann/json, folder is named json
add_subdirectory(lib/json)

# Link nlohmann/json
target_link_libraries(SkibiDB nlohmann_json::nlohmann_json -Wzero-as-null-pointer-constant)

# Done
message("SkibiDB built successfully!")

# Install
install(TARGETS SkibiDB DESTINATION bin)
