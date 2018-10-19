#pragma once
#include <bits/stdc++.h>

using namespace std;

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

vector<tlb_entry*> replacement(int x,vector<tlb_entry*> tlb_table, tlb_entry* new_entry)
{
	//for FIFO replacement
	if(x==0)
	{
		int min_arrival_time=INT_MAX; 
		vector<tlb_entry*>::iterator repl=tlb_table.begin();

		//check the tlb_table for the earliest/first entry
		for(vector<tlb_entry*>::iterator it=tlb_table.begin();it!=tlb_table.end();it++)
		{
			if((*it)->arrival_time_stamp<min_arrival_time)
			{
				min_arrival_time=(*it)->arrival_time_stamp;
				repl=it;
			}
		}
		*repl=new_entry; //replace the old entry with the new entry
	}

	//for LRU replacement
	else if(x==1)
	{
		int min_recent_usage=INT_MAX; 
		vector<tlb_entry*>::iterator repl=tlb_table.begin();

		//check the tlb_table for the least recently used entry
		for(vector<tlb_entry*>::iterator it=tlb_table.begin();it!=tlb_table.end();it++)
		{
			if((*it)->recent_usage_time_stamp<min_recent_usage)
			{
				min_recent_usage=(*it)->recent_usage_time_stamp;
				repl=it;
			}
		}
		*repl=new_entry; //replace the old entry with the new entry
	}
}

