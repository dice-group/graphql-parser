cmake_minimum_required(VERSION 3.16)
project(graphql-parser VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)


set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -g -O0")

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)


find_package(graphql-parser-base REQUIRED)

# installation directories
set(graphql-parser_INSTALL_INCLUDE_DIR "include" CACHE STRING "The installation include directory")
set(graphql-parser_INSTALL_CMAKE_DIR "share/graphql-parser/cmake" CACHE STRING "The installation cmake directory")

# define a static library
#add_library(graphql-parser STATIC
#        ${CMAKE_CURRENT_SOURCE_DIR}/src/Dice/graphql-parser/GraphQLParser.cpp)

# define a header-only library
add_library(graphql-parser INTERFACE)
add_library(graphql-parser::graphql-parser ALIAS graphql-parser)

target_include_directories(graphql-parser INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${graphql-parser_INSTALL_CMAKE_DIR}>
)
#target_include_directories(graphql-parser PUBLIC
#        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#        $<INSTALL_INTERFACE:${graphql-parser_INSTALL_CMAKE_DIR}>
#        )
#target_link_libraries(graphql-parser INTERFACE
#        graphql-parser-base::graphql-parser-base
#        )
target_link_libraries(graphql-parser INTERFACE
        graphql-parser-base::graphql-parser-base
        )

# require C++20
set_property(TARGET graphql-parser PROPERTY INTERFACE_CXX_STANDARD 20)

# Make package findable
configure_file(cmake/dummy-config.cmake.in graphql-parser-config.cmake @ONLY)

# Enable version checks in find_package
include(CMakePackageConfigHelpers)
write_basic_package_version_file(graphql-parser-config-version.cmake COMPATIBILITY SameMajorVersion)

# install and export target
install(TARGETS graphql-parser EXPORT graphql-parser-targets)

install(EXPORT graphql-parser-targets
        FILE graphql-parser-config.cmake
        NAMESPACE Dice::
        DESTINATION ${graphql-parser_INSTALL_CMAKE_DIR}
        )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/graphql-parser-config-version.cmake DESTINATION ${graphql-parser_INSTALL_CMAKE_DIR})
install(DIRECTORY include/ DESTINATION ${graphql-parser_INSTALL_INCLUDE_DIR})


# testing
option(GRAPHQL_PARSER_BUILD_TESTS "Build graphql-parser tests." OFF)
if (GRAPHQL_PARSER_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()
unset(GRAPHQL_PARSER_BUILD_TESTS CACHE)