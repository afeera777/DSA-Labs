#include <iostream>
#include <limits>

struct Node {
    int data;
    Node* next;
};

void insertAtHead(Node*& head, int value) {
    head = new Node{value, head};
}

void insertAtThird(Node*& head, int value) {
    if (head == nullptr || head->next == nullptr) {
        std::cout << "At least two nodes are needed for position 3.\n";
        return;
    }
    Node* second = head->next;
    second->next = new Node{value, second->next};
}

void displayList(const Node* head) {
    for (const Node* current = head; current != nullptr; current = current->next)
        std::cout << current->data << " -> ";
    std::cout << "NULL\n";
}

void deleteLast(Node*& head) {
    if (head == nullptr) {
        std::cout << "The list is empty.\n";
        return;
    }
    if (head->next == nullptr) {
        delete head;
        head = nullptr;
    } else {
        Node* current = head;
        while (current->next->next != nullptr) current = current->next;
        delete current->next;
        current->next = nullptr;
    }
    std::cout << "Updated list: ";
    displayList(head);
}

int countNodes(const Node* head) {
    int count = 0;
    for (const Node* current = head; current != nullptr; current = current->next)
        ++count;
    return count;
}

void reverseList(Node*& head) {
    Node* previous = nullptr;
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        current->next = previous;
        previous = current;
        current = next;
    }
    head = previous;
    std::cout << "Reversed list: ";
    displayList(head);
}

void searchValue(const Node* head, int value) {
    int position = 1; // Positions are 1-based; the head is position 1.
    for (const Node* current = head; current != nullptr; current = current->next, ++position) {
        if (current->data == value) {
            std::cout << value << " found at position " << position << ".\n";
            return;
        }
    }
    std::cout << value << " was not found.\n";
}

void destroyList(Node*& head) {
    while (head != nullptr) {
        Node* next = head->next;
        delete head;
        head = next;
    }
}

bool readInteger(const char* prompt, int& value) {
    std::cout << prompt;
    if (std::cin >> value) return true;
    if (std::cin.eof()) return false;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Please enter an integer.\n";
    return false;
}

int main() {
    Node* head = nullptr;
    while (true) {
        std::cout << "\n1. Insert at head\n2. Insert at 3rd position\n"
                     "3. Display list\n4. Delete last\n5. Count nodes\n"
                     "6. Reverse list\n7. Search value\n8. Exit\n";
        int choice;
        if (!readInteger("Choose an option: ", choice)) {
            if (std::cin.eof()) break;
            continue;
        }
        int value;
        switch (choice) {
            case 1:
                if (readInteger("Value: ", value)) { insertAtHead(head, value); displayList(head); }
                break;
            case 2:
                if (readInteger("Value: ", value)) { insertAtThird(head, value); displayList(head); }
                break;
            case 3: displayList(head); break;
            case 4: deleteLast(head); break;
            case 5: std::cout << "Node count: " << countNodes(head) << '\n'; break;
            case 6: reverseList(head); break;
            case 7:
                if (readInteger("Value to search: ", value)) searchValue(head, value);
                break;
            case 8: destroyList(head); std::cout << "Goodbye.\n"; return 0;
            default: std::cout << "Choose a number from 1 to 8.\n";
        }
        if (std::cin.eof()) break;
    }
    destroyList(head);
}
