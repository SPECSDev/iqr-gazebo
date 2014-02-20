#!/bin/bash


#for i in {1..5}
for i in {2,16}
do
    ssh  profesup@10.200.188.$i bash -s < $HOME/set_up_competition.sh

done