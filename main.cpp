#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <cstdlib>
#include <limits>
#include <algorithm>


using namespace std;

const string userDataFile = "userData.csv";
const string passwordFile = "password.csv";
const string lswipeFile = "left_swipe.csv";
const string rswipeFile = "right_swipe.csv";
const string uni_list = "uni_list.csv";
const string major_list = "major_list.csv";

unordered_map<string, int> dayToBitMap = {
    {"Monday", 6},
    {"Tuesday", 5},
    {"Wednesday", 4},
    {"Thursday", 3},
    {"Friday", 2},
    {"Saturday", 1},
    {"Sunday", 0}
};
const string days_list[] = {"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};

unordered_map<string, int> timeToBitMap = {
    {"Morning",0}, {"Afternoon",1}, {"Evening",2}, {"Night",3}, {"Late Night",4}
};
const string time_list[] = {"Morning","Afternoon","Evening","Night","Late Night"};


unordered_map<string,int> uni_map {
    {"Plaksha",0},
    {"Ashoka",1},
    {"UIUC",2},
    {"UCLA",3},
    {"LSR",4},
    {"SRCC",5},
    {"NIFT",6},
    {"Purdue",7}
};

unordered_map<string,int> major_map {
    {"Computer Engineering",0},
    {"RCPS",1},
    {"CSAI",2},
    {"Civil Engineering",4}
};



int timeToBit(const string& time) {
    if (timeToBitMap.find(time) != timeToBitMap.end()) {
        return timeToBitMap[time];
    } else {
        throw invalid_argument("time bad");
    }
}

bitset<5> timesToBitset(const string& timesList) {
    bitset<5> result;
    istringstream stream(timesList);
    string time;

    // Extract times from the stream, trimming spaces
    while (getline(stream, time, ',')) {
        time.erase(time.find_last_not_of(" \t\n\r\f\v") + 1); // Trim whitespace
        result.set(timeToBit(time)); // Set the bit for the corresponding time
    }

    return result;
}

int dayToBit(const string& day) {
    if (dayToBitMap.find(day) != dayToBitMap.end()) {
        return dayToBitMap[day];
    } else {
        throw invalid_argument("day no exis");
    }
}

// Convert that into big fat bitset
bitset<7> daysToBitset(const string& daysList) {
    bitset<7> result;
    istringstream stream(daysList);
    string day;

    // Taking text from the stream, removes commas, and puts in day
    while (getline(stream, day, ',')) {
        day.erase(day.find_last_not_of(" \t\n\r\f\v") + 1); // Trims whitespace
        result.set(dayToBit(day)); // Set the correct bit position
    }

    return result; 
}


class User {
    protected:

        //Basic Info
        string name;
        string pronouns;
        string bio;

        //secret secret
        string password;

        //User Profile
        int age;
        int gender;
        int mode; 
        int uniId;
        int majorId;
        bitset<7> studyDays;
        bitset<5> studyTimes;

        //User Preferences
        int ageWeight;
        int genderWeight;
        int modeWeight;
        int uniWeight;
        int majorWeight;
        int daysWeight;
        int timesWeight;

    public:
        User(); // no values given
        User(string, string, string, string, int, int, int, int, int, bitset<7>, bitset<5>); //only bio data
        User(string, string, string, string, int, int, int, int, int, bitset<7>, bitset<5>, int, int, int, int, int, int, int); // all weights
        User(string, string, string, int, int, int, int, int, bitset<7>, bitset<5>, int, int, int, int, int, int, int); //passwordless

        string getName();
        string getPronouns();
        string getBio();

        int getAge() const;
        int getGender() const;
        int getMode() const; 
        int getUniId() const;
        int getMajorId() const;
        bitset<7> getStudyDays() const;
        bitset<5> getStudyTimes() const;

        int getAgeWeight() const;
        int getGenderWeight() const;
        int getModeWeight() const;
        int getUniWeight() const;
        int getMajorWeight() const;
        int getDaysWeight() const;
        int getTimesWeight() const;

        static User loadUser(string);
        static User login();
        void logout();
        static User registerUser();
        static bool userexists(string);
        void writeToCSV();

        void updateWeights(User& otherUser, bool swipe);

        void swipeLeft(User&, unordered_map<string,bool>&);
        void swipeRight(User&, unordered_map<string,bool>&);
        string toString();
};

class Centroid : public User {
    public:
        Centroid() : User() {
            name = "";
            pronouns = "";
            bio = "";
            password = "";

            age = 1 + rand() % 100;
            gender = rand() % 3;
            mode = rand() % 2;
            uniId = rand() % 10;
            majorId = rand() % 10;
            for (int j = 0; j < 7; ++j) {
                studyDays[j] = rand() % 2;
            }
            for (int j = 0; j < 5; ++j) {
                studyTimes[j] = rand() % 2;
            }
            ageWeight = 1 + rand() % 1000; 
            genderWeight = 1 + rand() % 1000;
            modeWeight = 1 + rand() % 1000;
            uniWeight = 1 + rand() % 1000;
            majorWeight = 1 + rand() % 1000;
            daysWeight = 1 + rand() % 1000;
            timesWeight = 1 + rand() % 1000;
        };

        void setValues(int, int, int, int, int, bitset<7>, bitset<5>, int, int, int, int, int, int, int);
};

User::User() {
    name = "Test User 0";
    pronouns = "Rather Not Say";
    bio = "";

    age = 0;
    gender = 0;
    mode = 0;
    uniId = 0;
    majorId = 0;

    ageWeight = 200;
    genderWeight = 200;
    modeWeight = 600;
    uniWeight = 400;
    majorWeight = 500;
    daysWeight = 800;
    timesWeight = 800;
}

User::User(string name, string password, string pronouns, string bio, int age, int gender, int mode, int uniId, int majorId, bitset<7> studyDays, bitset<5> studyTimes) {
    this->name = name;
    this->pronouns = pronouns;
    this->bio = bio;

    this->password = password;

    this->age = age;
    this->gender = gender;
    this->mode = mode;
    this->uniId = uniId;
    this->majorId = majorId;
    this->studyDays = studyDays;
    this->studyTimes = studyTimes;

    ageWeight = 200;
    genderWeight = 200;
    modeWeight = 600;
    uniWeight = 400;
    majorWeight = 500;
    daysWeight = 800;
    timesWeight = 800;
}

User::User(string name, string password, string pronouns, string bio, int age, int gender, int mode, int uniId, int majorId, bitset<7> studyDays, bitset<5> studyTimes, int ageWeight, int genderWeight, int modeWeight, int uniWeight, int majorWeight, int daysWeight, int timesWeight) {
    this->name = name;
    this->pronouns = pronouns;
    this->bio = bio;

    this->password = password;

    this->age = age;
    this->gender = gender;
    this->mode = mode;
    this->uniId = uniId;
    this->majorId = majorId;
    this->studyDays = studyDays;
    this->studyTimes = studyTimes;

    this->ageWeight = ageWeight;
    this->genderWeight = genderWeight;
    this->modeWeight = modeWeight;
    this->uniWeight = uniWeight;
    this->majorWeight = majorWeight;
    this->daysWeight = daysWeight;
    this->timesWeight = timesWeight;
};

User::User(string name, string pronouns, string bio, int age, int gender, int mode, int uniId, int majorId, bitset<7> studyDays, bitset<5> studyTimes, int ageWeight, int genderWeight, int modeWeight, int uniWeight, int majorWeight, int daysWeight, int timesWeight) {
    this->name = name;
    this->pronouns = pronouns;
    this->bio = bio;

    this->password = "";

    this->age = age;
    this->gender = gender;
    this->mode = mode;
    this->uniId = uniId;
    this->majorId = majorId;
    this->studyDays = studyDays;
    this->studyTimes = studyTimes;

    this->ageWeight = ageWeight;
    this->genderWeight = genderWeight;
    this->modeWeight = modeWeight;
    this->uniWeight = uniWeight;
    this->majorWeight = majorWeight;
    this->daysWeight = daysWeight;
    this->timesWeight = timesWeight;
};

string User::getName() {return this->name;}
string User::getPronouns() {return this->pronouns;}
string User::getBio() {return this->bio;}

int User::getAge() const {return this->age;}
int User::getGender() const {return this->gender;}
int User::getMode() const {return this->mode;}
int User::getUniId() const {return this->uniId;}
int User::getMajorId() const {return this->majorId;}
bitset<7> User::getStudyDays() const {return this->studyDays;}
bitset<5> User::getStudyTimes() const {return this->studyTimes;}

int User::getAgeWeight() const {return this->ageWeight;}
int User::getGenderWeight() const {return this->genderWeight;}
int User::getModeWeight() const {return this->modeWeight;}
int User::getUniWeight() const {return this->uniWeight;}
int User::getMajorWeight() const {return this->majorWeight;}
int User::getDaysWeight() const {return this->daysWeight;}
int User::getTimesWeight() const {return this->timesWeight;}

User User::registerUser() {
    string name, password, pronouns, bio, inp;
    int age, gender, mode, uniId, majorId;
    bitset<5> studyTimes;
    bitset<7> studyDays;

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter name: ";
    getline(cin, name);
    while (userexists(name)) {
        cout << "Username already exists, try again: ";
        getline(cin, name);
    }


    cout << "\nEnter password: ";
    getline(cin,password);

    cout << "\nEnter pronouns: ";
    getline(cin, pronouns);

    // Input sanitation for pronouns
    while (pronouns.empty() || pronouns.find(',') != std::string::npos) {
        if (pronouns.find(',') != std::string::npos) {
            cout << "Commas are not allowed in pronouns. Please enter pronouns without commas: ";
        } else {
            cout << "Invalid input. Please enter pronouns: ";
        }
        getline(cin, pronouns);
    }

    cout << "\nEnter bio: ";
    getline(cin, bio);

    // Input sanitation for bio
    while (bio.empty() || bio.find(',') != std::string::npos) {
        if (bio.find(',') != std::string::npos) {
            cout << "Commas are not allowed in bio. Please enter bio without commas: ";
        } else {
            cout << "Invalid input. Please enter bio: ";
        }
        getline(cin, bio);
    }

    age = -1;
    while (age < 0) {
        cout << "\nEnter age: ";
        cin >> age;
        if (age < 0) {
            cout << "Invalid input. Please enter a valid age: ";
        }
    }

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    gender = -1;
    while (gender == -1) {
        cout << "\nEnter gender (M/F/NB): ";
        cin >> inp;
        if (inp == "m" || inp == "male" || inp == "M") { 
            gender = 0; 
        } else if (inp == "f" || inp == "female" || inp == "F") {
            gender = 1;
        } else if (inp == "nb" || inp == "non-binary" || inp == "NB") {
            gender = 2;
        } else {cout << "Invalid input. Please enter M, F, or NB.\n";}
    }

    mode = -1;
    while (mode == -1) {
        cout << "\nEnter mode (Online/Offline): ";
        cin >> inp;
        //tolower(inp);
        if (inp == "online" || inp == "on") {mode = 0; }
        else if (inp == "offline" || inp == "off") {mode = 1;}
        else {cout << "Invalid input. Please enter Online or Offline.\n";}
    }


    cout << "\nEnter University Name: ";
    cin >> inp;
    uniId = uni_map[inp];

    cout << "\nEnter Major Name: ";
    cin >> inp;
    majorId = major_map[inp];

    cout << "\nEnter study days per week: ";
    inp = "";
    for (int i = 0; i < 7; i++) {
        char response;

        cout << "Do you study on " << days_list[i] << "? (y/n): ";
        cin >> response;

        if (response == 'y' || response == 'Y') {
            if (!inp.empty()) {
                inp += ",";
            }
            inp += days_list[i];
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

    }
    studyDays = daysToBitset(inp);


    cout << "\nEnter study times per day: ";
    inp = "";
    for (int i = 0; i < 5; i++) {
        char response;

        cout << "Do you study during the " << time_list[i] << "? (y/n): ";
        cin >> response;

        if (response == 'y' || response == 'Y') {
            if (!inp.empty()) {
                inp += ",";
            }
            inp += time_list[i];
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

    }
    studyTimes = timesToBitset(inp);

    User newUser(name, password, pronouns, bio, age, gender, mode, uniId, majorId, studyDays, studyTimes);
    newUser.writeToCSV();
    return newUser;
}

void User::writeToCSV() {
    ofstream file(userDataFile, ios::app);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << userDataFile << " for writing." << endl;
        return;
    }
    file << name << ",";
    file << pronouns << ",";
    file << bio << ",";
    file << age << ",";
    file << gender << ",";
    file << mode << ",";
    file << uniId << ",";
    file << majorId << ",";
    file << studyDays << ",";
    file << studyTimes << ",";

    file << ageWeight << ",";
    file << genderWeight << ",";
    file << modeWeight << ",";
    file << uniWeight << ",";
    file << majorWeight << ",";
    file << daysWeight << ",";
    file << timesWeight;

    file << endl;
    file.close();


    ofstream file3(passwordFile, ios::app);
    if (!file3.is_open()) {
        cerr << "Error: Could not open file " << passwordFile << " for writing." << endl;
        return;
    }
    file3 << name << ",";
    file3 << password << ",";
    file3 << '\n';
    file3.close();
}

string User::toString() {
    string userString = "Name: " + name + "\n";
    userString += "Pronouns: " + pronouns + "\n";
    userString += "Bio: " + bio + "\n";
    userString += "Age: " + to_string(age) + "\n";

    userString += "Gender: ";
    switch (gender) {
        case 0:
            userString += "Male\n";
            break;

        case 1:
            userString += "Female\n";
            break;

        case 2:
            userString += "Non Binary\n";
            break;
    }

    userString += "Mode: ";
    switch (mode)
    {
    case 0:
        userString += "Online\n";
        break;

    case 1:
        userString += "Offline\n";
        break;
    }

    userString += "Uni ID: " + to_string(uniId) + "\n";
    userString += "Major ID: " + to_string(majorId) + "\n";

    userString += "Study Days: " + studyDays.to_string() + "\n";
    userString += "Study Times: " + studyTimes.to_string() + "\n";
    return userString;
}

void User::swipeLeft(User& viewUser, unordered_map<string,bool>& map) {
    map[viewUser.getName()] = false;
    this->updateWeights(viewUser, false);
};
void User::swipeRight(User& viewUser, unordered_map<string,bool>& map) {
    map[viewUser.getName()] = true;
    this->updateWeights(viewUser, true);
};

User User::loadUser(string username) {
    ifstream file(userDataFile);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                string tempUsername,temp;
                getline(ss, tempUsername, ','); 

                if (tempUsername == username) {
                    vector<string> output;
                    string temp;
                    while (getline(ss, temp,',')) {
                        output.push_back(temp);
                    }
                    file.close();
                    return User(tempUsername, output[0], output[1], stoi(output[2]), stoi(output[3]), stoi(output[4]), stoi(output[5]), stoi(output[6]), bitset<7>(output[7]), bitset<5>(output[8]), stoi(output[9]), stoi(output[10]), stoi(output[11]), stoi(output[12]), stoi(output[13]),stoi(output[14]),stoi(output[15]));
                }
            }
            cout << "User not found in file." << endl;
        } else {
            cout << "Unable to open user data file." << endl;
        }
        // Return an empty user if not found
        file.close();
        return User();
}

