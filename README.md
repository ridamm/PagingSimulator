# PagingSimulator

The following code simulates the paging system present in modern day operating systems. Three main replacement algorithms are implemented for any TLB replacement/ Ram Replacement;

### Replacement Policies Used :-
1. FIFO
2. LRU
3. Optimal Greedy Technique (Note : This is not practically Implemented anywhere, just for simulation purposes)

## Structure :-
It implements two level paging i.e. one page directory and one page table.  
We are given two files :-  
1. Process file which contains the process ids and the corresponding space requirement, note this space requirement can't exceed that of the total virtual space provided by OS.
2. Access file which contains the process ids and the corresponding requested memory access, note this should be a valid access (within memory as allocated by the OS in beginning), else the access is just ignored

To initialize the system, memory requirements are kept contigous in their virtual address space. Initially TLB is empty and it will start filling up as the accesses start to happen. RAM is initialized as soon as the process start requesting memory (when we traverse process file). Till there is space in RAM, it is allocated space in RAM, else its been allocated in Swap Space *(Note : We have assumed swap space to be infinite enough for all memory requirements, hence its not separately handled, how pages are retrieved from or are stored to swap space is being abstraced in the current implementation)*. Initial space allocation in RAM is done contigously for each process, but as we start accessing the memory addresses, this won't remain the case. 

Few essential parameters are taken input at the begining of the code :  
1. Total Bits in virtual Address space (like 64 bit/32 bit systems you people might be using)
2. Total Size of ram (in the log scale, i.e. for memory of size 2^k Bytes, specify k)
3. Size of one page (in the log scale)
4. Total number of TLB entries (in the log scale)
5. Replacement policy to follow

An additional input generator file is provided which provides random memory requirement and random accesses  

## Output:-
For each access, the simulation is live tracked. It prints the corresponding TLB hit/miss, page fault etc.
After the simulation is complete, a descriptive summary is presented in front of the user which shows the total number of TLB hits/ misses and page faults. Along with this, the same info is also presented for each of the process present in the simulation.  

The code is in C++14. All you need is 
* run ./simulate.sh if you are on ubuntu/linux
* run simulate.bat if you are on windows system