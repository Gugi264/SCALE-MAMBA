#!/bin/bash

./compile.sh Programs/mult
for i in 1 2 3 4 5
do
	./Player.x $i -t Programs/mult > out${i}.txt 2>&1 &
done

time ./Player.x 0 -t -v 1 Programs/mult | tee out0.txt 2>&1 

