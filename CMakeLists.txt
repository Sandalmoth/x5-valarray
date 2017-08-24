cmake_minimum_required(VERSION 3.0.0)

add_executable(test-read test/r.cpp)

set_target_properties(test-read PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test")

# Libraries
find_package(HDF5)
if (HDF5_FOUND)
  include_directories(${HDF5_INCLUDE_DIRS})
  target_link_libraries(test-read ${HDF5_LIBRARIES} -lhdf5_cpp)
endif (HDF5_FOUND)

# Compilation flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -ggdb") # debug compilation
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -Werror")
endif()
