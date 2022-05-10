#!/bin/bash


if [[ $1 == run ]]
then 
	g++ -std=c++17 *.cpp -pthread -o bin/p1.out && ./bin/p1.out
else
	g++ -std=c++17 *.cpp -pthread -o ./bin/p1.out -g 
fi