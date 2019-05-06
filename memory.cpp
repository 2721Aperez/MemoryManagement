#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<fstream>
#include<sstream>
#include<queue>
#include<unordered_map>
#include<set>


using namespace std;

int CAPACITY = 20;

struct Process
{
    int process_id;
    char action;
    int page = 0;
};

struct Page//Maybe something we use??
{
    bool taken = false;
    Process indiv_process;
};

void FIFO(vector<Process>processes, vector<Page>pages);
void LRU(vector<Process>processes, vector<Page>pages);
void Random(vector<Process>vec);

int main()
{
    //Rather than have a 2d vector and things more compilcated I created a vector of structs so now each element has an ID, action, and a page.
    vector<Process>processes;
    vector<Page>pages(20);//The amount of pages we're restricted to. Physical Memory
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

    LRU(processes, pages);
return 0;
}


void FIFO(vector<Process>processes, vector<Page>pages)
{
    queue<Process>proc_q;

    for(auto proc : processes)
    {
        
    }
}

void LRU(vector<Process>processes, vector<Page>pages)
{
    vector<Process>proc_updates;
    unordered_map<int,int>indexes;
    set<int>process_created;
    int page_spot = 0;

    for(int i=0; i< processes.size(); i++)
    {
        if(processes[i].action == 'C')
        {
            if(process_created.find(processes[i].process_id) == process_created.end()){ process_created.insert(processes[i].process_id); }
            else{cout << "Process has already been created. " << endl;}
        }
        else if(processes[i].action == 'T')
        {
             if(process_created.find(processes[i].process_id) != process_created.end())
             { 
                process_created.erase(process_created.find(processes[i].process_id)); 
                for(int j=0; j<pages.size(); j++)
                {
                    if(pages[j].indiv_process.process_id == processes[i].process_id)
                    {
                        pages.erase(pages.begin()+j);
                        pages[j].taken = false;
                    }
                }
             }
             else{ cout << "Process doesn't exist. "<< endl; }
        }
        else if(processes[i].action == 'A')
        {
            int amnt_allocate = processes[i].page;
            
            for(int j=0; j<pages.size() && (amnt_allocate) > 0; j++)
            {
                if(!pages[j].taken)
                {
                    pages[j].indiv_process = processes[i];
                    pages[j].taken = true;
                    amnt_allocate--;
                }
            }
        }
        else if(processes[i].action == 'R')
        {
            /* code */
        }
        else if(processes[i].action == 'W')
        {
            /* code */
        }
        else if(processes[i].action == 'F')
        {
            /* code */
        }
    }
}

//For testing output of the vector
    // for(int i=0; i<processes.size(); i++)
    // {
    //     cout << processes[i].process_id << " " << processes[i].action << " " << processes[i].page << endl;
    // }