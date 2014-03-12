cmake_minimum_required(VERSION 2.4)

SET(CMAKE_VERBOSE_MAKEFILE TRUE) 

ADD_DEFINITIONS(-Wall -Iserialstream -DQT_THREAD_SUPPORT -D_REENTRANT -DQT_NO_DEBUG -DIQRMODULE)

#SET(QT_MT_REQUIRED TRUE)
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})


SET(libSrc
  differenceModule.cpp
)
INCLUDE_directories(
  ${CMAKE_CURRENT_SOURCE_DIR} 
  /usr/include/iqr
  ${QT_INCLUDE_DIR}
  )

ADD_LIBRARY(differenceModule SHARED ${libSrc})

SET_TARGET_PROPERTIES(differenceModule PROPERTIES PREFIX "")

TARGET_LINK_LIBRARIES(
  differenceModule 
  )





INSTALL(TARGETS 
  differenceModule
  LIBRARY DESTINATION ~/iqr/lib/Modules/
  ARCHIVE DESTINATION ~/iqr/lib/Modules/
  RUNTIME DESTINATION bin 
)


