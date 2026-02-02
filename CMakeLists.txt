cmake_minimum_required(VERSION 3.18)
project(pybind_complex_demo LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Python3 COMPONENTS Interpreter Development.Module REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

execute_process(
  COMMAND "${Python3_EXECUTABLE}" -c "import numpy; print(numpy.get_include())"
  OUTPUT_VARIABLE NUMPY_INCLUDE
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

if (NOT NUMPY_INCLUDE)
  message(FATAL_ERROR "Could not determine NumPy include directory. Is numpy installed in this Python env?")
endif()

pybind11_add_module(complex_ext src/complex_ext.cpp)
target_include_directories(complex_ext PRIVATE "${NUMPY_INCLUDE}")

if (MSVC)
  target_compile_options(complex_ext PRIVATE /W4)
else()
  target_compile_options(complex_ext PRIVATE -O3 -Wall -Wextra -Wpedantic)
endif()

# Install the extension module into the Python package directory inside the wheel.
install(TARGETS complex_ext
  LIBRARY DESTINATION pybind_complex_demo
  RUNTIME DESTINATION pybind_complex_demo
  ARCHIVE DESTINATION pybind_complex_demo
)
