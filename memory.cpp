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

void FIFO(vector<Process>vec);
void LRU(vector<Process>vec);
void Random(vector<Process>vec);
//Process createProcess(vector<Process>vec);
void printPhysMem(vector<Page> memory);


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


void FIFO(vector<Process>vec/*, vector<bool>& pageTable*/)
{
    vector<Page> swapSpace; // "Infinite" swap space
    //vector<Pages> pageTable; // Page table for a specific process
    vector<Page> physicalMem(20); // Physical Memory pages
    cout << "Physical Memory size = " << physicalMem.size() << " Physical Memory Capacity = " << physicalMem.capacity() << endl;
    //vector<vector<Process>> physicalMem;
    vector<Page> pageQueue(20); // Not needed?
    int pageIndex = 0;
    //bool emptyPage = false;
    //vector<
    for(int i = 0; i < vec.size(); i++) {
        
        switch(vec[i].action) {
            case 'C': cout << "Process " << vec[i].process_id << " created" << endl;
                      //pageQueue.push_back(vec[i]);
                      
                      break;
            case 'T': cout << "Process " << vec[i].process_id << " terminated" << endl;
                      //Free all the pages of the terminated process
                      for(int j = 0; j < physicalMem.size(); j++) {
                          if(physicalMem[j].indiv_process.process_id == vec[i].process_id) {
                              physicalMem[j].taken = false;
                              physicalMem[j].physAddr = -1;
                              physicalMem[j].virtAddr = -1;
                          }
                      }
                      break;
            case 'A': cout << "Process " << vec[i].process_id << " allocated memory at address " << vec[i].page << endl;
                      if(pageIndex != physicalMem.capacity()) {
                          physicalMem[pageIndex].taken = true;
                          physicalMem[pageIndex].indiv_process = vec[i];
                          physicalMem[pageIndex].virtAddr = vec[i].page;
                          physicalMem[pageIndex].physAddr = pageIndex;
                          pageIndex++;
                      }
                      break;
            case 'R': cout << "Process " << vec[i].process_id << " read " << vec[i].page << endl;
                      break;
            case 'W': cout << "Process " << vec[i].process_id << " wrote to " << vec[i].page << endl;
                      break;
            case 'F': cout << "Process " << vec[i].process_id << " freed memory at address " << vec[i].page << endl;
                      break;
            default: cout << "Invalid process action" << endl;
        }
        printPhysMem(physicalMem);
        
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


}

void printPhysMem(vector<Page> memory) {
    cout << "PHYSICAL" << endl;
    for(int i = 0; i < memory.capacity(); i++) {
        cout << "\t" << i << "\t";
        if(!memory[i].taken) { cout << "FREE" << endl; }
        else {
            cout << "Process\t" << memory[i].indiv_process.process_id << endl;
        }
    }
}

//For testing output of the vector
    // for(int i=0; i<processes.size(); i++)
    // {
    //     cout << processes[i].process_id << " " << processes[i].action << " " << processes[i].page << endl;
    // }