User User::login() {
    string inputUsername, inputPassword;

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter Username: ";
    getline(cin, inputUsername);

    cout << "\nEnter Password: ";
    getline(cin, inputPassword);

    ifstream file(passwordFile);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                string tempUsername, tempPassword, temp;
                getline(ss, tempUsername, ','); 
                getline(ss, tempPassword, ','); 

                // Remove leading and trailing whitespaces from username and password
                tempUsername.erase(0, tempUsername.find_first_not_of(" \t\n\r\f\v"));
                tempUsername.erase(tempUsername.find_last_not_of(" \t\n\r\f\v") + 1);

                if (tempUsername != inputUsername) {continue;}
                tempPassword.erase(0, tempPassword.find_first_not_of(" \t\n\r\f\v"));
                tempPassword.erase(tempPassword.find_last_not_of(" \t\n\r\f\v") + 1);

                if (tempPassword == inputPassword) {
                    // Username and password matched, return the user
                    file.close();
                    User a = loadUser(tempUsername);
                    cout << a.toString();
                    return a;
                }
            }
            cout << "Username or password is incorrect.\n";
            throw;
        } else {
            cout << "Unable to open the password file.\n";
        }
        // Return an empty user if login fails
        file.close();
        return User();
}

void User::logout() {};

void Centroid::setValues(int age, int gender, int mode, int uniId, int majorId, bitset<7> days, bitset<5> times, int ageWeight, int genderWeight, int modeWeight, int uniWeight, int majorWeight, int daysWeight, int timesWeight) {
    this->age = age;
    this->gender = gender;
    this->mode = mode;
    this->uniId = uniId;
    this->majorId = majorId;
    this->studyDays = days;
    this->studyTimes = times;

    this->ageWeight = ageWeight;
    this->genderWeight = genderWeight;
    this->modeWeight = modeWeight;
    this->uniWeight = uniWeight;
    this->majorWeight = majorWeight;
    this->daysWeight = daysWeight;
    this->timesWeight = timesWeight;
}

