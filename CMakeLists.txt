cmake_minimum_required(VERSION 3.9)
 
PROJECT(JPEGReader)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED QUIET)
  
  
add_executable(JPEGReader MACOSX_BUNDLE JPEGReader)
 
if(VTK_LIBRARIES)
  target_link_libraries(JPEGReader ${VTK_LIBRARIES})
else()
  target_link_libraries(JPEGReader vtkHybrid vtkWidgets)
endif()