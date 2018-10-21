g++ inputGenerator.cpp -o inputGenerator
inputGenerator.exe
del inputGenerator.exe
g++ simulator.cpp -o simulation
simulation.exe
del simulation.exe
del access_list.txt
del process_list.txt