bool User::userexists(string name){
    ifstream f(passwordFile);
    if (!f.is_open()){
        cout<<"Error opening file"<<endl;
        return true;
    }
    vector<string> users;
    int numUsers=0;
    if (f.is_open()) {
        string line;
        while (getline(f, line)) {
            string entries; // Change the type of entries to vector<string>
            size_t pos = line.find(',');
            if (pos != string::npos) {
                string entry = line.substr(0, pos);
                entries=entry; // Use push_back to add entry to entries vector
            } else {
                entries=line; // If there is no delimiter, push the entire line
            }
            users.push_back(entries);
            numUsers++;
        }
        f.close();
    }
    for(int i=0;i<numUsers;i++){
        if(users[i]==name){
            return true;
        }
    }
    return false;
}

void User::updateWeights(User& otherUser, bool swipeRight) {
    if (swipeRight) {
        if (gender == otherUser.getGender()) {genderWeight *= 1.05;}
        if (abs(age - otherUser.getAge()) <= 5) {ageWeight *= 1.05;}
        if (mode == otherUser.getMode()) {modeWeight *= 1.05;}
        if (uniId == otherUser.getUniId()) {uniWeight *= 1.05;}
        if (majorId == otherUser.getMajorId()) {majorWeight *= 1.05;}
        if ((studyDays & otherUser.getStudyDays()).count() > 0) {daysWeight *= 1.05;}
        if ((studyTimes & otherUser.getStudyTimes()).count() > 0) {timesWeight *= 1.05;}
    } else {
        if (gender != otherUser.getGender()) {genderWeight *= 0.95;}
        if (abs(age - otherUser.getAge()) > 5) {ageWeight *= 0.95;}
        if (mode != otherUser.getMode()) {modeWeight *= 0.95;}
        if (uniId != otherUser.getUniId()) {uniWeight *= 0.95;}
        if (majorId != otherUser.getMajorId()) {majorWeight *= 0.95;}
        if ((studyDays & otherUser.getStudyDays()).none() > 0) {daysWeight *= 0.95;}
        if ((studyTimes & otherUser.getStudyTimes()).none() > 0) {timesWeight *= 0.95;}
    }
}

