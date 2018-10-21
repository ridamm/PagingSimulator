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

template <typename T>
pair<T*, int> tryFillingTable(vector<T*> &table, T* new_entry){
	typename vector<T*>::iterator repl=table.begin();
	int old_index = -1;
	//check the table for the earliest empty space
	int i = 0;
	for(typename vector<T*>::iterator it=table.begin();it!=table.end();it++)
	{
		if((*it) == NULL)
		{
			repl=it;
			old_index = i;
			break;
		}
		i++;
	}
	if(old_index != -1){
		// We have identified the empty space
		*repl = new_entry;
	}
	return {NULL, old_index};
}

/*  
policy_type::
	0 : FIFO
	1 : LRU
	2 : Optimal
*/

template <typename T>
pair<T*, int> replacement(int policy_type, vector<T*> &table, T* new_entry, int access_index=0)
{
	// First lets check if there is empty space available, hence no replacement needed;
	pair<T*, int> old = tryFillingTable(table, new_entry);
	if(old.second != -1){
		return old;
	}

	T* old_entry = NULL; // will return this after modifying the table;
	int old_index = -1;
	//for FIFO replacement
	if(policy_type==0)
	{
		int min_arrival_time=INT_MAX; 
		typename vector<T*>::iterator repl=table.begin();

		//check the table for the earliest/first entry
		int i = 0;
		for(typename vector<T*>::iterator it=table.begin();it!=table.end();it++)
		{
			if((*it)->arrival_time_stamp<min_arrival_time)
			{
				min_arrival_time=(*it)->arrival_time_stamp;
				repl=it;
				old_index = i;
			}
			i++;
		}
		old_entry = *repl;
		*repl=new_entry; //replace the old entry with the new entry
	}

	//for LRU replacement
	else if(policy_type==1)
	{
		int min_recent_usage=INT_MAX; 
		typename vector<T*>::iterator repl=table.begin();

		//check the table for the least recently used entry
		int i = 0;
		for(typename vector<T*>::iterator it=table.begin();it!=table.end();it++)
		{
			if((*it)->recent_usage_time_stamp<min_recent_usage)
			{
				min_recent_usage=(*it)->recent_usage_time_stamp;
				repl=it;
				old_index = i;
			}
			i++;
		}
		old_entry = *repl;
		*repl=new_entry; //replace the old entry with the new entry
	}

	//for optimal replacement
	else if(policy_type==2)
	{
	// 	ifstream access_list("access_list.txt");
	// 	if(access_list.is_open())
	// 	{
	//         string line;
	//         int search_index = 0; //time_stamp of the memory accesses
	//         std::vector<int> tlb_next_access(table.size(),INT_MAX); //stores when will the tlb entry be next accessed


	//         while(getline(access_list, line))
	//         {
	//             search_index++;
	//             stringstream ss(line);
	//             int search_PID, search_address;
	//             ss >> search_PID;
	//             ss >> search_address;

	//             //Checking memory accesses that will be made after current access
	//             if(search_index>access_index)
	//             {
	//             	bool foundall_nextaccesses=true;
	//             	for(int i=0;i<table.size();i++)
	//             	{
	//             		bool found=false;

	//             		//if the memory accessed searched/read from file is found in tlb
	//             		//update the value of its next access
	//             		if(table[i]->pid==search_PID&&table[i]->virtual_address==search_address)
	//             		{
	//             			found=true;
	//             			if(tlb_next_access[i]==INT_MAX)
	//             			{
	//             				tlb_next_access[i]=search_index;
	//             			}
	//             		}

	//             		//Next accesses of all tlb entries have not been found yet
	//             		if(tlb_next_access[i]==INT_MAX)
	//             			foundall_nextaccesses=false;

	//             		//If current search found in tlb and next accesses of all tlb entries have not been found yet
	//             		if(!foundall_nextaccesses&&found)
	//             			break;
	//             	}

	//             	//Next accesses of all tlb entries found: no more seacrhing needed!
	//             	if(foundall_nextaccesses)
	//             		break;
	//             }
	//         }

	//         //replace the tlb entry which will be accessed last(after the most time)
	//         int max_next_access=0,pos=0;
	//         for(int i=0;i<tlb_next_access.size();i++)
	//         {
	//         	if(max_next_access>tlb_next_access[i])
	//         	{
	//         		pos=i;
	//         		max_next_access=tlb_next_access[i];
	//         	}
	//         }

	//         table[pos]=new_entry;
	//     }
	//     else
	//     {
	//     	printf("Unable to read memory accesses file\n");
	//     }
	}

	else
	{
		printf("Undefined replacement strategy\n");
	}
	return {old_entry, old_index};		
}