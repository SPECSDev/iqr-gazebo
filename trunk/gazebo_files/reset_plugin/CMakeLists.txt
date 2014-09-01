cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

include (FindPkgConfig)
find_package(gazebo REQUIRED)

include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})

add_library(reset_plugin SHARED reset_plugin.cc)
target_link_libraries(reset_plugin ${GAZEBO_libraries})

INSTALL(TARGETS 
  reset_plugin
  LIBRARY DESTINATION $ENV{GAZEBO_PLUGIN_PATH}
  ARCHIVE DESTINATION $ENV{GAZEBO_PLUGIN_PATH}
  RUNTIME DESTINATION bin 
)