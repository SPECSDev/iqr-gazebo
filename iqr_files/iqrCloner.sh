#!/bin/bash
xml=$1
n=$2
#name=$3
name=Bug
   
mkdir -p  clones

for i in $(seq $n)
do
    sed -r -e "s/Bug_01/Bug_0$i/" $xml > clones/${name}_0${i}.iqr
done
