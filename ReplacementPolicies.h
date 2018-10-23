#pragma once
#include <bits/stdc++.h>
#include "globalVariables.h"
using namespace std;

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
		ifstream access_list("access_list.txt");

		int max_offset=pow(2, page_size) - 1;

		if(access_list.is_open())
		{
	        string line;
	        int search_index = 0; //time_stamp of the memory accesses
	        std::vector<int> next_access(table.size(),INT_MAX); //stores when will the tlb/ram entry be next accessed


	        while(getline(access_list, line))
	        {
	            search_index++;
	            stringstream ss(line);
	            int search_PID, search_address;
	            ss >> search_PID;
	            ss >> search_address;

				search_PID = pid_mapping[search_PID];

	            //Checking memory accesses that will be made after current access
	            if(search_index>access_index)
	            {
	            	bool foundall_nextaccesses=true;
	            	for(int i=0;i<table.size();i++)
	            	{
	            		bool found=false;

	            		//if the memory accessed searched/read from file is found in tlb/ram
	            		//update the value of its next access
	            		if(table[i]->pid==search_PID&&(table[i]->virtual_address<=search_address&&table[i]->virtual_address+max_offset>=search_address))
	            		{
	            			found=true;
	            			if(next_access[i]==INT_MAX)
	            			{
	            				next_access[i]=search_index;
	            			}
	            		}

	            		//Next accesses of all the tlb/ram entries have not been found yet
	            		if(next_access[i]==INT_MAX)
	            			foundall_nextaccesses=false;

	            		//If current search found in tlb/ram and next accesses of all tlb/ram entries have not been found yet
	            		if(!foundall_nextaccesses&&found)
	            			break;
	            	}

	            	//Next accesses of all tlb/ram entries found: no more seacrhing needed!
	            	if(foundall_nextaccesses)
	            		break;
	            }
	        }

	        //replace the tlb/ram entry which will be accessed last(after the most time)
	        int max_next_access=-1,pos=0;
	        for(int i=0;i<next_access.size();i++)
	        {
	        	if(max_next_access<next_access[i])
	        	{
	        		pos=i;
	        		max_next_access=next_access[i];
	        	}
	        }

	        old_index=pos;
	        old_entry=table[pos];
	        table[pos]=new_entry;
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
	return {old_entry, old_index};		
}