cmake_minimum_required(VERSION 2.8)

SET(CMAKE_VERBOSE_MAKEFILE TRUE)

ADD_SUBDIRECTORY(iqr_neurons)

ADD_SUBDIRECTORY(iqr_modules/differenceModule)
ADD_SUBDIRECTORY(iqr_modules/gazeboModule)
ADD_SUBDIRECTORY(iqr_modules/directionModule)

ADD_SUBDIRECTORY(world_control)

ADD_SUBDIRECTORY(gazebo_files/vel_plugin)
ADD_SUBDIRECTORY(gazebo_files/target_plugin)
ADD_SUBDIRECTORY(gazebo_files/reset_plugin)

