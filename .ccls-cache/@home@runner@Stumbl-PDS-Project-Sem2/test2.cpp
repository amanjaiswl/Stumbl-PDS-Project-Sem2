// test for creating hashmap from csv

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <string>
#include <algorithm>  
#include <vector>

using namespace std;

unordered_map<string, int> loadUserSwipes(const string& currentUserName) {
    unordered_map<string, int> swipes;
    ifstream leftFile("left_swipe.csv"), rightFile("right_swipe.csv");
    string line;

    auto processLine = [&](ifstream& file, int swipeValue) {
        while (getline(file, line)) {
            stringstream ss(line);
            string user, swipedUser;
            getline(ss, user, ',');
            if (user == currentUserName) {
                while (getline(ss, swipedUser, ',')) {
                    swipedUser.erase(remove(swipedUser.begin(), swipedUser.end(), ' '), swipedUser.end());  
                    swipes[swipedUser] = swipeValue;  
                }
            }
        }
    };

    processLine(leftFile, 0);  
    processLine(rightFile, 1); 

    leftFile.close();
    rightFile.close();

    return swipes;
}

int main() {
    string currentUserName = "John";  // Assuming 'John' is the current logged-in user
    unordered_map<string, int> userSwipes = loadUserSwipes(currentUserName);

    cout << "Swipes for user " << currentUserName << ":\n";
    for (const auto& pair : userSwipes) {
        cout << "{" << pair.first << " :" << pair.second << "}\n";
    }

    return 0;
}
