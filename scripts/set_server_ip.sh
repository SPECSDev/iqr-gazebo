#!/bin/bash

if [ $# -ne 1 ];
then 
    sed -i '/GAZEBO_MASTER_URI/d' $HOME/.bashrc
    unset GAZEBO_MASTER_URI
else
    ip=$1
    sed -i '/GAZEBO_MASTER_URI/d' $HOME/.bashrc
    echo 'export GAZEBO_MASTER_URI='$ip':11345' >> $HOME/.bashrc
    . $HOME/.bashrc
fi


if ! [[ $GAZEBO_MASTER_URI ]]
then
    echo 'GAZEBO_MASTER_URI not set'
else
    echo 'GAZEBO_MASTER_URI='$GAZEBO_MASTER_URI
fi
