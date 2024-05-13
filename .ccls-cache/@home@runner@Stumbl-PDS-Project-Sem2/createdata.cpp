#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <bitset>
#include <vector>

using namespace std;

void populateCsv(int num) {
    ofstream file("userData.csv", ios::app);

    // Header for userData.csv
    file << "name,pronouns,bio,password,age,gender,mode,uniId,majorId,studyDays,studyTimes,ageWeight,genderWeight,modeWeight,uniWeight,majorWeight,daysWeight,timesWeight\n";

    string names[] = {"Alice", "Bob", "Charlie", "David", "Eve", "Frank", "Grace", "Helen", "Ian", "Jane", "Kyle", "Laura", "Mason", "Nina", "Oscar", "Paula", "Quinn", "Rosa", "Steve", "Tina"};
    string pronouns[] = {"he/him", "she/her", "they/them"};
    string bios[] = {"Loves coding", "Enjoys sports", "Likes reading", "Fan of movies", "Music enthusiast"};

    srand((unsigned)time(0)); // Seed for random number generation

    for (int i = 0; i < num; i++) {
        string name = names[i];
        string pronoun = pronouns[rand() % 3];
        string bio = bios[rand() % 5];
        string password = "pass" + to_string(rand() % 9999);
        int age = 18 + rand() % 5; // Random age between 18 and 22
        int gender = rand() % 3; // 0, 1, or 2
        int mode = rand() % 2; // 0 or 1
        int uniId = rand() % 8; // Assuming there are 8 universities
        int majorId = rand() % 5; // Assuming there are 5 majors

        // Generating random bitsets for study days and times
        bitset<7> studyDays(rand() % 128); // Random 7-bit binary number
        bitset<5> studyTimes(rand() % 32); // Random 5-bit binary number

        // Random weights between 1 and 10
        int ageWeight = rand() % 10 + 1;
        int genderWeight = rand() % 10 + 1;
        int modeWeight = rand() % 10 + 1;
        int uniWeight = rand() % 10 + 1;
        int majorWeight = rand() % 10 + 1;
        int daysWeight = rand() % 10 + 1;
        int timesWeight = rand() % 10 + 1;

        // Writing data to CSV
        file << name << "," << pronoun << "," << bio << "," << password << ",";
        file << age << "," << gender << "," << mode << "," << uniId << "," << majorId << ",";
        file << studyDays << "," << studyTimes << ",";
        file << ageWeight << "," << genderWeight << "," << modeWeight << ",";
        file << uniWeight << "," << majorWeight << "," << daysWeight << "," << timesWeight << endl;
    }

    file.close();
    cout << "Data successfully populated." << endl;
}

int main() {
    populateCsv(20); // Create data for 20 users
    return 0;
}
