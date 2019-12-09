#!/bin/bash

COLORS=(red pink purple deep-purple indigo blue light-blue cyan teal green light-green lime yellow amber orange deep-orange brown grey blue-grey)

function gen_gradiant() {
  if [[ $(shuf -i 0-10 -n 1) -ge 5 ]]; then
    COUNT=$(shuf -i 2-3 -n 1)
    for i in $(seq 1 $COUNT); do
      printf "${COLORS[$(shuf -i 0-18 -n 1)]}-$(shuf -i 0-9 -n 1)"
      if [[ $i -ne $COUNT ]]; then
        printf ":"
      fi
    done
  else
    if [[ $(shuf -i 0-1 -n 1) == 1 ]]; then
      printf "n"
    fi
    printf "${COLORS[$(shuf -i 0-18 -n 1)]}s"
  fi
}

# for i in $(seq 1 $1); do
#   AREA=$(shuf -i 1000-100000 -n 1)
#   ORIGIN=$(shuf -i 0-1 -n 1)
#   PALLET=$(gen_gradiant)
#   # if [[ $ORIGIN -eq 1 ]]; then
#   #   OX=$(bc <<< "scale=5; $(shuf -i 0-100 -n 1) / 100")
#   #   OY=$(bc <<< "scale=5; $(shuf -i 0-100 -n 1) / 100")
#   #   echo "./trigon delaunay -qr 10000 -e png -a $AREA -p \"$PALLET\" --center $OX,$OY -o imgs/$i"
#   #   ./trigon delaunay -qr 10000 -e png -a $AREA -p "$PALLET" --center $OX,$OY -o imgs/$i
#   # else
#   echo "./trigon delaunay -qr 10000 -e png -a $AREA -p \"$PALLET\" -o imgs/$i"
#   ./trigon delaunay -qr 10000 -e png -a $AREA -p "$PALLET" -o imgs/$i
#   # fi
# done

pids=()

for COLOR in ${COLORS[@]}; do
  mkdir imgs/$COLOR
  COUNTER=0
  PALLET=$(printf "%ss" "$COLOR")
  for AREA in $(seq $1 $3 $2); do
    ./trigon delaunay -qr 8120 -e png -a $AREA -p "$PALLET" -o imgs/$COLOR/$AREA --aspect 14:10 &
    pids[${COUNTER}]=$!
    COUNTER=$((COUNTER + 1))
    if [[ $COUNTER -ge 4 ]]; then
      echo "WAITING"
      for pid in ${pids[*]}; do
        wait $pid
      done
      pids=()
      COUNTER=0
    fi
  done
  for pid in ${pids[*]}; do
    wait $pid
  done
done
