#!/bin/bash

./compile.sh Programs/one_mult
for i in 1 2
do
	./Player.x $i -t Programs/one_mult > out${i}.txt 2>&1 &
done

time ./Player.x 0 -t -v 1 Programs/one_mult

