#!/bin/bash

./compile.sh Programs/tutorial
for i in 1 2 3 4 5
do
	./Player.x $i -t Programs/tutorial > out${i}.txt 2>&1 &
done

time ./Player.x 0  -max 2000,0,0 -t -v 1 Programs/tutorial | tee out0.txt 2>&1

