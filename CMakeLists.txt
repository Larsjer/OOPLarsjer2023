cmake_minimum_required(VERSION 3.15)
project(project_template)

set(CMAKE_CXX_STANDARD 17)

find_package(threepp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE threepp::threepp)
