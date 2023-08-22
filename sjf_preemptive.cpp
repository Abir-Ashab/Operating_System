#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int remainingTime; 
    int waitingTime;
    int turnaroundTime;
};

bool sortByBurstTime(const Process &a, const Process &b) {
    return a.burstTime < b.burstTime;
}

bool sortByRemainingTime(const Process &a, const Process &b) {
    return a.remainingTime < b.remainingTime;
}

int main() {
    int n; 
    cin >> n;
    vector<Process> processes(n);

    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        cin >> processes[i].arrivalTime; //arrival time
        cin >> processes[i].burstTime; //burst time
        processes[i].remainingTime = processes[i].burstTime;
    }

    // Sort processes by burst time to handle tie-breakers
    sort(processes.begin(), processes.end(), sortByBurstTime);
    vector<int> executionOrder;
    
    int currentTime = 0;
    int completedProcesses = 0;
    
    while (completedProcesses < n) {
        int shortestJobIndex = -1;
        int shortestBurstTime = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime < shortestBurstTime && processes[i].remainingTime > 0) {
                shortestJobIndex = i;
                shortestBurstTime = processes[i].remainingTime;
            }
        }

        if (shortestJobIndex == -1) {
            currentTime++;
        } else {
            processes[shortestJobIndex].remainingTime--;
            currentTime++;

            if (processes[shortestJobIndex].remainingTime == 0) {
                completedProcesses++;
                processes[shortestJobIndex].waitingTime = currentTime - processes[shortestJobIndex].arrivalTime -
                                                        processes[shortestJobIndex].burstTime;
                processes[shortestJobIndex].turnaroundTime = processes[shortestJobIndex].waitingTime +
                                                            processes[shortestJobIndex].burstTime;
                executionOrder.push_back(processes[shortestJobIndex].id);
            }
        }
    }
    
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;
    
    for (int i = 0; i < n; i++) {
        totalWaitingTime += processes[i].waitingTime;
        totalTurnaroundTime += processes[i].turnaroundTime;
    }

    double averageWaitingTime = (1.0 * totalWaitingTime) / n;
    double averageTurnaroundTime = (1.0 * totalTurnaroundTime) / n;

    cout << "Average Waiting Time: " << averageWaitingTime << '\n';
    cout << "Average Turnaround Time: " << averageTurnaroundTime << '\n';

    cout << "Execution Order:";
    for (int i = 0; i < executionOrder.size(); i++) {
        cout << " P" << executionOrder[i];
    }
    cout << '\n';

    return 0;
}
