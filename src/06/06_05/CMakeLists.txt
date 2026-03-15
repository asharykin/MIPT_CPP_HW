cmake_minimum_required(VERSION 3.28)
project(DynamicLibraryExample)

find_package(Boost REQUIRED COMPONENTS system filesystem)

add_subdirectory(libtest_v1)
add_subdirectory(libtest_v2)

add_executable(06_05 06_05.cpp)

target_link_libraries(06_05 Boost::system Boost::filesystem)