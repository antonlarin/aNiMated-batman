cmake_minimum_required(VERSION 2.8.8)
set(PROJECT_NAME_STR diffusion-solver)
project(${PROJECT_NAME_STR} C CXX)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  add_definitions(-Wno-deprecated -std=c++11 -openmp)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  add_definitions(/EHsc /openmp)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /fp:fast")
  set(MSVC_COMPILER_DEFS "-D_VARIADIC_MAX=10")
endif()


#-------------------
# set common include folder for module
#-------------------
set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)

#-------------------
# Module source
#-------------------

# copy all libraries to bin directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/bin")

# common native library
set(CORE_LIB_NAME ${PROJECT_NAME_STR})
include_directories(${COMMON_INCLUDES})
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(${CORE_LIB_NAME} SHARED ${SRC_FILES})
