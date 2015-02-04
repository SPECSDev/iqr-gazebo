#!/bin/bash


#for i in {1..5}
for i in {4,5}
do
    gnome-terminal -x sh -c   "ssh profesup@10.200.187.$i bash -s < $HOME/iqr-gazebo/scripts/set_up_competition.sh"

done