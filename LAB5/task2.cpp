#include <iostream>
#include <string>
using namespace std;

struct Node {
    int jobID;
    string documentName;
    int pages;
    Node* next;
};

class PrinterQueue {
private:
    Node* front;
    Node* rear;

    void showJob(Node* job) {
        cout << job->jobID << " "
             << job->documentName << " "
             << job->pages << " pages" << endl;
    }

public:
    PrinterQueue() {
        front = nullptr;
        rear = nullptr;
    }

    bool isEmpty() {
        return front == nullptr;
    }

    void addJob(int id, string name, int pages) {
        Node* newNode = new Node{id, name, pages, nullptr};

        if (isEmpty()) {
            front = rear = newNode;
        }
        else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    void processJob() {
        if (isEmpty()) {
            cout << "Queue is empty. No job to process.\n";
            return;
        }

        Node* temp = front;

        cout << "Processing: ";
        showJob(temp);

        front = front->next;

        if (front == nullptr) {
            rear = nullptr;
        }

        delete temp;
    }

    void viewNextJob() {
        if (isEmpty()) {
            cout << "Queue is empty. No next job.\n";
            return;
        }

        cout << "Next job: ";
        showJob(front);
    }

    void displayQueue() {
        if (isEmpty()) {
            cout << "Queue is empty.\n";
            return;
        }

        Node* current = front;

        while (current != nullptr) {
            showJob(current);
            current = current->next;
        }
    }

    void countJobs() {
        int count = 0;
        Node* current = front;

        while (current != nullptr) {
            count++;
            current = current->next;
        }

        cout << "Waiting jobs: " << count << endl;
    }

    void clear() {
        while (front != nullptr) {
            Node* temp = front;
            front = front->next;
            delete temp;
        }

        rear = nullptr;
    }

    ~PrinterQueue() {
        clear();
    }
};

int main() {
    PrinterQueue queue;

    queue.addJob(101, "Assignment1.pdf", 10);
    queue.addJob(102, "Report.docx", 25);
    queue.addJob(103, "Notes.pdf", 5);
    queue.addJob(104, "LabTask.docx", 15);

    cout << "All jobs:\n";
    queue.displayQueue();
    queue.countJobs();

    cout << "\nProcess two jobs:\n";
    queue.processJob();
    queue.processJob();

    cout << "\nRemaining jobs:\n";
    queue.displayQueue();

    cout << "\nAdd a new job:\n";
    queue.addJob(105, "Project.pdf", 8);

    queue.viewNextJob();

    cout << "\nProcess all remaining jobs:\n";

    while (!queue.isEmpty()) {
        queue.processJob();
    }

    cout << "\nAttempt to process an empty queue:\n";
    queue.processJob();

    queue.clear();

    return 0;
}