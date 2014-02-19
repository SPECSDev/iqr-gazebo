cmake_minimum_required(VERSION 2.4)

SET(CMAKE_VERBOSE_MAKEFILE TRUE) 

ADD_DEFINITIONS(-Wall)




SET (HebbianSrc  	   synapseHebbian.cpp)  

INCLUDE_DIRECTORIES(
  /usr/include/iqr
)



ADD_LIBRARY(synapseHebbianDecay  SHARED ${HebbianSrc})  	     



SET_TARGET_PROPERTIES(synapseHebbianDecay PROPERTIES PREFIX "")  



INSTALL(TARGETS 
  synapseHebbianDecay  
  LIBRARY DESTINATION ~/iqr/lib/Synapses/ 
  ARCHIVE DESTINATION ~/iqr/lib/Synapses/
  RUNTIME DESTINATION bin 
  )
