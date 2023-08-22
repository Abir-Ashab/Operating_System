#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
 
struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int waitingTime;
    int turnaroundTime;
};

bool sortByBurstTime(const Process &a, const Process &b) {
    return a.burstTime < b.burstTime;
}

int main() {
    int n; cin >> n;
    vector<Process> processes(n);
    vector < ll > order;
    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        cin >> processes[i].arrivalTime; //arrival time
        cin >> processes[i].burstTime; //burst time
    }
    sort(processes.begin(), processes.end(), sortByBurstTime);
    // for(int i = 0; i < n; ++i) {
    //     cout << processes[i].arrivalTime << ' ' << processes[i].burstTime << '\n';
    // }
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;
    processes[0].waitingTime = 0;
    order.push_back(processes[0].id);

    for (int i = 1; i < n; i++) {
        int waitingTime = 0;
        for (int j = 0; j < i; j++) {
            waitingTime += processes[j].burstTime;
        }
        processes[i].waitingTime = max(0, waitingTime - processes[i].arrivalTime);
        order.push_back(processes[i].id);
    }
    

    for (int i = 0; i < n; i++) {
        processes[i].turnaroundTime = processes[i].waitingTime + processes[i].burstTime;
    }

    for (int i = 0; i < n; i++) {
        totalWaitingTime += processes[i].waitingTime;
        totalTurnaroundTime += processes[i].turnaroundTime;
    }

    double averageWaitingTime = (1.0 * totalWaitingTime) / n;
    double averageTurnaroundTime = (1.0 * totalTurnaroundTime) / n;

    cout << "Average Waiting Time: " << averageWaitingTime << '\n';
    cout << "Average Turnaround Time: " << averageTurnaroundTime << '\n';
    cout << "Execution Order : ";
    for( auto val : order) {
       cout << "P" << val << ' ';
    }
    return 0;
}