//populate the csv with random data
void populateCsv(int num) {
    string names[] = {
        "Emma", "Liam", "Olivia", "Noah", "Ava", "William", "Sophia", "James", "Isabella", "Oliver",
        "Charlotte", "Benjamin", "Amelia", "Elijah", "Mia", "Lucas", "Harper", "Mason", "Evelyn", "Logan",
        "Abigail", "Alexander", "Emily", "Michael", "Madison", "Jackson", "Elizabeth", "Ethan", "Avery",
        "Daniel", "Sofia", "Henry", "Chloe", "Joseph", "Grace", "Samuel", "Aubrey", "David", "Ellie",
        "Carter", "Scarlett", "Wyatt", "Zoe", "John", "Hannah", "Jack", "Natalie", "Luke", "Lily",
        "Owen", "Brooklyn", "Dylan", "Addison", "Matthew", "Victoria", "Grayson", "Leah", "Gabriel",
        "Audrey", "Anthony", "Samantha", "Leo", "Claire", "Lincoln", "Anna", "Jaxon", "Isabelle",
        "Asher", "Stella", "Christopher", "Nora", "Joshua", "Lucy", "Andrew", "Maya", "Theodore",
        "Aria", "Caleb", "Elena", "Ryan", "Penelope", "Nathan", "Hazel", "Miles", "Gabriella",
        "Sebastian", "Alice", "Eli", "Kinsley", "Jeremiah", "Madeline", "Brayden", "Ellie", "Jordan",
        "Peyton", "Colton", "Lillian", "Ian", "Addison", "Dominic", "Isabel", "Xavier", "Ivy",
        "Adam", "Layla", "Jayden", "Kylie", "Justin", "Emilia", "Julian", "Harmony", "Landon",
        "Alexa", "Robert", "Adeline", "Easton", "Alexandra", "Carson", "Molly", "Ezra", "Zoey",
        "Brooks", "Luna", "Micah", "Eva", "Charles", "Jasmine", "Harrison", "Gianna", "Nolan",
        "Eleanor", "Bryson", "Elise", "Jordan", "Nova", "Nicholas", "Ruby", "Parker", "Leila",
        "Nathaniel", "Margaret", "Jace", "Lucia", "Dylan", "Juliana", "Sawyer", "Sara", "Gavin",
        "Violet", "Wyatt", "Rebecca", "Roman", "Camila", "Elias", "Brielle", "Jaxson", "Willow",
        "Greyson", "Aurora", "Levi", "Hannah", "Austin", "CorA", "Everett", "Madelyn", "Jonah",
        "Clara", "Brantley", "Kaylee", "Josiah", "Delilah", "Maxwell", "Sophie", "Axel", "Olga",
        "Xander", "Catherine", "Chase", "Valentina", "Cole", "Piper", "Bentley", "Liliana", "Jude",
        "Lyla", "Braxton", "Adalynn", "Tristan", "Jade", "Dominic", "Natalia", "Emmett", "Danielle",
        "Knox", "Rylee", "Bennett", "Lydia"};

    string passwords[] = {"password1", "password2", "password3", "password4", "password5"};
    string pronouns[] = {"she/her", "he/him", "they/them"};
    string bios[] = {"I love coding!", "Music is my passion.", "I'm a bookworm.", "I enjoy outdoor activities."};
    int ages[] = {18, 19, 20, 21, 22};
    int genders[] = {0, 1, 2};
    int modes[] = {0, 1};
    int uniIds[] = {1, 2, 3, 4, 5};
    int majorIds[] = {101, 102, 103, 104, 105};

    std::bitset<7> studyDaysBitset;
    std::bitset<5> studyTimesBitset;

    int randomNameIndex, randomPasswordIndex, randomPronounsIndex, randomBioIndex, randomAgeIndex, randomGenderIndex, randomModeIndex, randomUniIdIndex, randomMajorIdIndex, randomAgeWeight, randomGenderWeight, randomModeWeight, randomUniWeight, randomMajorWeight, randomDaysWeight, randomTimesWeight;

    for (int i = 0; i < num; i++) {
        randomNameIndex = i;
        randomPasswordIndex = rand() % 5;
        randomPronounsIndex = rand() % 3;
        randomBioIndex = rand() % 4;
        randomAgeIndex = rand() % 5;
        randomGenderIndex = rand() % 3;
        randomModeIndex = rand() % 2;
        randomUniIdIndex = rand() % 5;
        randomMajorIdIndex = rand() % 5;
        randomAgeWeight = rand() % 1000 + 1; 
        randomGenderWeight = rand() % 1000 + 1;
        randomModeWeight = rand() % 1000 + 1;
        randomUniWeight = rand() % 1000 + 1;
        randomMajorWeight = rand() % 1000 + 1;
        randomDaysWeight = rand() % 1000 + 1;
        randomTimesWeight = rand() % 1000 + 1;

        // Generating random bitsets
        for (int j = 0; j < 7; ++j) {
            studyDaysBitset[j] = rand() % 2;
        }

        for (int j = 0; j < 5; ++j) {
            studyTimesBitset[j] = rand() % 2;
        }

        User temp(names[randomNameIndex], passwords[randomPasswordIndex], pronouns[randomPronounsIndex], bios[randomBioIndex], ages[randomAgeIndex], genders[randomGenderIndex], modes[randomModeIndex], uniIds[randomUniIdIndex], majorIds[randomMajorIdIndex], studyDaysBitset, studyTimesBitset, randomAgeWeight, randomGenderWeight, randomModeWeight, randomUniWeight, randomMajorWeight, randomDaysWeight, randomTimesWeight);
        temp.writeToCSV();
    }
}

