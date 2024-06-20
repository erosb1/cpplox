cmake_minimum_required(VERSION 3.2)
set(PROJECT_NAME_STR clox)
project(${PROJECT_NAME_STR})
set(CMAKE_CXX_STANDARD 20)

# Include paths
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/include)

# Gather all sources except the main entry point
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Gather all header files
file(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/include/*.h ${PROJECT_SOURCE_DIR}/src/*.h)

# Add the Boost libraries
find_package(Boost COMPONENTS filesystem system unit_test_framework REQUIRED)

# Unit tests
add_subdirectory(test)

# Build
set(SOURCE_FILES ${SRC_FILES} ${PROJECT_SOURCE_DIR}/src/main.cpp ${HEADER_FILES})
add_executable(clox ${SOURCE_FILES})
target_link_libraries(clox ${Boost_LIBRARIES})

# Include Boost directories
target_include_directories(clox PRIVATE ${Boost_INCLUDE_DIRS})

# Add a custom target to check for memory leaks
add_custom_target(check_leaks
        COMMAND leaks --atExit -- ./clox
        DEPENDS clox
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMENT "Checking for memory leaks..."
)

# Make sure to build the program before running the leak check
add_dependencies(check_leaks clox)

