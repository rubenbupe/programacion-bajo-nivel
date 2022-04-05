
#!/bin/bash


if [[ $1 == run ]]
then 
	nvcc *.cu -o bin/p2.out && ./bin/p2.out
else
	nvcc *.cu -o bin/p2.out
fi