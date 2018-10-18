#include <bits/stdc++.h>
using namespace std;

// Global variables for sizes (they are stored as log base 2)
int logical_address_space = 21; //2 MB space
int ram_size = 20; //1MB space
int page_size = 10; //1kB
int ram_table_size = ram_size - page_size;
int page_table_size = logical_address_space - page_size;
int page_directory_size = (page_table_size + 2) - (page_size); // Assuming one page_table_entry takes 4 bytes of space
int tlb_size = 7; //2^7 entries
int ram_page_index = 0; //Index from where the ram pages are not assigned to any process; // Helpful for initializing the system;

int pid_index = 0;
vector<int> requiredMemory; // stores memory requirements for processes (in multiples of kB)

struct page_table_entry{
    int physical_page_number; // This has no significance in page_directory, will keep -1 throughout the code for page_directory
    bool validity;

    page_table_entry(int a, bool b){
        physical_page_number = a;
        validity = b;
    }
};

struct tlb_entry{
    int virtual_address;
    int physical_address;
    int pid; // Address space id is the ideal thing stored by OS
    int arrival_time_stamp; //useful for FIFO replacement policy
    int recent_usage_time_stamp; //useful for LRU implementation

    tlb_entry(int v, int p, int id, int at, int rt){
        virtual_address = v;
        physical_address = p;
        pid = id;
        arrival_time_stamp = at;
        recent_usage_time_stamp = rt;
    }
};

struct ram_entry{
    int pid;
    int arrival_time_stamp; //useful for FIFO replacement policy
    int recent_usage_time_stamp; //useful for LRU implementation

    ram_entry(int id, int at, int rt){
        pid = id;
        arrival_time_stamp = at;
        recent_usage_time_stamp = rt;
    }
};

vector< tlb_entry* > tlb_table;
vector< vector< page_table_entry* > > page_tables;
vector< vector< page_table_entry* > > page_directories;
vector< ram_entry* > ram_table;

/*We are given two files :-
    process_list = list of all processes along with their sizes; memory sizes are multiples of kB, and is ensured to be less than 1GB, which is total address space available to any process;
    access_list = list of all memory accesses in sequence;
*/

ifstream process_list("process_list.txt"), access_list("access_list.txt");

void initialize_tlb(){
    tlb_table.resize(pow(2, tlb_size));
    for(int i=0;i<pow(2, tlb_size);i++){
        tlb_table[i] = NULL; //Initially there in nothing in TLB
    }
}

void initialize_ram(){
    ram_table.resize(pow(2, ram_table_size));
    for(int i=0;i<pow(2, ram_table_size);i++){
        ram_table[i] = NULL; //Initially ram is empty
    }
}

void initialize_page_directories_AND_page_tables(){
    page_directories.resize(pid_index);
    for(int i=0;i<pid_index;i++){
        page_directories[i].resize(pow(2,page_directory_size));
    }
    page_tables.resize(pid_index);
    for(int i=0;i<pid_index;i++){
        int pages_required = ceil((requiredMemory[i])/(pow(2, page_size-10) * 1.0));
        int page_ratio = pow(2, page_table_size - page_directory_size);
        int pages_required_in_directory = ceil(pages_required/(page_ratio*1.0));
        // Fill page_directory
        for(int j=0;j<pow(2,page_directory_size);j++){
            if(j<pages_required_in_directory){
                page_directories[i][j] = new page_table_entry(-1, true); 
            }
            else page_directories[i][j] = new page_table_entry(-1, false);
        }
        // Fill page_table
        for(int j=0;j<pages_required;j++){
            if(ram_page_index < ram_table_size){
                // there is space in ram, and this page can be mapped to some memory location in ram
                page_tables[i].push_back(new page_table_entry(ram_page_index, true));
                ram_table[ram_page_index] = new ram_entry(i, 0, 0); //TODO: at and rt are set to be 0 for time being, will look into it;
                ram_page_index++;
            }
            else{
                // mapping needs to be done memory location in hard disk;
                page_tables[i].push_back(new page_table_entry(-1, false)); // HardDisk Address is required, kept -1 for simulation purposes, no actual memory access is happening; 
            }
        }
    }
}

int main(){
    // First read the process_list and allocate memory to the processes
    // Basically initialize the system
    initialize_tlb();
    initialize_ram();
    if(process_list.is_open()){
        string line;
        while(getline(process_list, line)){
            stringstream ss(line);
            string tempString;
            while(ss){
                ss >> tempString;
            }
            requiredMemory.push_back(stoi(tempString));
            pid_index++;
        }
        process_list.close();
    }
    // now pid_index contains the total number of processes in the system;
    // Allocate the memory in virual spaces linearly, first few virtual pages are assigned
    // only first few pages in page directory will be valid;
    // only the required pages will be initialized in page table

    initialize_page_directories_AND_page_tables();
    // Now system has been initialized, now simulate according to the accesses;
}