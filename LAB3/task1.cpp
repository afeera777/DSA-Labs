#include <cctype>
#include <iostream>
#include <string>

bool isPalindrome(const std::string& text) {
    if (text.empty()) return true;
    std::size_t left = 0;
    std::size_t right = text.size() - 1;

    while (left < right) {
        unsigned char first = static_cast<unsigned char>(text[left]);
        unsigned char last = static_cast<unsigned char>(text[right]);
        if (!std::isalnum(first)) { ++left; continue; }
        if (!std::isalnum(last)) { --right; continue; }
        if (std::tolower(first) != std::tolower(last)) return false;
        ++left;
        --right;
    }
    return true;
}

int main() {
    std::string text;
    std::cout << "Enter a string: ";
    std::getline(std::cin, text);
    std::cout << (isPalindrome(text) ? "It is a palindrome.\n"
                                     : "It is not a palindrome.\n");
}