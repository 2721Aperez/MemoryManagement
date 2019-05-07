#include <iostream>
#include<string>
#include<vector>
//#include<utility>
#include<algorithm>
#include<fstream>
#include <sstream>

using namespace std;

struct Process
{
    int process_id;
    char action;
    int page = 0;
};

struct Page//Maybe something we use??
{
    bool taken = false;
    int physAddr = -1;
    int virtAddr = -1;
    Process indiv_process;
};

struct PageTable
{
    int p_id;
    vector<Page> pages;
};

void FIFO(vector<Process>vec);
void LRU(vector<Process>vec);
void Random(vector<Process>vec);
//Process createProcess(vector<Process>vec);
void printMem(vector<Page> memory);
int findNextPage(const vector<Page> memory);
void printSwap(const vector<Page> memory);

// Function to clear physical memory of specific process
bool terminateProcess(vector<Page>& memory, int p_id);

int findPhysIndex(const vector<Page> memory, int p_id, int virtAddr);

//Process Format: PROCESS_ID    ACTION  PAGE
//Process_ID: numeric value corresponding to process running on the system
//ACTION: 'C' = Process created (PAGE field not present)
//        'T' = Process terminated (PAGE field not present)
//        'A' = Process allocated memory at address 'PAGE'
//        'R' = process read 'PAGE'
//        'W' = process wrote to 'PAGE'
//        'F' = process freed memory at address 'PAGE'

int main()
{
    //Rather than have a 2d vector and things more compilcated I created a vector of structs so now each element has an ID, action, and a page.
    vector<Process>processes;
    vector<bool>pages(20);//The amount of pages we're restricted to.  Not sure if its supposed to be a bool but that's my guess for now.  Might actually have to create another struct
    Process proc;
    ifstream process_list;
    string line;
    int count = 0;

    process_list.open("memory.dat");
    while(getline(process_list, line))
    {
        istringstream iss(line);
        iss >> proc.process_id >> proc.action >> proc.page;
        if(proc.action == 'C' || proc.action == 'T'){proc.page = 0;}
        processes.push_back(proc);
        count++;
        if(count>=100){break;}
    }
    process_list.close();

    FIFO(processes);
return 0;
}

