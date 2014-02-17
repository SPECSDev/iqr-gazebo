#!/bin/bash

GREEN="\033[92;5;148m"
RED="\033[31;5;148m"
CLEAR="\033[39m"



##################
function check_compile
##################
{
if [ $? -ne 0 ]
then
    echo -e "\n $RED Compilaltion failed! $CLEAR\n "
    exit 1
else
    echo -e "\n $GREEN Complilation successful $CLEAR\n"
fi
}


svn update

#compile module 

echo -e "\n $GREEN Compiling iqr_module .... $CLEAR \n"

cd $HOME/iqr-gazebo/iqr_module
mkdir -p  build
cd build
cmake ..
make install

check_compile


#compile vel_plugin

echo -e "\n $GREEN Compiling vel_plugin .... $CLEAR \n"

cd $HOME/iqr-gazebo/gazebo_files/vel_plugin
mkdir -p build
cd build
cmake ..
make install

check_compile

#compile target_plugin

echo -e "\n $GREEN Compiling target_plugin .... $CLEAR \n"

cd $HOME/iqr-gazebo/gazebo_files/target_plugin
mkdir -p build
cd build
cmake ..
make install

check_compile

#compile worldcontrol
echo -e "\n $GREEN Compiling Worldcontrol .... $CLEAR \n"

cd $HOME/iqr-gazebo/gazebo_files/world_control
mkdir -p build
cd build
cmake ..
make  

check_compile

echo -e "\n $GREEN DONE! $CLEAR\n"

