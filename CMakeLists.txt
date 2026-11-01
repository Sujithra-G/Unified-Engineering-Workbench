cmake_minimum_required(VERSION 3.16)
project(UnifiedEngineeringWorkbench LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_executable(workbench
    src/backend/main.cpp
    src/backend/ThermalSolver.cpp
    src/backend/Backpropagation.cpp
    src/backend/Parsers.cpp
    src/backend/HardwareManager.cpp)
target_include_directories(workbench PRIVATE src/backend)
if(MSVC)
  target_compile_options(workbench PRIVATE /W4 /O2)
else()
  target_compile_options(workbench PRIVATE -Wall -Wextra -O2)
  find_package(Threads REQUIRED)
  target_link_libraries(workbench PRIVATE Threads::Threads)
endif()

enable_testing()
add_test(NAME smoke COMMAND ${CMAKE_COMMAND}
  -DWORKBENCH=$<TARGET_FILE:workbench>
  -DPROJECT_DIR=${CMAKE_SOURCE_DIR}
  -P ${CMAKE_SOURCE_DIR}/tests/CTestSmoke.cmake)
