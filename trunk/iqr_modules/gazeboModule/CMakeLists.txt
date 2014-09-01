cmake_minimum_required(VERSION 2.4)

SET(CMAKE_VERBOSE_MAKEFILE TRUE) 

ADD_DEFINITIONS(-Wall -Iserialstream -DQT_THREAD_SUPPORT -D_REENTRANT -DQT_NO_DEBUG -DIQRMODULE)

#SET(QT_MT_REQUIRED TRUE)
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

find_package(gazebo REQUIRED)

SET(libSrc
    gazebo_bug.cpp
    moduleGazeboBug.cpp
)
INCLUDE_directories(
  ${CMAKE_CURRENT_SOURCE_DIR} 
  /usr/include/iqr
  ${QT_INCLUDE_DIR}
  ${GAZEBO_INCLUDE_DIRS}
)
link_directories(${GAZEBO_LIBRARY_DIRS})


ADD_LIBRARY(moduleGazeboBug SHARED ${libSrc})

SET_TARGET_PROPERTIES(moduleGazeboBug PROPERTIES PREFIX "")

TARGET_LINK_LIBRARIES(
  moduleGazeboBug 
  gazebo_common
  gazebo_util
  gazebo_transport
  gazebo_msgs
  )





INSTALL(TARGETS 
  moduleGazeboBug
  LIBRARY DESTINATION ~/iqr/lib/Modules/
  ARCHIVE DESTINATION ~/iqr/lib/Modules/
)


