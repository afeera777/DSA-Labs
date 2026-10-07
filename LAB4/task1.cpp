#include <iostream>
#include <string>
#include <limits>
using namespace std;

struct Song {
    int id;
    string name;
    int minutes, seconds;
    Song *prev, *next;
    Song(int i, const string& n, int m, int s)
        : id(i), name(n), minutes(m), seconds(s), prev(nullptr), next(nullptr) {}
};

class Playlist {
    Song *head, *tail, *current;   // current = song being "played"

    Song* find(int id) const {
        for (Song* p = head; p; p = p->next)
            if (p->id == id) return p;
        return nullptr;
    }
    static void print(const Song* s) {
        cout << "  [" << s->id << "] " << s->name << " ("
             << s->minutes << ":" << (s->seconds < 10 ? "0" : "") << s->seconds << ")\n";
    }
public:
    Playlist() : head(nullptr), tail(nullptr), current(nullptr) {}
    ~Playlist() {
        while (head) { Song* t = head; head = head->next; delete t; }
    }

    //add song at end
    void addSong(int id, const string& name, int m, int s) {
        if (find(id)) { cout << "Song ID already exists.\n"; return; }
        if (s < 0 || s > 59 || m < 0) { cout << "Invalid duration.\n"; return; }
        Song* n = new Song(id, name, m, s);
        if (!head) head = tail = current = n;
        else { tail->next = n; n->prev = tail; tail = n; }
        cout << "Song added.\n";
    }

    // delete by ID
    void deleteSong(int id) {
        Song* p = find(id);
        if (!p) { cout << "Song not found.\n"; return; }
        if (p == current) current = p->next ? p->next : p->prev;
        if (p->prev) p->prev->next = p->next; else head = p->next;
        if (p->next) p->next->prev = p->prev; else tail = p->prev;
        delete p;
        cout << "Song deleted.\n";
    }

    // 3. Forward display
    void displayForward() const {
        if (!head) { cout << "Playlist is empty.\n"; return; }
        cout << "Playlist (first -> last):\n";
        for (Song* p = head; p; p = p->next) print(p);
    }

    // 4. Backward display
    void displayBackward() const {
        if (!tail) { cout << "Playlist is empty.\n"; return; }
        cout << "Playlist (last -> first):\n";
        for (Song* p = tail; p; p = p->prev) print(p);
    }

    // 5. Search by ID
    void search(int id) const {
        Song* p = find(id);
        if (p) { cout << "Found:\n"; print(p); }
        else cout << "Song not found.\n";
    }

    // 6. Player controls
    void nowPlaying() const {
        if (!current) { cout << "Nothing to play.\n"; return; }
        cout << "Now playing:\n"; print(current);
    }
    void playNext() {
        if (!current) { cout << "Playlist is empty.\n"; return; }
        if (!current->next) { cout << "End of playlist - no next song.\n"; return; }
        current = current->next; nowPlaying();
    }
    void playPrevious() {
        if (!current) { cout << "Playlist is empty.\n"; return; }
        if (!current->prev) { cout << "Start of playlist - no previous song.\n"; return; }
        current = current->prev; nowPlaying();
    }

    // 7. Reverse in place by swapping prev/next of every node
    void reverse() {
        if (!head) { cout << "Playlist is empty.\n"; return; }
        Song* p = head;
        while (p) {
            Song* t = p->prev;
            p->prev = p->next;
            p->next = t;
            p = p->prev;            // old next
        }
        Song* t = head; head = tail; tail = t;
        cout << "Playlist reversed.\n";
    }
};

int main() {
    Playlist pl;
    int choice;
    do {
        cout << "\n===== PLAYLIST MENU =====\n"
             << "1. Add Song\n2. Delete Song\n3. Display Forward\n4. Display Backward\n"
             << "5. Search Song\n6. Play Next\n7. Play Previous\n8. Now Playing\n"
             << "9. Reverse Playlist\n0. Exit\nChoice: ";
        if (!(cin >> choice)) break;
        int id, m, s; string name;
        switch (choice) {
        case 1:
            cout << "ID: "; cin >> id;
            cout << "Name: "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); getline(cin, name);
            cout << "Duration (minutes seconds): "; cin >> m >> s;
            pl.addSong(id, name, m, s); break;
        case 2: cout << "ID to delete: "; cin >> id; pl.deleteSong(id); break;
        case 3: pl.displayForward(); break;
        case 4: pl.displayBackward(); break;
        case 5: cout << "ID to search: "; cin >> id; pl.search(id); break;
        case 6: pl.playNext(); break;
        case 7: pl.playPrevious(); break;
        case 8: pl.nowPlaying(); break;
        case 9: pl.reverse(); break;
        case 0: cout << "Goodbye!\n"; break;
        default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
    return 0;
}