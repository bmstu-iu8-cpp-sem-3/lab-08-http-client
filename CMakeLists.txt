cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_DOCS "Build documentation" OFF)
option(BUILD_COVERAGE "Build code coverage" OFF)

set(
        HUNTER_CACHE_SERVERS
        "https://github.com/bmstu-iu8-cpp-sem-3/hunter-cache"
        CACHE STRING "Default cache server"
)

include("cmake/HunterGate.cmake")

HunterGate(
        URL "https://github.com/cpp-pm/hunter/archive/v0.23.278.tar.gz"
        SHA1 "35fa55bc1dfcf9f4d7264f8bb26cd1759ce1ae07"
)

project(http-client VERSION 0.1.0)
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")

hunter_add_package(asio)
find_package(asio CONFIG REQUIRED)
hunter_add_package(Beast)
find_package(Beast CONFIG REQUIRED)
hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)

file(GLOB_RECURSE PROJECT_LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/sources/lib/*.cpp")
add_library(${PROJECT_NAME}-lib STATIC ${PROJECT_LIB_SOURCES})

file(GLOB_RECURSE PROJECT_APP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/sources/app/*.cpp")
add_executable(${PROJECT_NAME}-app ${PROJECT_APP_SOURCES})

target_include_directories(${PROJECT_NAME}-lib PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:include>"
        )

target_link_libraries(${PROJECT_NAME}-lib PUBLIC asio::asio_headeronly Beast::Beast)
target_link_libraries(${PROJECT_NAME}-app asio::asio_static ${PROJECT_NAME}-lib)

if (BUILD_TESTS)
    file(GLOB_RECURSE PROJECT_TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
    add_executable(tests ${PROJECT_TEST_SOURCES})
    target_link_libraries(tests ${PROJECT_NAME}-lib GTest::gtest_main)
    enable_testing()
    add_test(NAME unit_tests COMMAND tests)
endif ()

if (BUILD_COVERAGE)
    set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage build." FORCE)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tools/coverage/cmake")
    find_package(codecov)
    add_coverage(${PROJECT_NAME}-lib)
    add_coverage(tests)
    coverage_evaluate()
endif ()

if (BUILD_DOCS)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/_docs)
    set(DOXYGEN_EXAMPLE_PATH examples)
    set(DOXYGEN_GENERATE_HTML NO)
    set(DOXYGEN_GENERATE_MAN YES)
    find_package(Doxygen)
    doxygen_add_docs(docs include)
    install(DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY}/man DESTINATION share)
    # add_dependencies(install docs) need to call target docs before install or package
endif ()

include(CPackConfig.cmake)

install(TARGETS ${PROJECT_NAME}-lib ${PROJECT_NAME}-app
        EXPORT "${PROJECT_NAME}-lib-targets"
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        INCLUDES DESTINATION include
        )

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include)

install(EXPORT "${PROJECT_NAME}-lib-targets"
        NAMESPACE "${PROJECT_NAME}::"
        DESTINATION "lib/cmake"
        )
