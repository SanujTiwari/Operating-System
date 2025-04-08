#include<iostream>
#include<algorithm>
#include<iomanip>
#include<queue>
using namespace std;

struct Process {
    int id, arrival, burst, priority, completion, tat, waiting, remaining;
};

Process proc[50];
int n, quantum;

void inputProcesses() {
    cout << "Enter number of processes: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        proc[i].id = i + 1;
        cout << "Enter Arrival Time, Burst Time, and Priority for Process " << i + 1 << ": ";
        cin >> proc[i].arrival >> proc[i].burst >> proc[i].priority;
        proc[i].remaining = proc[i].burst;
    }
}

void displayResults() {
    cout << "\nProcess\tAT\tBT\tCT\tTAT\tWT\n";
    for (int i = 0; i < n; i++) {
        proc[i].tat = proc[i].completion - proc[i].arrival;
        proc[i].waiting = proc[i].tat - proc[i].burst;
        cout << proc[i].id << "\t" << proc[i].arrival << "\t" << proc[i].burst << "\t" 
             << proc[i].completion << "\t" << proc[i].tat << "\t" << proc[i].waiting << endl;
    }
}

void calculateAvgTimes() {
    float totalTAT = 0, totalWT = 0;
    for (int i = 0; i < n; i++) {
        totalTAT += proc[i].tat;
        totalWT += proc[i].waiting;
    }
    cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << (totalTAT / n);
    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << (totalWT / n) << endl;
}

void fcfs() {
    sort(proc, proc + n, [](Process a, Process b) {
        return a.arrival < b.arrival;
    });

    int currentTime = 0;
    for (int i = 0; i < n; i++) {
        if (currentTime < proc[i].arrival)
            currentTime = proc[i].arrival;
        proc[i].completion = currentTime + proc[i].burst;
        currentTime = proc[i].completion;
    }
    displayResults();
    calculateAvgTimes();
}

void sjf() {
    sort(proc, proc + n, [](Process a, Process b) {
        return a.arrival < b.arrival || (a.arrival == b.arrival && a.burst < b.burst);
    });

    int currentTime = 0, completed = 0;
    while (completed < n) {
        int idx = -1, minBurst = 1e9;
        for (int i = 0; i < n; i++) {
            if (proc[i].arrival <= currentTime && proc[i].completion == 0) {
                if (proc[i].burst < minBurst) {
                    minBurst = proc[i].burst;
                    idx = i;
                }
            }
        }
        if (idx != -1) {
            proc[idx].completion = currentTime + proc[idx].burst;
            currentTime = proc[idx].completion;
            completed++;
        } else {
            currentTime++;
        }
    }
    displayResults();
    calculateAvgTimes();
}

void roundRobin() {
    cout << "Enter Time Quantum: ";
    cin >> quantum;

    queue<int> q;
    int currentTime = 0, completed = 0;

    for (int i = 0; i < n; i++)
        if (proc[i].arrival == 0)
            q.push(i);

    while (!q.empty()) {
        int i = q.front();
        q.pop();

        if (proc[i].remaining > quantum) {
            proc[i].remaining -= quantum;
            currentTime += quantum;
        } else {
            currentTime += proc[i].remaining;
            proc[i].remaining = 0;
            proc[i].completion = currentTime;
            completed++;
        }

        for (int j = 0; j < n; j++) {
            if (proc[j].arrival <= currentTime && proc[j].remaining > 0 && j != i)
                q.push(j);
        }

        if (proc[i].remaining > 0)
            q.push(i);
    }
    displayResults();
    calculateAvgTimes();
}

void priorityScheduling() {
    sort(proc, proc + n, [](Process a, Process b) {
        return a.arrival < b.arrival || (a.arrival == b.arrival && a.priority < b.priority);
    });

    int currentTime = 0, completed = 0;
    while (completed < n) {
        int idx = -1, highestPriority = 1e9;
        for (int i = 0; i < n; i++) {
            if (proc[i].arrival <= currentTime && proc[i].completion == 0) {
                if (proc[i].priority < highestPriority) {
                    highestPriority = proc[i].priority;
                    idx = i;
                }