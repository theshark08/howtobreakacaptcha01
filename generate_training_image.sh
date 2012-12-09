#! /bin/bash

for R in 0 1 ; do
for TEXT in 0 1 2 3 4 5 6 7 8 9 ; do

A=$[20*$R - 10]
DRAW="font-size 28 translate 7,12 rotate $A text -6,12 '$TEXT'"
convert -size 18x30 xc:lightblue -draw "$DRAW" $TEXT"_"$R".jpg"

done
done

