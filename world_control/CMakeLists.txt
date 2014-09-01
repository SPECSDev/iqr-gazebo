cmake_minimum_required(VERSION 2.4)

SET(CMAKE_VERBOSE_MAKEFILE TRUE) 

ADD_DEFINITIONS(-Wall -Iserialstream -DQT_THREAD_SUPPORT -D_REENTRANT -DQT_NO_DEBUG -DIQRMODULE)


PROJECT(worldcontrol)
#SET(QT_MT_REQUIRED TRUE)
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
FIND_PACKAGE(gazebo REQUIRED)

SET(worldcontrol_SOURCES 
  main.cpp 
  countdown.cpp
  score.cpp
  transport.cpp
  worldcontrol.cpp
  )
SET(worldcontrol_HEADERS 
  countdown.hpp
  score.hpp
  transport.hpp
  worldcontrol.hpp
  )

QT4_WRAP_CPP(worldcontrol_HEADERS_MOC ${worldcontrol_HEADERS})


INCLUDE_directories(
  ${CMAKE_CURRENT_SOURCE_DIR} 
  ${QT_INCLUDE_DIR}
  ${GAZEBO_INCLUDE_DIRS}
  )

link_directories(${GAZEBO_LIBRARY_DIRS})

ADD_EXECUTABLE(
  worldcontrol 
  ${worldcontrol_SOURCES} 
  ${worldcontrol_HEADERS_MOC}
  )

SET(GCC_BOOST_LINK_FLAGS  "-lboost_system")

TARGET_LINK_LIBRARIES(
  worldcontrol 
  ${QT_LIBRARIES}
  gazebo_transport
  gazebo_common
  gazebo_msgs
  gazebo_math
  boost_system
  protobuf
  sdformat
  pthread
  )


INSTALL(TARGETS 
  worldcontrol
  LIBRARY DESTINATION $ENV{HOME}/iqr-gazebo/gazebo_files
  ARCHIVE DESTINATION $ENV{HOME}/iqr-gazebo/gazebo_files
  RUNTIME DESTINATION $ENV{HOME}/iqr-gazebo/gazebo_files
)