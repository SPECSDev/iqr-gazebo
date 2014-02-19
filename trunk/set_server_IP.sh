#!/bin/bash

if [ $# -ne 1 ];
then 
    echo "Please provide an ip"
    return
fi

ip=$1

sed -i '/GAZEBO_MASTER_URI/d' $HOME/.bashrc
echo 'export GAZEBO_MASTER_URI='$ip':11345' >> $HOME/.bashrc

. $HOME/.bashrc

echo $GAZEBO_MASTER_URI