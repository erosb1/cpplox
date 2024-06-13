cmake_minimum_required(VERSION 3.2)
project(LoxInterpreter)
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
add_executable(cool ${SOURCE_FILES})
target_link_libraries(cool ${Boost_LIBRARIES})

# Include Boost directories
target_include_directories(cool PRIVATE ${Boost_INCLUDE_DIRS})
