#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
using namespace std;

struct Student {
    int id;
    string name;
    string department;
    double cgpa;
};

vector<Student> students;
const string FILE_NAME = "students.txt";

void loadFromFile() {
    ifstream file(FILE_NAME);
    if (!file) return;
    Student s;
    while (file >> s.id) {
        file.ignore();
        getline(file, s.name);
        getline(file, s.department);
        file >> s.cgpa;
        students.push_back(s);
    }
    file.close();
}

void saveToFile() {
    ofstream file(FILE_NAME);
    for (const Student& s : students) {
        file << s.id << "\n" << s.name << "\n"
             << s.department << "\n" << s.cgpa << "\n";
    }
    file.close();
}

int findIndexById(int id) {
    for (size_t i = 0; i < students.size(); i++) {
        if (students[i].id == id) return i;
    }
    return -1;
}

void addStudent() {
    Student s;
    cout << "Enter ID: ";
    cin >> s.id;
    if (findIndexById(s.id) != -1) {
        cout << "A student with this ID already exists.\n";
        return;
    }
    cin.ignore();
    cout << "Enter Name: ";
    getline(cin, s.name);
    cout << "Enter Department: ";
    getline(cin, s.department);
    cout << "Enter CGPA: ";
    cin >> s.cgpa;
    students.push_back(s);
    saveToFile();
    cout << "Student added successfully.\n";
}

void displayAll() {
    if (students.empty()) {
        cout << "No students found.\n";
        return;
    }
    cout << "\nID\tName\t\tDepartment\tCGPA\n";
    cout << "--------------------------------------------------\n";
    for (const Student& s : students) {
        cout << s.id << "\t" << s.name << "\t\t"
             << s.department << "\t\t" << s.cgpa << "\n";
    }
}

void searchStudent() {
    int id;
    cout << "Enter ID to search: ";
    cin >> id;
    int i = findIndexById(id);
    if (i == -1) {
        cout << "Student not found.\n";
        return;
    }
    cout << "Name: " << students[i].name << "\n"
         << "Department: " << students[i].department << "\n"
         << "CGPA: " << students[i].cgpa << "\n";
}

void updateStudent() {
    int id;
    cout << "Enter ID to update: ";
    cin >> id;
    int i = findIndexById(id);
    if (i == -1) {
        cout << "Student not found.\n";
        return;
    }
    cin.ignore();
    cout << "New Name: ";
    getline(cin, students[i].name);
    cout << "New Department: ";
    getline(cin, students[i].department);
    cout << "New CGPA: ";
    cin >> students[i].cgpa;
    saveToFile();
    cout << "Student updated successfully.\n";
}

void deleteStudent() {
    int id;
    cout << "Enter ID to delete: ";
    cin >> id;
    int i = findIndexById(id);
    if (i == -1) {
        cout << "Student not found.\n";
        return;
    }
    students.erase(students.begin() + i);
    saveToFile();
    cout << "Student deleted successfully.\n";
}

int main() {
    loadFromFile();
    int choice;
    do {
        cout << "\n=== Student Management System ===\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student\n";
        cout << "4. Update Student\n";
        cout << "5. Delete Student\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
        }

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayAll(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 0: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}