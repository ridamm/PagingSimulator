REM g++ inputGenerator.cpp -o inputGenerator
REM inputGenerator.exe
REM del inputGenerator.exe
g++ simulator.cpp -o simulation
simulation.exe
del simulation.exe
REM del access_list.txt
REM del process_list.txt