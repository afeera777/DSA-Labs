#include <iostream>
#include <string>
#include <utility>

class StringPool {
private:
    std::string* stringPool; // dynamically allocated array.
    int currentSize;
    int maxSize;

public:
    StringPool() : stringPool(new std::string[5]), currentSize(0), maxSize(5) {}

    // prevent two pools from owning the same dynamic array.
    StringPool(const StringPool&) = delete;
    StringPool& operator=(const StringPool&) = delete;

    ~StringPool() { delete[] stringPool; }

    bool addString(const std::string& value) {
        if (currentSize == maxSize) return false;
        stringPool[currentSize++] = value;
        return true;
    }

    bool removeString(const std::string& value) {
        for (int i = 0; i < currentSize; ++i) {
            if (stringPool[i] == value) {
                // value held by the now-unused array element.
                for (int j = i; j < currentSize - 1; ++j)
                    stringPool[j] = std::move(stringPool[j + 1]);
                stringPool[currentSize - 1].clear();
                --currentSize;
                return true;
            }
        }
        return false;
    }

    void displayStatus() const {
        std::cout << "Pool status (" << currentSize << "/" << maxSize << "): ";
        if (currentSize == 0) std::cout << "empty";
        for (int i = 0; i < currentSize; ++i)
            std::cout << (i ? ", " : "") << stringPool[i];
        std::cout << '\n';
    }
};

int main() {
    StringPool pool;
    pool.addString("Apple");
    pool.addString("Banana");
    pool.addString("Cherry");
    pool.displayStatus();

    pool.removeString("Banana");
    pool.displayStatus();
    pool.removeString("Apple");
    pool.displayStatus();

 
}
