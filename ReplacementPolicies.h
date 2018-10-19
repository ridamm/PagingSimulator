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

vector<tlb_entry*> replacement(int x,vector<tlb_entry*> tlb_table, tlb_entry* new_entry, int access_index=0)
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

	//for optimal replacement
	else if(x==2)
	{
		ifstream access_list("access_list.txt");
		if(access_list.is_open())
		{
	        string line;
	        int search_index = 0; //time_stamp of the memory accesses
	        std::vector<int> tlb_next_access(tlb_table.size(),INT_MAX); //stores when will the tlb entry be next accessed


	        while(getline(access_list, line))
	        {
	            search_index++;
	            stringstream ss(line);
	            int search_PID, search_address;
	            ss >> search_PID;
	            ss >> search_address;

	            //Checking memory accesses that will be made after current access
	            if(search_index>access_index)
	            {
	            	bool foundall_nextaccesses=true;
	            	for(int i=0;i<tlb_table.size();i++)
	            	{
	            		bool found=false;

	            		//if the memory accessed searched/read from file is found in tlb
	            		//update the value of its next access
	            		if(tlb_table[i]->pid==search_PID&&tlb_table[i]->virtual_address==search_address)
	            		{
	            			found=true;
	            			if(tlb_next_access[i]==INT_MAX)
	            			{
	            				tlb_next_access[i]=search_index;
	            			}
	            		}

	            		//Next accesses of all tlb entries have not been found yet
	            		if(tlb_next_access[i]==INT_MAX)
	            			foundall_nextaccesses=false;

	            		//If current search found in tlb and next accesses of all tlb entries have not been found yet
	            		if(!foundall_nextaccesses&&found)
	            			break;
	            	}

	            	//Next accesses of all tlb entries found: no more seacrhing needed!
	            	if(foundall_nextaccesses)
	            		break;
	            }
	        }

	        //replace the tlb entry which will be accessed last(after the most time)
	        int max_next_access=0,pos=0;
	        for(int i=0;i<tlb_next_access.size();i++)
	        {
	        	if(max_next_access>tlb_next_access[i])
	        	{
	        		pos=i;
	        		max_next_access=tlb_next_access[i];
	        	}
	        }

	        tlb_table[pos]=new_entry;
	    }
	    else
	    {
	    	printf("Unable to read memory accesses file\n");
	    }

	}

	else
	{
		printf("Undefined replacement strategy\n");
	}		
}

