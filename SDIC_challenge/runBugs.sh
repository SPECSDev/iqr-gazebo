#!/bin/bash
default_n=4
n=${1:-$default_n}

rm -rf clnes

./iqrCloner.sh gazeboBug.iqr $n

if [ $n -le 2 ]
then
    gnome-terminal -x sh -c "gazebo $HOME/iqr-gazebo/gazebo_files/SDIC_test_arena.world"
else
    gnome-terminal -x sh -c "gazebo $HOME/iqr-gazebo/gazebo_files/SDIC_arena.world"
fi
sleep 4
gnome-terminal -x sh -c "$HOME/iqr-gazebo/gazebo_files/world_control/build/worldcontrol -r"


for i in $(seq $n)
do
    gnome-terminal -x sh -c "iqr -r -f clones/Bug_0$i.iqr"
done

