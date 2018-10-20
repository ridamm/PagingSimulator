#!/bin/bash
g++ inputGenerator.cpp -o inputGenerator.o
./inputGenerator
rm inputGenerator
g++ simulator.cpp -o simulation
./simulation
rm simulation