#include <iostream>
#include <vector>
#include <unordered_map>
#include <conio.h>
#include <cctype>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <queue>

using namespace std;

// Function Declarations
void mainMenu(), processMenu(), algorithmMenu();
void createProcess(), editProcess(), deleteProcess(), viewProcesses();
void firstComeFirstServe(), shortestJobFirst(), shortestRemainingTime(), priorityScheduling(), roundRobin();
void terminateProgram(), printProcesses(const vector<vector<string>>& list), cloneProcesses(), resetTemporary(), initializeMenus();
void validateName(const string& name), validateNumber(const string& num), checkForEmptyList();

vector<vector<string>> processList, temporaryList;
unordered_map<char, void(*)()> mainMenuMap, processMenuMap, algorithmMenuMap;

char userChoice;
string input;

int main() {
    initializeMenus();
    mainMenu();
    return 0;
}

// Menu Functions
void mainMenu() {
    while (true) {
        system("cls");
        cout << "===================================\n";
        cout << "        SCHEDULER SIMULATOR        \n";
        cout << "===================================\n";
        cout << "[1] Process Management\n";
        cout << "[2] Algorithm Testing\n";
        cout << "[3] Exit Program\n";
        cout << "===================================\n";
        cout << "Enter your choice: ";
        cin >> userChoice;

        if (mainMenuMap.find(userChoice) != mainMenuMap.end()) {
            mainMenuMap[userChoice]();
        }
        else {
            cout << "Invalid choice. Please try again.";
            _getch();
        }
    }
}

void processMenu() {
    while (true) {
        system("cls");
        cout << "===================================\n";
        cout << "        PROCESS MANAGEMENT         \n";
        cout << "===================================\n";
        cout << "[1] Create Process\n";
        cout << "[2] Edit Process\n";
        cout << "[3] Delete Process\n";
        cout << "[4] View All Processes\n";
        cout << "[5] Return to Main Menu\n";
        cout << "===================================\n";
        cout << "Enter your choice: ";
        cin >> userChoice;

        if (processMenuMap.find(userChoice) != processMenuMap.end()) {
            processMenuMap[userChoice]();
        }
        else {
            cout << "Invalid choice. Please try again.";
            _getch();
        }
    }
}

void algorithmMenu() {
    checkForEmptyList();
    while (true) {
        system("cls");
        cout << "===================================\n";
        cout << "        ALGORITHM TESTING          \n";
        cout << "===================================\n";
        cout << "[1] First Come First Serve\n";
        cout << "[2] Shortest Job First\n";
        cout << "[3] Shortest Remaining Time\n";
        cout << "[4] Priority Scheduling\n";
        cout << "[5] Round Robin\n";
        cout << "[6] Return to Main Menu\n";
        cout << "===================================\n";
        cout << "Enter your choice: ";
        cin >> userChoice;

        if (algorithmMenuMap.find(userChoice) != algorithmMenuMap.end()) {
            algorithmMenuMap[userChoice]();
        }
        else {
            cout << "Invalid choice. Please try again.";
            _getch();
        }
    }
}

// Process Management Functions
void createProcess() {
    string name, burst, arrival, priority;

    cout << "------------------------------\n"
        << "Create a Process\n"
        << "------------------------------\n"
        << "Enter Process Name: ";
    cin >> name;
    validateName(name);
    cout << "Enter Burst Time: ";
    cin >> burst;
    validateNumber(burst);
    cout << "Enter Arrival Time: ";
    cin >> arrival;
    validateNumber(arrival);
    cout << "Enter Priority Level: ";
    cin >> priority;
    validateNumber(priority);

    processList.push_back({ name, burst, arrival, priority });
    cout << "Process \"" << name << "\" has been saved.\n";
    _getch();
    system("cls");
    processMenu();
}

void editProcess() {
    checkForEmptyList();

    string name, burst, arrival, priority;
    char confirm;

    cout << "------------------------------\n"
        << "Edit a Process\n"
        << "------------------------------\n"
        << "Enter process name to edit: ";
    cin >> name;

    auto it = find_if(processList.begin(), processList.end(), [&name](const vector<string>& process) {
        return process[0] == name;
        });

    if (it != processList.end()) {
        cout << "Confirm edit? (Y/N): ";
        cin >> confirm;
        confirm = toupper(confirm);

        if (confirm == 'Y') {
            cout << "Enter new Burst Time: ";
            cin >> burst;
            validateNumber(burst);
            cout << "Enter new Arrival Time: ";
            cin >> arrival;
            validateNumber(arrival);
            cout << "Enter new Priority Level: ";
            cin >> priority;
            validateNumber(priority);

            (*it)[1] = burst;
            (*it)[2] = arrival;
            (*it)[3] = priority;
            cout << "Process \"" << name << "\" has been updated.\n";
        }
    }
    else {
        cout << "Process not found!\n";
    }
    _getch();
    system("cls");
    processMenu();
}

