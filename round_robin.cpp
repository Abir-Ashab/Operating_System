#include <bits/stdc++.h>
using namespace std;

struct Process
{
    int pid;          
    int start_time;  
    int completion_time; 
    int turnaround_time; 
    int waiting_time;   
    int burst_time;   
    int arrival_time;   
    int remaining_time;
};

bool compareArrivalTime(Process a, Process b)
{
    return a.arrival_time < b.arrival_time;
}

bool compareProcessID(Process a, Process b)
{
    return a.pid < b.pid;
}

int main()
{
    int n, time_quantum, count = 0, current_time = 0;
    float average_turnaround_time = 0, average_waiting_time = 0, total_turnaround_time = 0, total_waiting_time = 0;

    // Input number of processes and time quantum
    cin >> n;
    cin >> time_quantum;

    Process processes[n];
    vector<int> burst_time(n + 1, 0);

    // Input process arrival times and burst times
    for (int i = 0; i < n; i++)
    {
        cin >> processes[i].arrival_time;
        cin >> processes[i].burst_time;
        processes[i].pid = i + 1;
        burst_time[processes[i].pid] = processes[i].burst_time;
    }

    // Sort processes by arrival time
    sort(processes, processes + n, compareArrivalTime);

    queue<int> ready_queue;
    vector<int> marked(n, false);
    ready_queue.push(0);
    marked[0] = true;

    while (count != n)
    {
        int index = ready_queue.front();
        ready_queue.pop();

        if (burst_time[processes[index].pid] == processes[index].burst_time)
        {
            processes[index].start_time = max(current_time, processes[index].arrival_time);
            current_time = processes[index].start_time;
        }

        if (burst_time[processes[index].pid] - time_quantum > 0)
        {
            current_time += time_quantum;
            burst_time[processes[index].pid] -= time_quantum;
        }
        else
        {
            current_time += burst_time[processes[index].pid];
            burst_time[processes[index].pid] = 0;
            count++;

            processes[index].completion_time = current_time;
            processes[index].turnaround_time = processes[index].completion_time - processes[index].arrival_time;
            processes[index].waiting_time = processes[index].turnaround_time - processes[index].burst_time;

            total_waiting_time += processes[index].waiting_time;
            total_turnaround_time += processes[index].turnaround_time;
        }

        for (int i = 1; i < n; i++)
        {
            int id = processes[i].pid;
            if (processes[i].arrival_time <= current_time && !marked[i] && burst_time[id] > 0)
            {
                marked[i] = true;
                ready_queue.push(i);
            }
        }

        if (burst_time[processes[index].pid] > 0)
        {
            ready_queue.push(index);
        }

        if (ready_queue.empty())
        {
            for (int i = 1; i < n; i++)
            {
                int id = processes[i].pid;
                if (burst_time[id] > 0)
                {
                    marked[i] = true;
                    ready_queue.push(i);
                }
            }
        }
    }

    average_waiting_time = (total_waiting_time / n);
    average_turnaround_time = (total_turnaround_time / n);

    sort(processes, processes + n, compareProcessID);

    cout << "\nAverage Waiting Time: " << average_waiting_time;
    cout << "\nAverage Turnaround Time: " << average_turnaround_time;

    return 0;
}