//removes all data from the csv
void depopulateCsv() {
    ofstream file1(userDataFile);
    file1.close();
    ofstream file2(passwordFile);
    file2.close();
}

int findAgeScore(int myAge, int otherAge, int ageWeight) {
    if (myAge - otherAge == 0) {
        return ageWeight;
    }
    return ageWeight / abs(myAge - otherAge);
}

int findGenderScore(const int& myGender, const int& otherGender, int genderWeight) {
    return (myGender == otherGender) ? genderWeight : 0;
}

int findModeScore(const int& myMode, const int& otherMode, int modeWeight) {
    return (myMode == otherMode) ? modeWeight : 0;
}

int findUniScore(const int& myUni, const int& otherUni, int uniWeight) {
    return (myUni == otherUni) ? uniWeight : 0;
}

int findMajorScore(const int& myMajor, const int& otherMajor, int majorWeight) {
    return (myMajor == otherMajor) ? majorWeight : 0;
}

int findDaysOfWeekScore(bitset<7> myDays, bitset<7> otherDays, int daysWeight) {
    if (myDays.count() == 0) {return 0;}
    return (myDays & otherDays).count() * daysWeight / myDays.count() ;
}

int findStudyTimeScore(bitset<5> myTime, bitset<5> otherTime, int timesWeight) {
    if (myTime.count() == 0) {return 0;}
    return (myTime & otherTime).count() * timesWeight / myTime.count();
}

