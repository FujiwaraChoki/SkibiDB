# CMakeLists.txt
set(CMAKE_CXX_STANDARD 17)
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(CPACK_GENERATOR "DEB")

set(CPACK_PACKAGE_NAME "SkibiDB")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION "A secure, lightweight, and fast DBMS.")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "SkibiDB Team")

project(SkibiDB)

add_subdirectory(src)

add_executable(SkibiDB main.cpp)

# Libraries are in the /lib folder
link_directories(lib)

# Link nlohmann_json
target_link_libraries(SkibiDB nlohmann_json::nlohmann_json)

# Link Crypto++
target_link_libraries(SkibiDB cryptopp)

# Done
message("SkibiDB built successfully!")

# Install
install(TARGETS SkibiDB DESTINATION bin)
