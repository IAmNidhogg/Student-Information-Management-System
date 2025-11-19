cmake_minimum_required(VERSION 3.10)
project("Student Information Management System")

if(EXISTS ${PROJECT_SOURCE_DIR}/build)
    file(REMOVE_RECURSE ${PROJECT_SOURCE_DIR}/build)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin)
include_directories(${PROJECT_SOURCE_DIR}/include)

include_directories(${PROJECT_SOURCE_DIR}/include/json)
link_directories(${PROJECT_SOURCE_DIR}/lib)

file(GLOB SRC ${PROJECT_SOURCE_DIR}/src/*.cpp) 
file(GLOB HPP ${PROJECT_SOURCE_DIR}/include/*.hpp)
set(MAIN_SRC ${PROJECT_SOURCE_DIR}/main/main.cpp)

add_executable(sims ${SRC} ${MAIN_SRC})
target_link_libraries(sims json)