int calculateCompactibilityScore(const User &user1, const User &user2) {
    int ageScore = findAgeScore(user1.getAge(), user2.getAge(), user1.getAgeWeight());
    int genderScore = findGenderScore(user1.getGender(), user2.getGender(), user1.getGenderWeight());
    int modeScore = findModeScore(user1.getMode(), user2.getMode(), user1.getModeWeight());
    int uniScore = findUniScore(user1.getUniId(), user2.getUniId(), user1.getUniWeight());
    int majorScore = findMajorScore(user1.getMajorId(), user2.getMajorId(), user1.getMajorWeight());
    int daysOfWeekScore = findDaysOfWeekScore(user1.getStudyDays(), user2.getStudyDays(), user1.getDaysWeight());
    int studyTimeScore = findStudyTimeScore(user1.getStudyTimes(), user2.getStudyTimes(), user1.getTimesWeight());

    return ageScore + genderScore + modeScore + uniScore + majorScore + daysOfWeekScore + studyTimeScore;
}

int calculateCompactibilityScore(const User &user1, const Centroid &centroid) {
    int ageScore = findAgeScore(user1.getAge(), centroid.getAge(), user1.getAgeWeight());
    int genderScore = findGenderScore(user1.getGender(), centroid.getGender(), user1.getGenderWeight());
    int modeScore = findModeScore(user1.getMode(), centroid.getMode(), user1.getModeWeight());
    int uniScore = findUniScore(user1.getUniId(), centroid.getUniId(), user1.getUniWeight());
    int majorScore = findMajorScore(user1.getMajorId(), centroid.getMajorId(), user1.getMajorWeight());
    int daysOfWeekScore = findDaysOfWeekScore(user1.getStudyDays(), centroid.getStudyDays(), user1.getDaysWeight());
    int studyTimeScore = findStudyTimeScore(user1.getStudyTimes(), centroid.getStudyTimes(), user1.getTimesWeight());
    return ageScore + genderScore + modeScore + uniScore + majorScore + daysOfWeekScore + studyTimeScore;
};

vector<User> loadAll() {
    vector<User> allTheStuff;
    ifstream file(userDataFile);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                if (line.empty())
                    continue;
                stringstream ss(line);
                string tempUsername;
                getline(ss, tempUsername, ','); 
                    vector<string> output;
                    string temp;
                    while (getline(ss, temp,',')) {
                        output.push_back(temp);
                    }
                    allTheStuff.push_back(User(tempUsername, output[0], output[1], stoi(output[2]), stoi(output[3]), stoi(output[4]), stoi(output[5]), stoi(output[6]), bitset<7>(output[7]), bitset<5>(output[8]), stoi(output[9]), stoi(output[10]), stoi(output[11]), stoi(output[12]), stoi(output[13]),stoi(output[14]),stoi(output[15])));
            }
        } else {
            cout << "Unable to open user data file." << endl;
        }
        file.close();
        return allTheStuff;
}


