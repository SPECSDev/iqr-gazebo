#!/bin/bash

if [ $# -ne 1 ];
then 
    echo "Please provide an ip"
    return
fi

p=$1

export GAZEBO_MASTER_URI=$ip:11345
