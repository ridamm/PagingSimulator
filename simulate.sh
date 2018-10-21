#!/bin/bash
g++ inputGenerator.cpp -o inputGenerator
./inputGenerator
rm inputGenerator
g++ simulator.cpp -o simulation
./simulation
rm simulation
rm access_list.txt
rm process_list.txt