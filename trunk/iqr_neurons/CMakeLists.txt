cmake_minimum_required(VERSION 2.4)

SET(CMAKE_VERBOSE_MAKEFILE TRUE) 

ADD_DEFINITIONS(-Wall)





SET (TDValueSrc  	   neuronTDValue.cpp)  


INCLUDE_DIRECTORIES(
  /usr/include/iqr
)



ADD_LIBRARY(neuronTDValue  SHARED ${TDValueSrc})  	     


SET_TARGET_PROPERTIES(neuronTDValue PROPERTIES PREFIX "")  



INSTALL(TARGETS 
  neuronTDValue
  LIBRARY DESTINATION ~/iqr/lib/Neurons/ 
  ARCHIVE DESTINATION ~/iqr/lib/Neurons/
  RUNTIME DESTINATION bin 
  )
