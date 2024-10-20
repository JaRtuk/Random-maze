  cmake_minimum_required(VERSION 3.24)
  project(test)

  set(CMAKE_CXX_STANDARD 17)
  file(GLOB_RECURSE SOURCE_FILES
      source/*.cpp    
      source/*c
  )

  add_executable(${PROJECT_NAME} ${SOURCE_FILES})

  include_directories(
      include
  )