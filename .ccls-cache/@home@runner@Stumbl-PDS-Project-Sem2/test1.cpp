// TEST for appending usernames to csv file on swipe L or R

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

class User {
public:
    string name;

    User(const string& username) : name(username) {}

    void swipeLeft(const string& swipedOnUsername) {
        appendSwipe(swipedOnUsername, "left_swipe.csv");  
    }

    void swipeRight(const string& swipedOnUsername) {
        appendSwipe(swipedOnUsername, "right_swipe.csv");  
    }

    void appendSwipe(const string& swipedOnUsername, const string& filename) {
        vector<string> lines;
        string line;
        bool updated = false;

        ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            cerr << "Failed to open file " << filename << endl;
            return;
        }

        while (getline(inputFile, line)) {
            size_t pos = line.find(name);
            if (pos != string::npos && (pos == 0 || line[pos-1] == ' ')) { 
                line += ", " + swipedOnUsername;  
                updated = true;
            }
            lines.push_back(line);
        }
        inputFile.close();

        if (!updated) {
            lines.push_back(name + ", " + swipedOnUsername);
        }

        ofstream outputFile(filename);
        if (!outputFile.is_open()) {
            cerr << "Failed to write to file " << filename << endl;
            return;
        }
        for (const string& outLine : lines) {
            outputFile << outLine << endl;
        }
        outputFile.close();
    }
};

int main() {
    User john("John");
    vector<string> profiles = {"Mary", "Rose", "Hillary", "Jack", "Daniel", "Chris", "Alice", "Bob", "Charlie", "David"};

    for (auto& profile : profiles) {
        cout << "Swipe left (L) or right (R) on " << profile << ": ";
        char decision;
        cin >> decision;
        if (decision == 'L' || decision == 'l') {
            john.swipeLeft(profile);
        } else {
            john.swipeRight(profile);
        }
    }

    

    system("cat left_swipe.csv");
    system("cat right_swipe.csv");

    return 0;
}