//K MEANS
void clusterize(const vector<User>& users, const vector<Centroid>& centroids, vector<int>& clusters) {
    for (int i = 0; i < users.size(); i++) {
        int maxScore = 0;
        int closestCluster = 0;
        for (int j = 0; j < centroids.size(); j++) {
            int score = calculateCompactibilityScore(users[i], centroids[j]);
            if (score > maxScore) {
                maxScore = score;
                closestCluster = j;
            }
        }
        clusters[i] = closestCluster;
    }
}


void updateCentroids(const vector<User>& users, vector<Centroid>& centroids, const vector<int>& clusters) {
    vector<vector<double>> weightSum(centroids.size(), vector<double>(14, 0));
    vector<int> count(centroids.size(),0);

    for (int i = 0; i < users.size(); ++i) {
        weightSum[clusters[i]][0] += users[i].getAgeWeight();
        weightSum[clusters[i]][1] += users[i].getGenderWeight();
        weightSum[clusters[i]][2] += users[i].getModeWeight();
        weightSum[clusters[i]][3] += users[i].getUniWeight();
        weightSum[clusters[i]][4] += users[i].getMajorWeight();
        weightSum[clusters[i]][5] += users[i].getDaysWeight();
        weightSum[clusters[i]][6] += users[i].getTimesWeight();

        weightSum[clusters[i]][7] += users[i].getAge();
        weightSum[clusters[i]][8] += users[i].getGender();
        weightSum[clusters[i]][9] += users[i].getMode();
        weightSum[clusters[i]][10] += users[i].getUniId();
        weightSum[clusters[i]][11] += users[i].getMajorId();
        weightSum[clusters[i]][12] += users[i].getStudyDays().to_ulong();
        weightSum[clusters[i]][13] += users[i].getStudyTimes().to_ulong();

        count[clusters[i]]++;
    }

    for (int i = 0; i < centroids.size(); ++i) {
        if (count[i] > 0) {
            for (int k = 0; k < 14; ++k) {
                weightSum[i][k] /= count[i];
            }
            centroids[i].setValues(weightSum[i][0],weightSum[i][1],weightSum[i][2],weightSum[i][3],weightSum[i][4],weightSum[i][5],weightSum[i][6],weightSum[i][7],weightSum[i][8],weightSum[i][9],weightSum[i][10],weightSum[i][11],weightSum[i][12],weightSum[i][13]);
        }
    }
}

unordered_map<string,bool> loadSwipes(User inpUser) {
    unordered_map<string, bool> swipes;
    ifstream leftFile("left_swipe.csv"), rightFile("right_swipe.csv");
    string line;

    auto processLine = [&](ifstream& file, int swipeValue) {
        while (getline(file, line)) {
            stringstream ss(line);
            string user, swipedUser;
            getline(ss, user, ',');
            if (user == inpUser.getName()) {
                while (getline(ss, swipedUser, ',')) {
                    swipedUser.erase(remove(swipedUser.begin(), swipedUser.end(), ' '), swipedUser.end());  
                    swipes[swipedUser] = swipeValue;  
                }
            }
        }
    };

    processLine(leftFile, false);
    processLine(rightFile, true);

    leftFile.close();
    rightFile.close();

    return swipes;  
}

vector<vector<string>> readCSV(const string& filename) {
    vector<vector<string>> data;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return data;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        vector<string> row;
        string cell;
        while (getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        data.push_back(row);
    }
    file.close();
    return data;
}

void writeCSV(const string& filename, const vector<vector<string>>& data) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i != row.size() - 1) {
                file << ",";
            }
        }
        file << "\n";
    }
    file.close();
}

void mapToCSV(const string& leftFile, const string& rightFile, const unordered_map<string, bool>& hashmap, const string& username) {
    vector<vector<string>> leftData = readCSV(leftFile);
    vector<vector<string>> rightData = readCSV(rightFile);

    for (auto& row : leftData) {
        if (!row.empty() && row[0] == username) {
            if (hashmap.find(username) != hashmap.end() && hashmap.at(username)) {
                row[0] = "true"; // Replace with keys from hashmap for true
            } else {
                row[0] = "false"; // Replace with keys from hashmap for false
            }
        }
    }

    for (auto& row : rightData) {
        if (!row.empty() && row[0] == username) {
            if (hashmap.find(username) != hashmap.end() && !hashmap.at(username)) {
                row[0] = "false"; // Replace with keys from hashmap for false
            } else {
                row[0] = "true"; // Replace with keys from hashmap for true
            }
        }
    }

    // Write the modified data back to CSV files
    writeCSV(leftFile, leftData);
    writeCSV(rightFile, rightData);
}

