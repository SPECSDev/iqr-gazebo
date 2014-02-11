#!/bin/bash
xml=$1
n=$2
for i in $(seq $n)
do
  sed -r -e "s/((id|target|source)=\"[^\"]+[0-9])/\1-$i/g" -e "s/(iqrBug_+[0-9])/\1$i/" $xml > $i.iqrProcess
done
