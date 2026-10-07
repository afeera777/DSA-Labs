#include <iostream>
#include <vector>
using namespace std;

struct Person {
    int id;
    Person* next;
    Person(int i) : id(i), next(nullptr) {}
};

// build circle 1 to N ...returns pointer to the last node 
Person* createCircle(int n) {
    Person *first = new Person(1), *last = first;
    for (int i = 2; i <= n; i++) {
        last->next = new Person(i);
        last = last->next;
    }
    last->next = first;              // close the circle
    return last;
}

int main() {
    int n, k;
    cout << "Enter number of people (N): "; cin >> n;
    cout << "Enter step count (k): ";        cin >> k;
    if (n < 1 || k < 1) { cout << "N and k must be positive.\n"; return 1; }

    Person* prev = createCircle(n);  // prev always trails the current person
    vector<int> order;

    while (prev->next != prev) {     // more than one person left
        for (int i = 1; i < k; i++)  // move to the k-th person
            prev = prev->next;
        Person* victim = prev->next;
        order.push_back(victim->id);
        prev->next = victim->next;   // unlink
        delete victim;               // free memory
    }
    int survivor = prev->id;
    delete prev;

    cout << "Elimination order: ";
    for (size_t i = 0; i < order.size(); i++)
        cout << order[i] << (i + 1 < order.size() ? " -> " : "");
    cout << "\nSurvivor: Person " << survivor << endl;
    return 0;
}