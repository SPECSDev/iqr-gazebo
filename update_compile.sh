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
    return 1
else
    echo -e "\n $GREEN Compilation successful $CLEAR\n"
    return 0
fi
}

#if ! [[ $GAZEBO_MODEL_PATH && ${GAZEBO_MODEL_PATH-_} ]]
#then
    echo -e " $GREEN Setting GAZEBO_MODEL_PATH $CLEAR"
    sed -i '/GAZEBO_MODEL_PATH/d' $HOME/.bashrc
    echo 'export GAZEBO_MODEL_PATH=$HOME/iqr-gazebo/gazebo_files/models' >> $HOME/.bashrc
    . $HOME/.bashrc
#fi

#if ! [[ $GAZEBO_PLUGIN_PATH && ${GAZEBO_PLUGIN_PATH-_} ]]
#then
    echo -e " $GREEN Setting GAZEBO_PLUGIN_PATH $CLEAR"
    sed -i '/GAZEBO_PLUGIN_PATH/d' $HOME/.bashrc
    echo 'export GAZEBO_PLUGIN_PATH=$HOME/iqr-gazebo/gazebo_files/plugins' >> $HOME/.bashrc
    . $HOME/.bashrc
#fi


# checking if update_compile.sh script has changed

    if [[ -n $(svn update |grep update_compile.sh) ]]; then
	echo -e "\n $RED Script update_compile.sh has been updated please run update_compile.sh again! \n $CLEAR"
	return
    else
	echo -e "\n $GREEN Script update_compile.sh up to date! \n $CLEAR"
    fi
    

# compile 

echo -e "\n $GREEN Compiling  .... $CLEAR \n"

cd $HOME/iqr-gazebo
mkdir -p  build
cd build
cmake ..
make install

check_compile
if [ $? -ne 0 ] ; then return; fi

#install dac_libs

mkdir -p $HOME/iqr/lib/Modules
mkdir -p $HOME/iqr/lib/Neurons

arch=`uname -m`

if [ "$arch" == "x86_64" ]
then
    echo -e "\n $GREEN Installing dac_libs for x86_64... $CLEAR"
    cp $HOME/iqr-gazebo/iqr_modules/dac_libs/moduleColor_64.so $HOME/iqr/lib/Modules/moduleColor.so
    cp $HOME/iqr-gazebo/iqr_modules/dac_libs/moduleAL_64.so $HOME/iqr/lib/Modules/moduleAL.so
    cp $HOME/iqr-gazebo/iqr_modules/dac_libs/moduleCL_64.so $HOME/iqr/lib/Modules/moduleCL.so
    cp $HOME/iqr-gazebo/iqr_modules/dac_libs/neuronNumeric_64.so $HOME/iqr/lib/Neurons/neuronNumeric.so
else
    echo -e "\n $GREEN Installing dac_libs for i686... $CLEAR"
    cp $HOME/iqr-gazebo/iqr_modules/dac_libs/moduleColor_32.so $HOME/iqr/lib/Modules/moduleColor.so
    cp $HOME/iqr-gazebo/iqr_modules/dac_libs/moduleAL_32.so $HOME/iqr/lib/Modules/moduleAL.so
    cp $HOME/iqr-gazebo/iqr_modules/dac_libs/moduleCL_32.so $HOME/iqr/lib/Modules/moduleCL.so
    cp $HOME/iqr-gazebo/iqr_modules/dac_libs/neuronNumeric_32.so $HOME/iqr/lib/Neurons/neuronNumeric.so
fi
echo -e " $GREEN DONE! $CLEAR\n"


echo -e "\n $GREEN Update and Compile successful  $CLEAR\n"

cd $HOME/iqr-gazebo