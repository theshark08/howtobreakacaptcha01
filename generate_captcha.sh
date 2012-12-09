#! /bin/bash

TEXT=$[($RANDOM*$RANDOM % 1000000)]
TEXT=`printf "%06d" $TEXT`

for l in 0 1 2 3 4 5 ; do
 A[$l]=$[(2*($RANDOM % 2)-1) * 10]
 X[$l]=$[($RANDOM % 5)-2]
 Y[$l]=$[($RANDOM % 5)-2]
done

DRAW=" line 0,"$[$RANDOM % 30]" 100,"$[$RANDOM % 30]" line 0,"$[$RANDOM % 30]" 100,"$[$RANDOM % 30]" line 0,"$[$RANDOM % 30]" 100,"$[$RANDOM % 30]" font-size 28 translate 0,25 "

for l in 0 1 2 3 4 5 ; do
  DRAW=$DRAW"translate "$[11+${X[$l]}]","${Y[$l]}" rotate ${A[$l]} text 0,0 '${TEXT:$l:1}' rotate $[-${A[$l]}] translate "$[-${X[$l]}]","$[-${Y[$l]}]" "
done

convert -size 100x30 xc:lightblue -draw "$DRAW" $1

JSON="{\"text\":\""$TEXT"\",\"pos\":["

for l in 0 1 2 3 4 ; do
  JSON=$JSON$[${X[$l]}+10*($l+1)]","$[${Y[$l]}+25]","
done
JSON=$JSON$[${X[5]}+60]","$[${Y[5]}+25]

JSON=$JSON"]}"

echo $JSON
