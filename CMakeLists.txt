cmake_minimum_required(VERSION 3.20)
project(Forwarder)

file(GLOB HEADERS "headers/*.h" "headers/*.hpp")
file(GLOB SOURCES "*.cpp" "src/*.cpp")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Boost)
find_package(Threads)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

set_target_properties(
        ${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 17
        OUTPUT_NAME "fw"
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra;"
)

target_include_directories(${PROJECT_NAME} PUBLIC headers)

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
