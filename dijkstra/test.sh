#! /bin/bash

MAX=6

for i in `seq 0 $MAX`
do
  echo "Arquivo $i:"
  ./dijkstra < "arq0$i.in" > "arq0$i.out"
  diff "arq0$i.out" "arq0$i.res"
done
