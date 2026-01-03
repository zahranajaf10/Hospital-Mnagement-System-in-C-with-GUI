#include <iostream>
#include <fstream>
using namespace std;

struct Patient {
    int id;
    char name[50];
    int age;
    char disease[50];
};

int main() {
    Patient p;
    int choice;

    do {
        cout << "\n--- Hospital Management System ---\n";
        cout << "1. Add Patient\n";
        cout << "2. View Patients\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            ofstream file("patients.txt", ios::app);

            cout << "Enter Patient ID: ";
            cin >> p.id;
            cout << "Enter Name: ";
            cin >> p.name;
            cout << "Enter Age: ";
            cin >> p.age;
            cout << "Enter Disease: ";
            cin >> p.disease;

            file << p.id << " " << p.name << " "
                 << p.age << " " << p.disease << endl;

            file.close();
            cout << "Patient added successfully.\n";
        }
        else if (choice == 2) {
            ifstream file("patients.txt");

            cout << "\nID  Name  Age  Disease\n";
            while (file >> p.id >> p.name >> p.age >> p.disease) {
                cout << p.id << "   " << p.name << "   "
                     << p.age << "   " << p.disease << endl;
            }

            file.close();
        }
        else if (choice == 3) {
            cout << "Exiting program...\n";
        }
        else {
            cout << "Invalid choice!\n";
        }

    } while (choice != 3);

    return 0;
}
