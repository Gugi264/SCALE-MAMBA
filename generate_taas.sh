#!/bin/bash

./compile.sh Programs/triples
for i in 1 2 3 4 5
do
	./Player.x $i Programs/triples > out${i}.txt 2>&1 &
done

time ./Player.x 0  Programs/triples | tee out0.txt 2>&1 
