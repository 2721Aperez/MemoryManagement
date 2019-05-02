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

void FIFO(vector<Process>vec);
void LRU(vector<Process>vec);
void Random(vector<Process>vec);

int main()
{
    vector<Process>processes;
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

    for(int i=0; i<processes.size(); i++)
    {
        cout << processes[i].process_id << " " << processes[i].action << " " << processes[i].page << endl;
    }


return 0;
}