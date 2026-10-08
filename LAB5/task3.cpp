#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <cmath>
using namespace std;

template <typename T>
class Stack {
  struct Node {
    T data;
    Node* next;
  };
  Node* head;

public:
  Stack() {
    head = nullptr;
  }

  bool isEmpty() {
    return head == nullptr;
  }

  void push(T value) {
    head = new Node{value, head};
  }

  bool pop(T& value) {
    if (isEmpty()) {
      return false;
    }
    Node* temp = head;
    value = head->data;
    head = head->next;
    delete temp;
    return true;
  }

  bool top(T& value) {
    if (isEmpty()) {
      return false;
    }
    value = head->data;
    return true;
  }

  void clear() {
    T value;
    while (pop(value)) {
    }
  }

  ~Stack() {
    clear();
  }
};

bool isOperator(char ch) {
  return ch == '+' || ch == '-' || ch == '*' ||
         ch == '/' || ch == '%';
}

int precedence(char ch) {
  if (ch == '*' || ch == '/' || ch == '%') {
    return 2;
  }
  if (ch == '+' || ch == '-') {
    return 1;
  }
  return 0;
}

bool convert(string expression, string& postfix, string& error) {
  Stack<char> s;
  postfix = "";
  bool needNumber = true;
  bool hasNumber = false;

  for (int i = 0; i < (int)expression.length(); i++) {
    char ch = expression[i];

    if (isspace(static_cast<unsigned char>(ch))) {
      continue;
    }

    if (isdigit(static_cast<unsigned char>(ch))) {
      if (!needNumber) {
        error = "Missing operator";
        return false;
      }
      while (i < (int)expression.length() &&
             isdigit(static_cast<unsigned char>(expression[i]))) {
        postfix += expression[i];
        i++;
      }
      i--;
      postfix += " ";
      needNumber = false;
      hasNumber = true;
    }
    else if (ch == '(') {
      if (!needNumber) {
        error = "Missing operator";
        return false;
      }
      s.push(ch);
    }
    else if (ch == ')') {
      char op;
      bool found = false;

      while (s.pop(op)) {
        if (op == '(') {
          found = true;
          break;
        }
        postfix += op;
        postfix += " ";
      }
      if (!found) {
        error = "Mismatched parentheses";
        return false;
      }
      if (needNumber) {
        error = "Invalid expression";
        return false;
      }
      needNumber = false;
    }
    else if (isOperator(ch)) {
      if (needNumber) {
        error = "Missing number";
        return false;
      }
      char op;

      while (s.top(op) && op != '(' &&
             precedence(op) >= precedence(ch)) {
        s.pop(op);
        postfix += op;
        postfix += " ";
      }
      s.push(ch);
      needNumber = true;
    }
    else {
      error = "Invalid character";
      return false;
    }
  }

  char op;
  while (s.pop(op)) {
    if (op == '(') {
      error = "Mismatched parentheses";
      return false;
    }
    postfix += op;
    postfix += " ";
  }

  if (!hasNumber) {
    error = "Empty or invalid expression";
    return false;
  }
  if (needNumber) {
    error = "Missing number at the end";
    return false;
  }
  return true;
}

bool evaluate(string postfix, double& result, string& error) {
  Stack<double> s;
  stringstream input(postfix);
  string token;

  while (input >> token) {
    if (token.length() == 1 && isOperator(token[0])) {
      double a, b;

      if (!s.pop(b) || !s.pop(a)) {
        error = "Invalid expression";
        return false;
      }
      char op = token[0];

      if (op == '/' && b == 0) {
        error = "Division by zero";
        return false;
      }
      if (op == '%' && b == 0) {
        error = "Modulus by zero";
        return false;
      }

      switch (op) {
        case '+': s.push(a + b); break;
        case '-': s.push(a - b); break;
        case '*': s.push(a * b); break;
        case '/': s.push(a / b); break;
        case '%': s.push(fmod(a, b)); break;
      }
    }
    else {
      double number;
      stringstream value(token);

      if (!(value >> number)) {
        error = "Invalid number";
        return false;
      }
      s.push(number);
    }
  }

  if (!s.pop(result) || !s.isEmpty()) {
    error = "Invalid expression";
    return false;
  }
  return true;
}

int main() {
  string expression, postfix, error;
  double result;

  cout << "Enter expression: ";
  getline(cin, expression);

  if (!convert(expression, postfix, error)) {
    cout << error << endl;
    return 0;
  }

  cout << "Postfix: " << postfix << endl;

  if (!evaluate(postfix, result, error)) {
    cout << error << endl;
    return 0;
  }

  cout << "Answer: " << result << endl;
  return 0;
}