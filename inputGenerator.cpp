#include <bits/stdc++.h>
using namespace std;

/*We have to create two files :-
    process_list = list of all processes along with their sizes; memory sizes are multiples of kB, and is ensured to be less than 1GB, which is total address space available to any process;
    access_list = list of all memory accesses in sequence;
*/

ofstream process_list("process_list.txt"), access_list("access_list.txt");

int main(){
    srand(time(0));
    int number_of_processes = 5;
    int number_of_accesses = 2000; 
    vector<int> memory;
    
    //create process_list
    if(process_list.is_open()){
        for(int i=0;i<number_of_processes;i++){
            // Mininum memory 1, maximum 8 bytes // Just my choice, Play with it! 
            // just make sure that required memory doesn't exceed the total virtual memory available to the process;
            int req_memory = (rand())%((int)pow(2, 3)) + 1;
            // Though total virtual space available might be large, but a process never uses that much space in practical sense
            // Hence maximum space available to a process is made to be 8 Bytes (small spaces for simulation purposes)
            if(i!=number_of_processes-1) process_list << i << " " << req_memory << endl;
            else process_list << i << " " << req_memory;
            memory.push_back(req_memory);
        }
        process_list.close();
    }
    // process_list created successfully

    // create access_list

    // Access file contains random accesses in the virtual space allocated to the corresponding process;
    if(access_list.is_open()){
        for(int i=0;i<number_of_accesses;i++){
            int access_PID = rand()%number_of_processes;
            int memory_address = rand()%(memory[access_PID]);
            if(i!=number_of_accesses-1) access_list << access_PID << " " <<  memory_address << endl;
            else access_list << access_PID << " " << memory_address;
        }
        access_list.close();
    }
    // access_list created
    return 0;
}