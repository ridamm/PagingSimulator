#pragma once
#include <bits/stdc++.h>
using namespace std;

// Global variables for sizes (they are stored as log base 2)
int logical_address_space;
int ram_size;
int page_size;
int tlb_size;

unordered_map<int, int> pid_mapping; // For storing PIDs, as PIDs can be random, simply storing in array doesn't work

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

struct page_table_entry{
    int physical_page_number; // This has no significance in page_directory, will keep -1 throughout the code for page_directory
    bool validity;

    page_table_entry(int a, bool b){
        physical_page_number = a;
        validity = b;
    }
};

struct ram_entry{
    int pid;
	int virtual_address;
    int arrival_time_stamp; //useful for FIFO replacement policy
    int recent_usage_time_stamp; //useful for LRU implementation

    ram_entry(int id, int vAddress,int at, int rt){
        pid = id;
		virtual_address = vAddress;
        arrival_time_stamp = at;
        recent_usage_time_stamp = rt;
    }
};