cmake_minimum_required(VERSION 3.30)
project(IonicSimulator)

set(CMAKE_TOOLCHAIN_FILE "C:/Users/Louis/vcpkg/scripts/buildsystems/vcpkg.cmake") #replace this depending on your setup

set(CMAKE_CXX_STANDARD 20)            # Use C++20
set(CMAKE_CXX_STANDARD_REQUIRED ON)   # Enforce the C++ standard

find_package(fmt CONFIG REQUIRED)
find_package(raylib CONFIG REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp
			       src/PhysicsContainer.cpp
			       src/PhysicsEntity.cpp
			       src/App.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

