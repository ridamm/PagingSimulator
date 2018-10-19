#include <bits/stdc++.h>
using namespace std;

/*We have to create two files :-
    process_list = list of all processes along with their sizes; memory sizes are multiples of kB, and is ensured to be less than 1GB, which is total address space available to any process;
    access_list = list of all memory accesses in sequence;
*/
int logical_address_space = 21; //2 MB space
ofstream process_list("process_list.txt"), access_list("access_list.txt");

int main(){
    //create process_list
    srand(time(0));
    int number_of_processes = 10;
    vector<int> memory;
    if(process_list.is_open()){
        for(int i=0;i<number_of_processes;i++){
            int req_memory = (rand())%((int)pow(2, logical_address_space - 10)) + 1; // Mininum memory 1, maximum 2048KB
            if(i!=number_of_processes-1) process_list << i << " " << req_memory << endl;
            else process_list << i << " " << req_memory;
            memory.push_back(req_memory);
        }
        process_list.close();
    }
    // process_list created successfully

    // create access_list

    // Access file contains random accesses in the virtual space allocated to the corresponding process;
    int number_of_accesses = 100; 
    if(access_list.is_open()){
        for(int i=0;i<number_of_accesses;i++){
            int access_PID = rand()%number_of_processes;
            int memory_address = rand()%((int)pow(2, memory[access_PID]));
            if(i!=number_of_accesses-1) access_list << access_PID << " " <<  memory_address << endl;
            else access_list << access_PID << " " << memory_address;
        }
        access_list.close();
    }
    // access_list created
    return 0;
}