void deleteProcess() {
    checkForEmptyList();

    string name;
    char confirm;

    cout << "------------------------------\n"
        << "Delete Processes\n"
        << "------------------------------\n"
        << "1. Delete One\n"
        << "2. Delete All\n"
        << "3. Return\n"
        << "------------------------------\n";
    cin >> userChoice;
    userChoice = toupper(userChoice);

    switch (userChoice) {
    case '1': {
        cout << "Enter process name to delete: ";
        cin >> name;
        auto it = find_if(processList.begin(), processList.end(), [&name](const vector<string>& process) {
            return process[0] == name;
            });
        if (it != processList.end()) {
            cout << "Confirm delete? (Y/N): ";
            cin >> confirm;
            confirm = toupper(confirm);
            if (confirm == 'Y') {
                processList.erase(it);
                cout << "Process \"" << name << "\" has been deleted.\n";
            }
        }
        else {
            cout << "Process not found!\n";
        }
        break;
    }
    case '2': {
        cout << "Confirm delete all processes? (Y/N): ";
        cin >> confirm;
        confirm = toupper(confirm);
        if (confirm == 'Y') {
            processList.clear();
            cout << "All processes have been deleted.\n";
        }
        break;
    }
    case '3':
        system("cls");
        processMenu();
        return;
    default:
        cout << "Invalid option. Please try again.\n";
        break;
    }
    _getch();
    system("cls");
    processMenu();
}

void viewProcesses() {
    checkForEmptyList();

    cout << "------------------------------------------------------------\n"
        << "View All Processes\n"
        << "------------------------------------------------------------\n";
    printProcesses(processList);
    cout << "------------------------------------------------------------\n";
    _getch();
    system("cls");
    processMenu();
}

void firstComeFirstServe() {
    cloneProcesses();
    sort(temporaryList.begin(), temporaryList.end(), [](const vector<string>& a, const vector<string>& b) {
        return stoi(a[2]) < stoi(b[2]);
    });
    cout << "\nExecuting FCFS Scheduling:\n";
    printProcesses(temporaryList);

    _getch(); // Pause before clearing the screen
    resetTemporary();
}

void shortestJobFirst() {
    cloneProcesses();
    sort(temporaryList.begin(), temporaryList.end(), [](const vector<string>& a, const vector<string>& b) {
        return stoi(a[1]) < stoi(b[1]);
    });
    cout << "\nExecuting SJF Scheduling:\n";
    printProcesses(temporaryList);

    _getch(); // Pause before clearing the screen
    resetTemporary();
}
void shortestRemainingTime() {
    cloneProcesses();
    priority_queue<pair<int, vector<string>>, vector<pair<int, vector<string>>>, greater<>> pq;
    for (auto& process : temporaryList)
        pq.push({ stoi(process[1]), process });

    cout << "\nExecuting SRTF Scheduling:\n";
    while (!pq.empty()) {
        auto process = pq.top(); pq.pop();
        cout << "Process: " << process.second[0] << " (Burst: " << process.first << ")\n";
    }

    _getch(); // Pause before clearing the screen
    resetTemporary();
}

void priorityScheduling() {
    cloneProcesses();
    sort(temporaryList.begin(), temporaryList.end(), [](const vector<string>& a, const vector<string>& b) {
        return stoi(a[3]) < stoi(b[3]);
    });
    cout << "\nExecuting Priority Scheduling:\n";
    printProcesses(temporaryList);

    _getch(); // Pause before clearing the screen
    resetTemporary();
}

void roundRobin() {
    cloneProcesses();
    int timeQuantum;
    cout << "Enter Time Quantum: ";
    cin >> timeQuantum;

    queue<vector<string>> q;
    for (auto& process : temporaryList)
        q.push(process);

    cout << "\nExecuting Round Robin Scheduling:\n";
    while (!q.empty()) {
        auto process = q.front(); q.pop();
        int burst = stoi(process[1]);
        if (burst > timeQuantum) {
            burst -= timeQuantum;
            process[1] = to_string(burst);
            q.push(process);
        }
        cout << "Process: " << process[0] << " (Remaining Burst: " << burst << ")\n";
    }

    _getch(); // Pause before clearing the screen
    resetTemporary();
}


// Utility Functions
void initializeMenus() {
    mainMenuMap['1'] = processMenu;
    mainMenuMap['2'] = algorithmMenu;
    mainMenuMap['3'] = terminateProgram;

    processMenuMap['1'] = createProcess;
    processMenuMap['2'] = editProcess;
    processMenuMap['3'] = deleteProcess;
    processMenuMap['4'] = viewProcesses;
    processMenuMap['5'] = mainMenu;

    algorithmMenuMap['1'] = firstComeFirstServe;
    algorithmMenuMap['2'] = shortestJobFirst;
    algorithmMenuMap['3'] = shortestRemainingTime;
    algorithmMenuMap['4'] = priorityScheduling;
    algorithmMenuMap['5'] = roundRobin;
    algorithmMenuMap['6'] = mainMenu;
}


void validateName(const string& name) {
    auto it = find_if(processList.begin(), processList.end(), [&name](const vector<string>& process) {
        return process[0] == name;
        });
    if (it != processList.end()) {
        cout << "Process name already exists. Please choose another name.\n";
        _getch();
        system("cls");
        createProcess();
    }
}

void validateNumber(const string& num) {
    if (any_of(num.begin(), num.end(), [](char c) { return !isdigit(c); })) {
        cout << "Invalid input. Please enter a valid number.\n";
        _getch();
        system("cls");
        createProcess();
    }
}

void terminateProgram() {
    exit(0);
}

void printProcesses(const vector<vector<string>>& list) {
    int width = 15;
    cout << left << setw(width) << "Process Name" << setw(width) << "Burst Time" << setw(width) << "Arrival Time" << setw(width) << "Priority" << endl;
    for (const auto& process : list) {
        for (const auto& field : process) {
            cout << setw(width) << field;
        }
        cout << endl;
    }
}

void checkForEmptyList() {
    if (processList.empty()) {
        cout << "No processes found. Please create at least one process.\n";
        _getch();
        system("cls");
        mainMenu();
    }
}

void cloneProcesses() {
    temporaryList = processList;
}

void resetTemporary() {
    temporaryList.clear();
}