cmake_minimum_required(VERSION 3.2)

# Project settings
set(PROJECT_UNIT_TESTS_NAME ${PROJECT_NAME_STR}_unit_tests)

# Dependencies
find_package(Boost COMPONENTS filesystem system unit_test_framework REQUIRED)

# Gather the source files for the tests
file(GLOB UNIT_TESTS_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)

# Gather the source files for the main application
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Exclude src/main.cpp
list(REMOVE_ITEM SRC_FILES ${PROJECT_SOURCE_DIR}/src/main.cpp)

# Include paths
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${Boost_INCLUDE_DIRS})

# Build unit tests
add_definitions(-DBOOST_TEST_DYN_LINK)

# Create an executable for each test source file
foreach(TEST_SRC ${UNIT_TESTS_SRC_FILES})
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SRC} ${SRC_FILES})
    target_link_libraries(${TEST_NAME} ${Boost_LIBRARIES})
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()

enable_testing()
