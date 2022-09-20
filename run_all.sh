#!/bin/bash

./compile.sh Programs/one_mult
for i in 1 2 3 4 5
do
	./Player.x $i -t Programs/one_mult > out${i}.txt 2>&1 &
done

time ./Player.x 0  -max 2000,1,1 -t -v 1 Programs/one_mult | tee out0.txt 2>&1

