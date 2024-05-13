#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <bitset>

using namespace std;

struct User {
    string name, pronouns, bio, password;
    int age, gender, mode, uniId, majorId;
    string studyDays, studyTimes;
    double ageWeight, genderWeight, modeWeight, uniWeight, majorWeight, daysWeight, timesWeight;
};


vector<User> loadUsers(const string& filename) {
    vector<User> users;
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        return users;
    }
    string line;
    getline(file, line); // Skip header if it exists

    while (getline(file, line)) {
        stringstream ss(line);
        User user;
        getline(ss, user.name, ',');
        getline(ss, user.pronouns, ',');
        getline(ss, user.bio, ',');
        getline(ss, user.password, ',');  // Preserving password for completeness
        ss >> user.age; ss.ignore();
        ss >> user.gender; ss.ignore();
        ss >> user.mode; ss.ignore();
        ss >> user.uniId; ss.ignore();
        ss >> user.majorId; ss.ignore();
        getline(ss, user.studyDays, ',');
        getline(ss, user.studyTimes, ',');
        ss >> user.ageWeight; ss.ignore();
        ss >> user.genderWeight; ss.ignore();
        ss >> user.modeWeight; ss.ignore();
        ss >> user.uniWeight; ss.ignore();
        ss >> user.majorWeight; ss.ignore();
        ss >> user.daysWeight; ss.ignore();
        ss >> user.timesWeight;
        users.push_back(user);
    }
    file.close();
    return users;
}

void displayProfile(const User& user) {
    cout << "Name: " << user.name << "\n"
         << "Pronouns: " << user.pronouns << "\n"
         << "Bio: " << user.bio << "\n"
         << "Age: " << user.age << "\n"
         << "Gender: " << user.gender << "\n"
         << "Mode: " << user.mode << "\n"
         << "University ID: " << user.uniId << "\n"
         << "Major ID: " << user.majorId << "\n"
         << "Study Days: " << user.studyDays << "\n"
         << "Study Times: " << user.studyTimes << "\n";
}


void updateWeights(User& currentUser, const User& profileUser, char swipe) {
    // Calculate differences that might influence weight adjustments
    int ageDifference = abs(currentUser.age - profileUser.age);

    // Converting string bitsets to actual bitset for comparison
    std::bitset<7> currentUserDays(currentUser.studyDays);
    std::bitset<7> profileUserDays(profileUser.studyDays);
    std::bitset<5> currentUserTimes(currentUser.studyTimes);
    std::bitset<5> profileUserTimes(profileUser.studyTimes);

    if (swipe == 'R') { // Right swipe increases weights for matching preferences
        if (currentUser.gender == profileUser.gender) {
            currentUser.genderWeight *= 1.05;
        }
        if (ageDifference <= 5) { // Consider a small age difference as compatible
            currentUser.ageWeight *= 1.05;
        }
        if (currentUser.mode == profileUser.mode) {
            currentUser.modeWeight *= 1.05;
        }
        if (currentUser.uniId == profileUser.uniId) {
            currentUser.uniWeight *= 1.05;
        }
        if (currentUser.majorId == profileUser.majorId) {
            currentUser.majorWeight *= 1.05;
        }
        if ((currentUserDays & profileUserDays).count() > 0) {
            currentUser.daysWeight *= 1.05;
        }
        if ((currentUserTimes & profileUserTimes).count() > 0) {
            currentUser.timesWeight *= 1.05;
        }
    } else if (swipe == 'L') { // Left swipe decreases weights for non-matching preferences
        if (currentUser.gender != profileUser.gender) {
            currentUser.genderWeight *= 0.95;
        }
        if (ageDifference > 5) {
            currentUser.ageWeight *= 0.95;
        }
        if (currentUser.mode != profileUser.mode) {
            currentUser.modeWeight *= 0.95;
        }
        if (currentUser.uniId != profileUser.uniId) {
            currentUser.uniWeight *= 0.95;
        }
        if (currentUser.majorId != profileUser.majorId) {
            currentUser.majorWeight *= 0.95;
        }
        if ((currentUserDays & profileUserDays).none()) {
            currentUser.daysWeight *= 0.95;
        }
        if ((currentUserTimes & profileUserTimes).none()) {
            currentUser.timesWeight *= 0.95;
        }
    }
}





void updateUserDataCSV(const string& filename, const vector<User>& users) {
    ofstream file(filename, ios::trunc); // Open file in truncate mode to overwrite
    if (!file) {
        cerr << "Failed to open file for writing: " << filename << endl;
        return;
    }

    //   headers 
    file << "Name,Pronouns,Bio,Password,Age,Gender,Mode,UniId,MajorId,StudyDays,StudyTimes,AgeWeight,GenderWeight,ModeWeight,UniWeight,MajorWeight,DaysWeight,TimesWeight\n";

    for (const User& user : users) {
        file << user.name << ","
             << user.pronouns << ","
             << user.bio << ","
             << user.password << ","
             << user.age << ","
             << user.gender << ","
             << user.mode << ","
             << user.uniId << ","
             << user.majorId << ","
             << user.studyDays << ","
             << user.studyTimes << ","
             << user.ageWeight << ","
             << user.genderWeight << ","
             << user.modeWeight << ","
             << user.uniWeight << ","
             << user.majorWeight << ","
             << user.daysWeight << ","
             << user.timesWeight << endl;
    }
    file.close();
}


int main() {
    vector<User> users = loadUsers("userData.csv");
    if (users.empty()) {
        cerr << "No users loaded from the file or file could not be opened." << endl;
        return 1;
    }

    User& currentUser = users[0];  
    cout << "Current User: " << currentUser.name << endl;

    for (int i = 1; i < users.size()-1; i++) {
        cout << "\nProfile " << i << ":\n";
        displayProfile(users[i]);
        cout << "Swipe Right (R) or Left (L) or Quit (Q): ";
        char decision;
        cin >> decision;

        if (decision == 'Q' || decision == 'q') {
            break;
        }

        updateWeights(currentUser, users[i], decision);
        updateUserDataCSV("userData.csv", users);

        cout << "\nUpdated Weights for " << currentUser.name << ":\n"
             << "Age Weight: " << currentUser.ageWeight << "\n"
             << "Gender Weight: " << currentUser.genderWeight << "\n"
             << "Mode Weight: " << currentUser.modeWeight << "\n"
             << "University Weight: " << currentUser.uniWeight << "\n"
             << "Major Weight: " << currentUser.majorWeight << "\n"
             << "Days Weight: " << currentUser.daysWeight << "\n"
             << "Times Weight: " << currentUser.timesWeight << "\n";
    }

    // Update user data in CSV file with new weights after the session
    // updateUserDataCSV("userData.csv", users);

    return 0;
}
