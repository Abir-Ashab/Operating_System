#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
vector < int > v;
struct Process {
    int id;        
    int arrivalTime;
    int priority;  
    int burstTime;
    int waitingTime; 
    int turnaroundTime;
};

bool comparePriority(const Process& p1, const Process& p2) {
    if (p1.priority != p2.priority) {
        return p1.priority > p2.priority;
    }
    return p1.arrivalTime < p2.arrivalTime;
}

bool sortByArrivalTime(const Process &a, const Process &b) {
    return a.arrivalTime < b.arrivalTime;
}

void priorityScheduling(vector<Process>& processes) {
    sort(processes.begin(), processes.end(), sortByArrivalTime);
    sort(processes.begin() + 1, processes.end(), comparePriority);

    int n = processes.size();
    int totalTime = 0; 
    float averageWaitingTime = 0.0;
    float averageTurnaroundTime = 0.0;

    for (int i = 0; i < n; i++) {
        if (totalTime < processes[i].arrivalTime) {
            totalTime = processes[i].arrivalTime;
        }
        processes[i].waitingTime = totalTime - processes[i].arrivalTime;
        processes[i].turnaroundTime = processes[i].waitingTime + processes[i].burstTime;

        totalTime += processes[i].burstTime;
        averageWaitingTime += processes[i].waitingTime;
        averageTurnaroundTime += processes[i].turnaroundTime;
        v.push_back(processes[i].id);
    }

    averageWaitingTime /= n;
    averageTurnaroundTime /= n;
    cout << "\nAverage Waiting Time: " << averageWaitingTime << endl;
    cout << "Average Turnaround Time: " << averageTurnaroundTime << endl;
}

int main() {
    int n; 
    cin >> n;

    vector<Process> processes(n);

    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        cin >> processes[i].arrivalTime;
        cin >> processes[i].burstTime;
        cin >> processes[i].priority;
    }
    priorityScheduling(processes);
    for( auto val : v ) {
       cout << "p" << val << ' ';
    }
    return 0;
}
