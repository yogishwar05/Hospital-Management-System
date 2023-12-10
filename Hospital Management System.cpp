#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstring>

using namespace std;

struct User {
    char username[50];
    char password[50];
};


bool loginUser(const char* username, const char* password) {
    const char* predefinedUsername = "srmhospitals";
    const char* predefinedPassword = "srm123";

    return (strcmp(username, predefinedUsername) == 0 && strcmp(password, predefinedPassword) == 0);
}


bool login() {
    char username[50];
    char password[50];
    std::cout << "\n====================================================================" << std::endl;
    std::cout << "\n\t Welcome to SRM Hospitals " << std::endl;
    std::cout << "\n====================================================================" << std::endl;
    std::cout << "\n\n++++++++++++++++++++++++ User Login ++++++++++++++++++++++++\n" << std::endl;
    cout << "Enter your username: ";
    cin >> username;

    cout << "Enter your password: ";
    cin >> password;

    return loginUser(username, password);
}

class Patient {
public:
    long int opno;
    char name[50];
    char dis[50];
    long int mobilenumber;
};

void enterRecord() {
    Patient p1;
    
    srand(static_cast<unsigned int>(time(nullptr)));

    p1.opno = rand() % 1000 + 1;

    cout << "Enter Name: ";
    cin.ignore(); 
    cin.getline(p1.name, sizeof(p1.name));

    cout << "Enter the problem or disease: ";
    cin.getline(p1.dis, sizeof(p1.dis));

    cout << "Enter the mobile number: ";
    cin >> p1.mobilenumber;

    ofstream file("PATIENTRECORDS1.dat", ios::binary | ios::app);

    if (!file.is_open()) {
        cerr << "Error opening file for writing." << endl;
        return;
    }

    file.write(reinterpret_cast<char*>(&p1), sizeof(p1));

    file.close();
}

void showRecord() {
    Patient p1;

    ifstream file("PATIENTRECORDS1.dat", ios::binary);

    if (!file.is_open()) {
        cerr << "Error opening file for reading." << endl;
        return;
    }

    cout << "OP No\tName\tDisease\tMobile Number" << endl;

    while (file.read(reinterpret_cast<char*>(&p1), sizeof(p1))) {
        cout << p1.opno << "\t" << p1.name << "\t" << p1.dis << "\t" << p1.mobilenumber << endl;
    }

    file.close();
}

void deleteRecord() {
    Patient p1;
    int found = 0;
    long int opno;

    cout << "Enter the OP number that you want to delete: ";
    cin >> opno;

    ifstream fileIn("PATIENTRECORDS1.dat", ios::in | ios::binary);
    ofstream fileOut("PATIENTRECORDS2.dat", ios::out | ios::binary);

    if (!fileIn.is_open() || !fileOut.is_open()) {
        cerr << "Error opening files." << endl;
        return;
    }

    while (fileIn.read(reinterpret_cast<char*>(&p1), sizeof(p1))) {
        if (opno == p1.opno) {
            cout << "\nAccount Deleted Successfully." << endl;
            found = 1;
            continue;
        }
        fileOut.write(reinterpret_cast<char*>(&p1), sizeof(p1));
    }

    fileIn.close();
    fileOut.close();

    if (found == 0) {
        cout << "\nNo patient found with that OP number." << endl;
    } else {
        if (remove("PATIENTRECORDS1.dat") != 0) {
            cerr << "Error deleting file." << endl;
        }

        if (rename("PATIENTRECORDS2.dat", "PATIENTRECORDS1.dat") != 0) {
            cerr << "Error renaming file." << endl;
        }
    }
}

void updateRecord() {
    Patient p1;
    long int opno;

    cout << "Enter the OP number for the record you want to update: ";
    cin >> opno;

    fstream file("PATIENTRECORDS1.dat", ios::in | ios::out | ios::binary);

    if (!file.is_open()) {
        cerr << "Error opening file for updating." << endl;
        return;
    }

    bool found = false;

    while (file.read(reinterpret_cast<char*>(&p1), sizeof(p1))) {
        if (opno == p1.opno) {
            cout << "Enter Name: ";
            cin.ignore();
            cin.getline(p1.name, sizeof(p1.name));

            cout << "Enter the problem or disease: ";
            cin.getline(p1.dis, sizeof(p1.dis));

            cout << "Enter the mobile number: ";
            cin >> p1.mobilenumber;

            file.seekp(-static_cast<int>(sizeof(p1)), ios::cur);

            file.write(reinterpret_cast<char*>(&p1), sizeof(p1));

            found = true;
            cout << "Record updated successfully." << endl;
            break;
        }
    }

    file.close();

    if (!found) {
        cout << "No patient found with that OP number." << endl;
    }
}

int main() {
    if (!login()) {
        cout << "Login failed. Exiting program." << endl;
        return 0;
    }

    int choice;

    while (choice != 5) {
        cout << "\n\n\n\n1. Enter a patient record.\n";
        cout << "2. Display patient records.\n";
        cout << "3. Delete a patient record.\n";
        cout << "4. Update a patient record.\n";
        cout << "5. Exit.\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            enterRecord();
            break;
        case 2:
            showRecord();
            break;
        case 3:
            deleteRecord();
            break;
        case 4:
            updateRecord();
            break;
        case 5:
            exit(0);
        default:
            cout << "Invalid choice. Try again." << endl;
        }
    }

    return 0;
}