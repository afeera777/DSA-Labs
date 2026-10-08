#include <iostream>
#include <string>
using namespace std;

struct Node {
    char data;
    Node* next;
};

class Stack {
private:
    Node* head;

public:
    Stack() {
        head = nullptr;
    }

    bool isEmpty() {
        return head == nullptr;
    }

    void push(char value) {
        Node* newNode = new Node{value, head};
        head = newNode;
    }

    void pop() {
        if (isEmpty()) {
            cout << "Stack is empty.\n";
            return;
        }

        Node* temp = head;
        head = head->next;
        delete temp;
    }

    char top() {
        if (isEmpty()) {
            cout << "Stack is empty.\n";
            return '\0';
        }

        return head->data;
    }

    void display() {
        if (isEmpty()) {
            cout << "Stack is empty.\n";
            return;
        }

        Node* current = head;

        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }

        cout << endl;
    }

    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }

    bool checkBalance(string expression) {
        clear();

        for (char ch : expression) {
            if (ch == '(' || ch == '[' || ch == '{') {
                push(ch);
            }
            else if (ch == ')' || ch == ']' || ch == '}') {
                if (isEmpty()) {
                    return false;
                }

                char opening = top();

                bool matched =
                    (opening == '(' && ch == ')') ||
                    (opening == '[' && ch == ']') ||
                    (opening == '{' && ch == '}');

                if (!matched) {
                    clear();
                    return false;
                }

                pop();
            }
        }

        bool balanced = isEmpty();
        clear();
        return balanced;
    }

    ~Stack() {
        clear();
    }
};

int main() {
    Stack stack;

    string expressions[] = {
        "(A+B)",
        "{A+[B*C]}",
        "(A+B]",
        "((A+B)",
        "{[()]}",
        "A+B*C",
        "([A+B])",
        ""
    };

    for (string expression : expressions) {
        cout << "Expression: ";

        if (expression.empty()) {
            cout << "(empty)";
        }
        else {
            cout << expression;
        }

        if (stack.checkBalance(expression)) {
            cout << " is balanced\n";
        }
        else {
            cout << " is not balanced\n";
        }
    }

    return 0;
}