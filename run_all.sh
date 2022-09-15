#!/bin/bash

./compile.sh Programs/mult_demo
for i in 1 2 3 4 5
do
	./Player.x $i -t Programs/mult_demo > out${i}.txt 2>&1 &
done

time ./Player.x 0 -t -v 1 Programs/mult_demo | tee out0.txt 2>&1 

