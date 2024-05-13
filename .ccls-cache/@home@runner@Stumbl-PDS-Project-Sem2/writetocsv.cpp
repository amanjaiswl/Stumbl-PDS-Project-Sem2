#include <bits/stdc++.h>

const string lswipeFile = "left_swipe.csv";
const string rswipeFile = "right_swipe.csv";

void User::swipeLeft(const string& swipedOnUsername) {
    appendSwipe(swipedOnUsername, lswipeFile);  
}

void User::swipeRight(const string& swipedOnUsername) {
    appendSwipe(swipedOnUsername, rswipeFile);  
}


void User::appendSwipe(const string& swipedOnUsername, const string& filename) {
    vector<string> lines;
    string line, newLine;
    bool updated = false;

    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Failed to open file " << filename << endl;
        return;
    }

    while (getline(inputFile, line)) {
        size_t pos = line.find(name);
        if (pos != string::npos && (pos == 0 || line[pos-1] == ',')) { 
            line += ", " + swipedOnUsername;  
            updated = true;
        }
        lines.push_back(line);
    }
    inputFile.close();

  // If the user's name wasn't found, add a new line
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

// ------------------------------------
if (userDecision == 'L') {  
    currentUser->swipeLeft(currentProfile.getName());
} else if (userDecision == 'R') {  
    currentUser->swipeRight(currentProfile.getName());
}
