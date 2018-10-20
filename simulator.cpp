#include <bits/stdc++.h>
#include "ReplacementPolicies.h"
using namespace std;

// Global variables for sizes (they are stored as log base 2)
int logical_address_space = 21; //2 MB space
int ram_size = 20; //1MB space
int page_size = 11; //2kB
int ram_table_size = ram_size - page_size;
int page_table_size = logical_address_space - page_size;
int page_directory_size = (page_table_size + 2) - (page_size); // Assuming one page_table_entry takes 4 bytes of space
int tlb_size = 7; //2^7 entries
int ram_page_index = 0; //Index from where the ram pages are not assigned to any process; // Helpful for initializing the system;

int replacement_policy = 0; //{0, 1, 2} for various algorithms
int pid_index = 0;
vector<int> requiredMemory; // stores memory requirements for processes (in multiples of kB)

vector< tlb_entry* > tlb_table;
vector< vector< page_table_entry* > > page_tables;
vector< vector< page_table_entry* > > page_directories;
vector< ram_entry* > ram_table;

/*
    We are given two files :-
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
        int pages_required = ceil((requiredMemory[i])/(pow(2, page_size-10) * 1.0)); //-10 as requiredMemory is in KB
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
            if(ram_page_index < pow(2, ram_table_size)){
                // there is space in ram, and this page can be mapped to some memory location in ram
                page_tables[i].push_back(new page_table_entry(ram_page_index, true));
                ram_table[ram_page_index] = new ram_entry(i, 0, 0); // at and rt are set to be 0 for time being;
                ram_page_index++;
            }
            else{
                // mapping needs to be done memory location in hard disk;
                page_tables[i].push_back(new page_table_entry(-1, false)); // HardDisk Address is required, kept -1 for simulation purposes, no actual memory access is happening; 
            }
        }
    }
}

string getBitRepresentation(int a){
    // returns bit representation having total bits = number of bits in logical_address_space (from MSB to LSB)
    string result = "";
    int numbits = 0;
    while(a!=0 || numbits < logical_address_space){
        result += (a%2==1)?'1':'0';
        a = a/2;
        numbits++;
    }
    reverse(result.begin(), result.end());
    return result;
}

int getIntegerFromString(string s){
    // s is in binary (MSB - LSB) form, returns the corresponding decimal integer
    // Horner's Algorithm
    int number = 0;
    for(int i=0;i<s.size();i++){
        number *= 2;
        number += (int)(s[i]-'0');
    }
    return number;
}

int processVirtualAddress(int pid, int vAddress){
    string bitString = getBitRepresentation(vAddress);
    string directory_string = bitString.substr(0, page_directory_size);
    string page_table_string = bitString.substr(page_directory_size, page_table_size - page_directory_size);
    string page_offset_string = bitString.substr(page_table_size);
    int pte_index = getIntegerFromString(directory_string) * pow(2, page_table_size - page_directory_size) + getIntegerFromString(page_table_string);
    if(page_tables[pid][pte_index]->validity){
        return (page_tables[pid][pte_index]->physical_page_number)*(pow(2, page_size)) + getIntegerFromString(page_offset_string); 
    }
    return -1;
}

int findInTLB(int id, int vAddress, int access_stamp){
    for(int i=0;i<pow(2, tlb_size);i++){
        if(tlb_table[i]!=NULL && tlb_table[i]->pid==id && tlb_table[i]->virtual_address==vAddress){
            tlb_table[i]->recent_usage_time_stamp = access_stamp;
            return tlb_table[i]->physical_address;
        }
    }
    return -1;
}

void flushTLB(){
    // empties the Translation lookaside buffer
    for(int i=0;i<pow(2, tlb_size);i++){
        tlb_table[i] = NULL;
    }
}

void updatePageTableForNew(int pid, int vAddress, int ppn){
    string bitString = getBitRepresentation(vAddress);
    string directory_string = bitString.substr(0, page_directory_size);
    string page_table_string = bitString.substr(page_directory_size, page_table_size - page_directory_size);
    string page_offset_string = bitString.substr(page_table_size);
    int pte_index = getIntegerFromString(directory_string) * pow(2, page_table_size - page_directory_size) + getIntegerFromString(page_table_string);
    page_tables[pid][pte_index]->physical_page_number = ppn;
    page_tables[pid][pte_index]->validity = true;
}

void updatePageTableForOld(int pid, int ppn){
    for(int i=0;i<page_tables[pid].size();i++){
        if(page_tables[pid][i]!=NULL && page_tables[pid][i]->validity && page_tables[pid][i]->physical_page_number==ppn){
            page_tables[pid][i]->validity = false;
            page_tables[pid][i]->physical_page_number = -1;
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

    if(access_list.is_open()){
        string line;
        int access_index = 0; // will use it as a time_stamp for replacement policies
        while(getline(access_list, line)){
            access_index++;
            stringstream ss(line);
            int access_PID, access_address;
            ss >> access_PID;
            ss >> access_address;
            printf("process number %d has requested to access the virtual address : %d\n", access_PID, access_address);
            int physical_address = findInTLB(access_PID, access_address, access_index);
            printf("looking for the required entry in translation lookaside buffer\n");
            if(physical_address != -1){
                // corresponding entry was found in TLB
                printf("Its a TLB hit, %d is accessed successfully\n", physical_address);
                ram_table[physical_address/(pow(2, page_size))]->recent_usage_time_stamp = access_index;
                continue;
            }
            printf("Ohh no, a TLB miss\n");
            physical_address = processVirtualAddress(access_PID, access_address);
            if(physical_address != -1){
                // required page was present in RAM
                printf("Memory access %d was succesfull\n", physical_address);
                tlb_entry* new_tlb_entry = new tlb_entry(access_address, physical_address, access_PID, access_index, access_index);
                pair<tlb_entry*, int> old = replacement(replacement_policy, tlb_table, new_tlb_entry, access_index);
                printf("new tlb entry is inserted at %d\n", old.second);
                ram_table[physical_address/(pow(2, page_size))]->recent_usage_time_stamp = access_index;
                continue; 
            }
            // Handle page fault by updating ram, flushing TLB
            printf("Page fault occured, flushing existing TLB, updating ram\n");
            flushTLB();
            ram_entry* new_entry = new ram_entry(access_PID, access_index, access_index);
            pair<ram_entry*, int> old = replacement(replacement_policy, ram_table, new_entry, access_index);
            printf("new ram entry is inserted at %d\n", old.second);
            // Now we need to update pageTables of two processes
            // one for which new entry has entered the ram - Also tlb needs to be updated for this
            // one for which already existing page has moved to hard disk now
            updatePageTableForNew(access_PID, access_address, old.second);
            if(old.first != NULL) updatePageTableForOld(old.first->pid, old.second); //Actually some replacement took place
            tlb_entry* new_tlb_entry = new tlb_entry(access_address, physical_address, access_PID, access_index, access_index);
            pair<tlb_entry*, int> oldTLB = replacement(replacement_policy, tlb_table, new_tlb_entry, access_index);
            printf("new tlb entry is inserted at %d\n", oldTLB.second);
        }
    }
}