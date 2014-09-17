#!/bin/bash


#for i in {1..5}
for i in {2,16}
do
    gnome-terminal -x sh -c   "profesup@10.200.188.$i bash -s < $HOME/iqr-gazebo/scripts/set_up_competition.sh"

done