#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

struct Process {
    char processName;
    int arrivalTime, burstTime, completionTime, waitingTime, turnaroundTime, priority;
    int status;
};

vector<Process> processQueue;
int processCount;

bool compareByPriority(const Process& p1, const Process& p2) {
    return p1.priority > p2.priority;
}

int main() {
    int currentTime = 0, totalBurstTime = 0, largest;
    char processID;
    float totalWaitingTime = 0, totalTurnaroundTime = 0, avgWaitingTime, avgTurnaroundTime;

    cin >> processCount;

    for (int i = 0; i < processCount; i++) {
        processID = 'A' + i;
        Process p;
        p.processName = processID;
        cin >> p.arrivalTime;
        cin >> p.burstTime;
        cin >> p.priority;

        p.status = 0;
        totalBurstTime += p.burstTime;
        processQueue.push_back(p);
    }

    sort(processQueue.begin(), processQueue.end(), compareByPriority);
    processQueue.back().priority = -9999;

    for (currentTime = processQueue[0].arrivalTime; currentTime < totalBurstTime;) {
        largest = processCount - 1;
        for (int i = 0; i < processCount; i++) {
            if (processQueue[i].arrivalTime <= currentTime && processQueue[i].status != 1 && processQueue[i].priority > processQueue[largest].priority) {
                largest = i;
            }
        }

        currentTime += processQueue[largest].burstTime;
        processQueue[largest].completionTime = currentTime;
        processQueue[largest].waitingTime = processQueue[largest].completionTime - processQueue[largest].arrivalTime - processQueue[largest].burstTime;
        processQueue[largest].turnaroundTime = processQueue[largest].completionTime - processQueue[largest].arrivalTime;
        processQueue[largest].status = 1;

        totalWaitingTime += processQueue[largest].waitingTime;
        totalTurnaroundTime += processQueue[largest].turnaroundTime;

    }

    avgWaitingTime = totalWaitingTime / processCount;
    avgTurnaroundTime = totalTurnaroundTime / processCount;

    cout << "\nAverage Waiting Time: " << avgWaitingTime << endl;
    cout << "Average Turnaround Time: " << avgTurnaroundTime << endl;

    return 0;
}