vector<Centroid> initializeCentroids(int k) {
    vector<Centroid> centroids;
    for (int i = 0; i < k; ++i) {
        Centroid a;
        centroids.push_back(a);
    }
    return centroids;
}

vector<int> kmeans(vector<User>& users, vector<Centroid>& centroids, vector<int>& clusters, int maxIter) {
    int iter = 0;
    bool converged = false;

    while (iter < maxIter && !converged) {
        clusterize(users, centroids, clusters);
        updateCentroids(users, centroids, clusters);
        iter++;
        // convergence check
    }

    return clusters;
}

int findUserCluster(const User& user, const vector<Centroid>& centroids) {
    int maxScore = 0; 
    int clusterIndex = -1;
    for (int i = 0; i < centroids.size(); ++i) {
        int score = calculateCompactibilityScore(user, centroids[i]);
        if (score > maxScore) {
            maxScore = score;
            clusterIndex = i; // Update cluster index for maximum score
        }
    }
    return clusterIndex;
}

// User findNextUserInCluster(const vector<User>& users, const vector<int>& clusters, int targetCluster, int currentIndex) {
//     for (int i = currentIndex + 1; i < users.size(); ++i) {
//         if (clusters[i] == targetCluster) {
//             return users[i]; 
//         }
//     }
//     cout << "This is blank";
//     return User(); 
// }
User findNextUserInCluster(const vector<vector<User>> clusterData, int targetCluster, int currentIndex) {
    int clusterSize = clusterData[targetCluster].size();
    currentIndex = (currentIndex + 1) % clusterSize;
    return clusterData[targetCluster][currentIndex];
}

vector<vector<User>> to2dVector(const vector<User>& users, const vector<int>& clusters) {
    vector<vector<User>> clusterData(10);
    for (int i = 0; i < users.size(); ++i)
        clusterData[clusters[i]].push_back(users[i]);


    return clusterData;
}


char getUserChoice(bool isLoggedIn) {
    char choice;
    if (isLoggedIn) {
        cout << "Enter 'L' to reject or 'R' to match. Enter 'H' for further help : ";
    }
    else
        cout << "Enter 'L' to login or 'R' to register: ";

    cin >> choice;
    return toupper(choice);
}

void printHelp() {
    cout << "Welcome to Stumbl!" << endl;
    cout << "-------------------------------------" << endl;
    cout << "Commands:" << endl;
    cout << "H: Help" << endl;
    cout << "L: Swipe left" << endl;
    cout << "R: Swipe right" << endl;
    cout << "O: Logout" << endl;
    cout << "V: View Profile" << endl;
    cout << "-------------------------------------" << endl;
}


int main() {
    // depopulateCsv();
    // populateCsv(180);

    bool isLoggedIn = false;
    User* curr;
    unordered_map<string,bool> swipedMap;

    vector<User> users = loadAll();
    vector<Centroid> centroids = initializeCentroids(10);
    vector<int> clusters(users.size(), 0);

    vector<int> PRAY = kmeans(users, centroids, clusters, 10);
    vector<vector<User>> clusterData = to2dVector(users, clusters);

    //Before Login
    bool done = false;
    while (true) {
        switch (getUserChoice(false)) {
            {case 'L':
                User temp = User::login(); 
                curr = &temp;
                isLoggedIn = true;
                done = true;
                break;}

            {case 'R':
                User temp = User::registerUser();
                curr = &temp;
                isLoggedIn = true;
                done = true;
                break;}

            default:
                cout << "Input Invalid";
                break;
        }
        if (done) {
            break;
        }
    }
    //After Login
    done = false;
    bool displayNewUser = true;
    int cluster = findUserCluster((*curr), centroids);
    int dupli_count = 0;
    while(true){
        int startIndex = rand() % users.size();
        User viewUser = findNextUserInCluster(clusterData, cluster, startIndex);
        if (swipedMap.find(viewUser.getName()) != swipedMap.end()) {
            dupli_count += 1;
            if (dupli_count > 100) {
                cluster += 1;
            }
            continue;
        }
        dupli_count = 0;   
        if (displayNewUser) {
            cout << "\nHere's your next user \n";
            cout << viewUser.toString();
        }
        switch (getUserChoice(true)) {
            case 'H':
                printHelp();
                displayNewUser = false;
                break;

            case 'L':
                curr->swipeLeft(viewUser, swipedMap);
                displayNewUser = true;
                break;

            case 'R':
                curr->swipeRight(viewUser, swipedMap);
                displayNewUser = true;
                break;

            case 'O':
                curr->logout();
                isLoggedIn = false;
                done = true;
                break;

            case 'V':
                cout << curr->toString();
                displayNewUser = false;
                break;

            default:
                cout << "Input Invalid. Type H for Help";
                displayNewUser = false;
                break;
        }

        if (done){
            break;
        }
    }
}