// FIFO memory allocation, treat physical memory as a queue
void FIFO(vector<Process>vec/*, vector<bool>& pageTable*/)
{   
    PageTable pTable;
    vector<Page> swapSpace; // "Infinite" swap space
    
    //TODO: change size of physical mem back to 20
    
    //vector<Pages> pageTable; // Page table for a specific process
    vector<Page> physicalMem(5); // Physical Memory pages
    cout << "Physical Memory size = " << physicalMem.size() << " Physical Memory Capacity = " << physicalMem.capacity() << endl;
    //vector<vector<Process>> physicalMem;
    vector<Page> pageQueue(20); // Not needed?
    int pageIndex = 0;
    int virtualIndex = -1;
    int physicalIndex = -1;
    //bool emptyPage = false;
    //vector<
    for(int i = 0; i < vec.size(); i++) {
        
        switch(vec[i].action) {
            // Process created, create page table?
            case 'C': cout << "Process " << vec[i].process_id << " created" << endl;
                      //pageQueue.push_back(vec[i]);
                      pTable.p_id = vec[i].process_id;
                      break;
            // Process terminated, free all of its pages, and page table?
            case 'T': cout << "Process " << vec[i].process_id << " terminated" << endl;
                      //Free all the pages of the terminated process in physical memory
                      for(int j = 0; j < physicalMem.size(); j++) {
                          if(physicalMem[j].indiv_process.process_id == vec[i].process_id) {
                              physicalMem[j].taken = false;
                              physicalMem[j].physAddr = -1;
                              physicalMem[j].virtAddr = -1;
                          }
                      }
                      
                      // Free pages in page table
                    //   for(int k = 0; k < pTable.pages.size(); k++) {

                    //   }
                      // Code to free all of the pages of the terminated process in swap space
                      break;
            // Process allocated memory at address 'PAGE'
            case 'A': cout << "Process " << vec[i].process_id << " allocated memory at address " << vec[i].page << endl;
                      // If physical memory is not full, allocate memory at next page
                      
                      pageIndex = findNextPage(physicalMem);

                      if(pageIndex < physicalMem.capacity()) {
                          physicalMem[pageIndex].taken = true;
                          physicalMem[pageIndex].indiv_process = vec[i];
                          physicalMem[pageIndex].virtAddr = vec[i].page;
                          physicalMem[pageIndex].physAddr = pageIndex;
                          pageIndex++;
                      }
                      //else case when physical memory is full, fall back on FIFO
                      else {
                          pageIndex = 0;
                          if(!physicalMem[pageIndex].taken) {
                              physicalMem[pageIndex].taken = true;
                              physicalMem[pageIndex].indiv_process = vec[i];
                              physicalMem[pageIndex].virtAddr = vec[i].page;
                              physicalMem[pageIndex].physAddr = pageIndex;
                          }
                          else {
                              // Swap existing data to swap space
                              Page temp = physicalMem[pageIndex];
                              temp.taken = false;
                              temp.physAddr = -1;
                              swapSpace.push_back(temp);
                              physicalMem[pageIndex].taken = true;
                              physicalMem[pageIndex].indiv_process = vec[i];
                              physicalMem[pageIndex].virtAddr = vec[i].page;
                              physicalMem[pageIndex].physAddr = pageIndex;
                          }
                          pageIndex++;
                      }
                      break;
            case 'R': cout << "Process " << vec[i].process_id << " read " << vec[i].page << endl;
                      // Terminate process if trying to access page it has not prev allocated
                      //if(vec[i].process_id != physicalMem)
                      virtualIndex = -1;
                      for(int j = 0; j < physicalMem.size(); j++) {
                          if(vec[i].process_id == physicalMem[j].indiv_process.process_id && vec[i].page == physicalMem[j].virtAddr) {
                              virtualIndex = j;
                          }
                      }
                      if(virtualIndex == -1) {
                          cout << "PROCESS " << vec[i].process_id << "\t\tKILLED" << endl;
                          // Code to empty page table of killed process?
                          if(terminateProcess(physicalMem, vec[i].process_id)) { cout << "Process terminated successfuly" << endl; }
                      }
                      break;
            case 'W': cout << "Process " << vec[i].process_id << " wrote to " << vec[i].page << endl;
                      virtualIndex = -1;
                      for(int j = 0; j < physicalMem.size(); j++) {
                          if(vec[i].process_id == physicalMem[j].indiv_process.process_id && vec[i].page == physicalMem[j].virtAddr) {
                              virtualIndex = j;
                          }
                      }
                      if(virtualIndex == -1) {
                          cout << "PROCESS " << vec[i].process_id << "\t\tKILLED" << endl;
                          // Code to empty page table of killed process?
                          if(terminateProcess(physicalMem, vec[i].process_id)) { cout << "Process terminated successfuly" << endl; }
                      }
                      break;
            case 'F': cout << "Process " << vec[i].process_id << " freed memory at address " << vec[i].page << endl;
                      // Assuming freed page has already been allocated
                      physicalIndex = findPhysIndex(physicalMem, vec[i].process_id, vec[i].page);
                      
                      if(physicalIndex >= 0) {
                          physicalMem[physicalIndex].taken = false;
                          physicalMem[physicalIndex].physAddr = -1;
                          physicalMem[physicalIndex].virtAddr = -1;
                      }
                      else { 
                          cout << "Error: Process did not successfully free memory at address " << vec[i].page << endl;
                      }
                      break;
            default: cout << "Invalid process action" << endl;
        }

        //Print physical memory
        cout << "PHYSICAL" << endl;
        printMem(physicalMem);
        
        // if(vec[i].action == 'A') {
        //     pageQueue.push_back(vec[i]);

        //     if(!pageTable[i]) pageTable[i] = true;
        //     else {
        //         int pageTableIndex = i;
        //         while(!emptyPage) {
        //             pageTabl
        //             if(!pageTable[i])
        //         }


        //     }
        // }
    }

    //Print swap space
    cout << "SWAP" << endl;
    printSwap(swapSpace);
}

void printSwap(vector<Page> memory) {
    for(int i = 0; i < memory.size(); i++) {
        cout << "\tProcess\t" << memory[i].indiv_process.process_id << "\tVirtual\t" << memory[i].virtAddr << endl;
    }
}

void printMem(vector<Page> memory) {
    for(int i = 0; i < memory.capacity(); i++) {
        cout << "\t" << i << "\t";
        if(!memory[i].taken) { cout << "FREE" << endl; }
        else {
            cout << "Process\t" << memory[i].indiv_process.process_id << endl;
        }
    }
}

int findNextPage(const vector<Page> memory) {
    int index = 0;
    for(int i = 0; i < memory.size(); i++) {
        if(!memory[i].taken) { 
            index = i;
            return index;
        }
    }
    return -1; // Error: physical memory full
}

bool terminateProcess(vector<Page>& memory, int p_id) {
    bool successful = false;

    for(int i = 0; i < memory.size(); i++) {
        if(memory[i].indiv_process.process_id == p_id) {
            memory[i].taken = false;
            memory[i].virtAddr = -1;
            memory[i].physAddr = -1;
            successful = true;
        }
    }
    return successful;
}

int findPhysIndex(const vector<Page> memory, int p_id, int virtAddr) {
    int physicalAddr = -1;

    for(int i = 0; i < memory.size(); i++) {
        if(memory[i].indiv_process.process_id == p_id && memory[i].virtAddr == virtAddr)
            physicalAddr = i;
    }

    return physicalAddr;
}

//For testing output of the vector
    // for(int i=0; i<processes.size(); i++)
    // {
    //     cout << processes[i].process_id << " " << processes[i].action << " " << processes[i].page << endl;
    // }