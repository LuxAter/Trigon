#!/bin/bash

for i in {1..$1}; do
  ./trigon -r -n $2 -o images/out_1
done
