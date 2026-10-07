#include <iostream>
#include <string>
using namespace std;

struct Bit {
    int bit;
    Bit *next, *prev;
    Bit(int b) : bit(b), next(nullptr), prev(nullptr) {}
};

class BinaryDLL {
public:
    Bit *head, *tail;
    int size;

    BinaryDLL() : head(nullptr), tail(nullptr), size(0) {}
    BinaryDLL(const BinaryDLL& o) : head(nullptr), tail(nullptr), size(0) {
        for (Bit* p = o.head; p; p = p->next) pushBack(p->bit);
    }
    BinaryDLL& operator=(const BinaryDLL& o) {
        if (this != &o) { clear(); for (Bit* p = o.head; p; p = p->next) pushBack(p->bit); }
        return *this;
    }
    ~BinaryDLL() { clear(); }

    void clear() {
        while (head) { Bit* t = head; head = head->next; delete t; }
        tail = nullptr; size = 0;
    }
    void pushBack(int b) {
        Bit* n = new Bit(b);
        if (!tail) head = tail = n;
        else { tail->next = n; n->prev = tail; tail = n; }
        size++;
    }
    void pushFront(int b) {
        Bit* n = new Bit(b);
        if (!head) head = tail = n;
        else { n->next = head; head->prev = n; head = n; }
        size++;
    }
    void popFront() {
        Bit* t = head; head = head->next;
        if (head) head->prev = nullptr; else tail = nullptr;
        delete t; size--;
    }
    // pad with leading zeros to a multiple of 8 bits
    void padTo8() { while (size % 8 != 0 || size == 0) pushFront(0); }
    // pad with leading zeros to exactly n bits
    void padTo(int n) { while (size < n) pushFront(0); }

    // store binary number
    bool store(const string& s) {
        clear();
        for (char c : s) {
            if (c != '0' && c != '1') { clear(); return false; }
            pushBack(c - '0');
        }
        padTo8();
        return true;
    }

    void display() const {
        int i = 0;
        for (Bit* p = head; p; p = p->next, i++) {
            if (i && i % 8 == 0) cout << ' ';   // show 8-bit blocks
            cout << p->bit;
        }
        cout << '\n';
    }

    // ones complement 
    void onesComplement() {
        for (Bit* p = head; p; p = p->next) p->bit ^= 1;
    }

    // addition (traverse from LSB to MSB using prev pointers)
    static BinaryDLL add(const BinaryDLL& a, const BinaryDLL& b) {
        BinaryDLL r;
        Bit *pa = a.tail, *pb = b.tail;
        int carry = 0;
        while (pa || pb) {
            int s = carry + (pa ? pa->bit : 0) + (pb ? pb->bit : 0);
            r.pushFront(s & 1);
            carry = s >> 1;
            if (pa) pa = pa->prev;
            if (pb) pb = pb->prev;
        }
        if (carry) r.pushFront(1);
        r.padTo8();
        return r;
    }

    //  two's complement = one's complement + 1 (same width, overflow dropped)
    void twosComplement() {
        int w = size;
        onesComplement();
        BinaryDLL one; one.store("1");
        BinaryDLL r = add(*this, one);
        while (r.size > w) r.popFront();   // discard carry out of the top bit
        *this = r;
    }

    // shift left by 1 (append a 0 at LSB end)
    void shiftLeft() { pushBack(0); }

    // multiplication
    static BinaryDLL multiply(const BinaryDLL& a, const BinaryDLL& b) {
        BinaryDLL result; result.store("0");
        BinaryDLL shifted = a;                     // a << i
        for (Bit* p = b.tail; p; p = p->prev) {    // scan multiplier from LSB
            if (p->bit) result = add(result, shifted);
            shifted.shiftLeft();
        }
        result.padTo8();
        return result;
    }

    // decimal conversion
    unsigned long long toDecimal() const {
        unsigned long long v = 0;
        for (Bit* p = head; p; p = p->next) v = v * 2 + p->bit;
        return v;
    }
    // signed interpretation for convenience
    long long toSigned() const {
        if (!head || head->bit == 0 || size > 63) return (long long)toDecimal();
        BinaryDLL t = *this; t.twosComplement();
        return -(long long)t.toDecimal();
    }
};

static BinaryDLL readNumber(const char* label) {
    BinaryDLL n; string s;
    while (true) {
        cout << label; cin >> s;
        if (n.store(s)) return n;
        cout << "Invalid input - use only 0 and 1.\n";
    }
}

int main() {
    int choice;
    do {
        cout << "\n BINARY ARITHMETIC \n"
             << "1. Store & display a binary number\n2. One's complement\n"
             << "3. Two's complement\n4. Binary addition\n5. Binary multiplication\n"
             << "6. Convert to decimal\n0. Exit\nChoice: ";
        if (!(cin >> choice)) break;
        switch (choice) {
        case 1: { BinaryDLL a = readNumber("Enter binary: "); cout << "Stored: "; a.display(); break; }
        case 2: { BinaryDLL a = readNumber("Enter binary: "); cout << "Original:      "; a.display();
                  a.onesComplement(); cout << "1's complement: "; a.display(); break; }
        case 3: { BinaryDLL a = readNumber("Enter binary: "); cout << "Original:      "; a.display();
                  a.twosComplement(); cout << "2's complement: "; a.display(); break; }
        case 4: { BinaryDLL a = readNumber("First number:  "), b = readNumber("Second number: ");
                  BinaryDLL r = BinaryDLL::add(a, b);
                  cout << "Sum: "; r.display(); cout << "Decimal: " << r.toDecimal() << '\n'; break; }
        case 5: { BinaryDLL a = readNumber("First number:  "), b = readNumber("Second number: ");
                  BinaryDLL r = BinaryDLL::multiply(a, b);
                  cout << "Product: "; r.display(); cout << "Decimal: " << r.toDecimal() << '\n'; break; }
        case 6: { BinaryDLL a = readNumber("Enter binary: ");
                  cout << "Unsigned decimal: " << a.toDecimal()
                       << "\nSigned (2's complement) decimal: " << a.toSigned() << '\n'; break; }
        case 0: cout << "Goodbye!\n"; break;
        default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
    return 0;
}